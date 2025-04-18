# -*- coding: utf-8 -*-
import re
from pathlib import Path

# .proto 기본 타입 → C++ 타입 매핑
PROTO_TO_CPP_TYPE = {
    "uint32": "UINT32",
    "int32": "INT32",
    "uint64": "UINT64",
    "int64": "INT64",
    "float": "float",
    "double": "double",
    "bool": "bool",
    "string": "std::string",
    "fixed32": "UINT32",
    "sfixed32": "INT32",
    "fixed64": "UINT64",
    "sfixed64": "INT64",
}

def to_pascal_case(snake_str):
    # enum ID 생성용: snake→PascalCase
    return ''.join(word.capitalize() for word in snake_str.split('_'))

def parse_proto(proto_path):
    text = Path(proto_path).read_text(encoding='utf-8')
    msg_pattern = re.compile(r'message\s+(\w+)\s*{([^}]*)}', re.MULTILINE)
    all_msgs = {}
    for m in msg_pattern.finditer(text):
        name = m.group(1)
        body = m.group(2)
        fields = []
        fld_pat = re.compile(r'\s*(repeated\s+)?(\w+)\s+(\w+)\s*=\s*\d+;')
        for line in body.splitlines():
            match = fld_pat.match(line)
            if not match:
                continue
            is_rep  = bool(match.group(1))
            proto_t = match.group(2)
            name_t  = match.group(3)
            lower_t = proto_t.lower()

            if lower_t in PROTO_TO_CPP_TYPE:
                cpp_t  = PROTO_TO_CPP_TYPE[lower_t]
                is_msg = False
            else:
                cpp_t  = proto_t      # 원문 대소문자 그대로
                is_msg = True

            fields.append((cpp_t, name_t, is_rep, is_msg))
        all_msgs[name] = fields

    # CS_ 메시지만 RPC용
    rpc_msgs = {n: f for n, f in all_msgs.items() if n.startswith("CS_")}
    return rpc_msgs, all_msgs

def generate_header(rpc_msgs):
    header = '''#pragma once

#include "Session.h"
#include "Protobuf/Protocol.pb.h"
#include "ProtoStruct.h"

extern int g_iSyncCount;

class CObject;
class CSession;

bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket);
void DisconnectSessionProc(CSession* pSession);
'''
    parts = [header]
    for msg, fields in rpc_msgs.items():
        args = []
        for typ, name, rep, is_msg in fields:
            if rep:
                args.append(f"const std::vector<{typ}>& {name}")
            else:
                args.append(f"{typ} {name}")
        arg_str = ", " + ", ".join(args) if args else ""
        parts.append(f"bool {msg}(CSession* pSession{arg_str});")
    return "\n".join(parts)

def generate_cpp(rpc_msgs, all_msgs):
    includes = '''#include "pch.h"
#include "PacketProc.h"
#include "Packet.h"
#include "MakePacket.h"

#include "SessionManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "MemoryPoolManager.h"
#include "TimerManager.h"
#include "LogManager.h"
#include "RoomManager.h"
#include "LoginManager.h"
#include "AIManager.h"
#include "EncodingUtils.h"

static CObjectManager& objectManager = CObjectManager::GetInstance();
static CTimerManager& timerManager     = CTimerManager::GetInstance();
static LogManager&    logManager       = LogManager::GetInstance();
static CRoomManager&  roomManager      = CRoomManager::GetInstance();
static CLoginManager& loginManager     = CLoginManager::GetInstance();
static CAIManager&    aiManager        = CAIManager::GetInstance();

int g_iSyncCount = 0;
'''
    lines = [includes]

    # PacketProc 함수
    lines.append("bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket)")
    lines.append("{")
    lines.append("    switch (packetType)")
    lines.append("    {")

    for msg, fields in rpc_msgs.items():
        suffix  = msg[len("CS_") :]
        enum_id = "CS_" + to_pascal_case(suffix)

        lines.append(f"    case game::PacketID::{enum_id}:")
        lines.append("    {")
        # 변수 선언
        for typ, name, rep, is_msg in fields:
            decl = f"std::vector<{typ}>" if rep else typ
            lines.append(f"        {decl} {name};")
        lines.append("")  # 빈 줄

        # Protobuf 파싱 (모두 소문자 호출)
        lines.append(f"        game::{msg} pkt;")
        lines.append(f"        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());")
        lines.append("")  # 빈 줄

        # 필드 복사 로직
        for typ, name, rep, is_msg in fields:
            if rep:
                fld_low = name.lower()
                lines.append(f"        auto* __list_{name} = pkt.mutable_{fld_low}();")
                lines.append(f"        {name}.reserve(__list_{name}->size());")

                if not is_msg:
                    # 기본 타입
                    lines.append(f"        for (const auto& v : *__list_{name}) {name}.push_back(v);")
                else:
                    # 메시지 구조체
                    struct_fields = all_msgs.get(typ, [])
                    lines.append(f"        for (const auto& v : *__list_{name})")
                    lines.append("        {")
                    lines.append(f"            {typ} tmp;")
                    for sf_t, sf_n, _, _ in struct_fields:
                        sub_low = sf_n.lower()
                        lines.append(f"            tmp.{sf_n} = v.{sub_low}();")
                    lines.append(f"            {name}.push_back(tmp);")
                    lines.append("        }")
                lines.append("")  # 빈 줄
            else:
                getter_low = name.lower()
                lines.append(f"        {name} = pkt.{getter_low}();")
        lines.append("")  # 빈 줄

        # 핸들러 호출
        args_call = ", ".join(n for _, n, _, _ in fields)
        call_str  = f"(pSession, {args_call})" if args_call else "(pSession)"
        lines.append(f"        return {msg}{call_str};")
        lines.append("    }")
        lines.append("    break;")
        lines.append("")  # 빈 줄

    lines.append("    default:")
    lines.append("        break;")
    lines.append("    }")
    lines.append("    return false;")
    lines.append("}")
    lines.append("")  # 빈 줄

    # DisconnectSessionProc stub
    lines.append("void DisconnectSessionProc(CSession* pSession)")
    lines.append("{")
    lines.append("    return;")
    lines.append("}")
    lines.append("")  # 빈 줄

    # RPC 핸들러 stubs
    for msg, fields in rpc_msgs.items():
        args = []
        for typ, name, rep, is_msg in fields:
            if rep:
                args.append(f"const std::vector<{typ}>& {name}")
            else:
                args.append(f"{typ} {name}")
        sig = ", ".join(args)
        lines.append(f"bool {msg}(CSession* pSession{', ' + sig if sig else ''})")
        lines.append("{")
        lines.append("    // TODO: Implement handler")
        lines.append("    return false;")
        lines.append("}")
        lines.append("")  # 빈 줄

    return "\n".join(lines)

if __name__ == "__main__":
    rpc_msgs, all_msgs = parse_proto("Protocol.proto")
    Path("PacketProc.h").write_text(generate_header(rpc_msgs), encoding="utf-8")
    Path("PacketProc.cpp").write_text(generate_cpp(rpc_msgs, all_msgs), encoding="utf-8")
    print("Generated PacketProc.h and PacketProc.cpp")
