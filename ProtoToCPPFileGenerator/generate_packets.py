import re
from pathlib import Path

# --- 헤더/CPP 상단 고정 텍스트 ---
HEADER_PREFIX = """#pragma once

#include "ProtoStruct.h"
#include "Protobuf/Protocol.pb.h"

class CSession;
class CRoom;
"""

CPP_PREFIX = """#include "pch.h"
#include "MakePacket.h"
#include "SessionManager.h"
#include "Packet.h"
#include "Player.h"
#include "Room.h"
#include "MemoryPoolManager.h"
#include "Protobuf/Protocol.pb.h"

"""

# 기본 타입 매핑
PROTO_TO_CPP_TYPE = {
    'fixed32': 'UINT32',
    'sfixed32': 'INT32',
    'fixed64': 'UINT64',
    'sfixed64': 'INT64',
    'uint32': 'UINT32',
    'int32': 'INT32',
    'uint64': 'UINT64',
    'int64': 'INT64',
    'float': 'float',
    'double': 'double',
    'bool': 'bool',
    'string': 'std::string',
}

# 스네이크 케이스 -> PascalCase
def to_pascal_case(snake_str):
    return ''.join(word.capitalize() for word in snake_str.split('_'))


def parse_proto(proto_path):
    text = Path(proto_path).read_text(encoding='utf-8')

    # package 이름 추출
    pkg = ''
    pkg_match = re.search(r'package\s+([A-Za-z0-9_]+)\s*;', text)
    if pkg_match:
        pkg = pkg_match.group(1)

    # 모든 메시지(구조체 포함) 파싱
    struct_map = {}
    for name, body in re.findall(r'message\s+([A-Za-z0-9_]+)\s*{([^}]*)}', text, re.MULTILINE):
        fields = []
        for line in body.splitlines():
            m_rep = re.match(r'\s*repeated\s+(\w+)\s+(\w+)\s*=\s*\d+;', line)
            m_sng = re.match(r'\s*(\w+)\s+(\w+)\s*=\s*\d+;', line)
            if m_rep:
                fields.append((m_rep.group(1), m_rep.group(2), True))
            elif m_sng:
                fields.append((m_sng.group(1), m_sng.group(2), False))
        struct_map[name] = fields

    # SC_ 메시지만 RPC 처리 대상
    messages = []
    for name, body in re.findall(r'message\s+(SC_[A-Z0-9_]+)\s*{([^}]*)}', text, re.MULTILINE):
        fields = []
        for line in body.splitlines():
            m_rep = re.match(r'\s*repeated\s+(\w+)\s+(\w+)\s*=\s*\d+;', line)
            m_sng = re.match(r'\s*(\w+)\s+(\w+)\s*=\s*\d+;', line)
            if m_rep:
                fields.append((m_rep.group(1), m_rep.group(2), True))
            elif m_sng:
                fields.append((m_sng.group(1), m_sng.group(2), False))
        messages.append((name, fields))

    return messages, struct_map, pkg


def get_cpp_type(ft, pkg, struct_map):
    lt = ft.lower()
    if lt in PROTO_TO_CPP_TYPE:
        return PROTO_TO_CPP_TYPE[lt]
    if ft in struct_map:
        return ft
    return f"{pkg}::{ft}" if pkg else ft


def get_inner_cpp_type(ft, pkg):
    if ft.startswith('SC_') or ft.startswith('CS_'):
        return ft
    return f"{pkg}::{ft}" if pkg else ft


def expand_repeated(ft, name, struct_map, pkg):
    """
    repeated 필드 처리
    - 기본 타입 및 enum: Reserve + Add(begin,end)
    - 메시지 구조체: Reserve + for loop + 필드별 setter 호출
    """
    lines = []

    # mutable list 가져오기 및 용량 예약
    lines.append(f"    auto* list = pkt.mutable_{name.lower()}();")
    lines.append(f"    list->Reserve(static_cast<int>({name}.size()));")
    lines.append("")

    # 1) 기본 타입 or enum
    if ft.lower() in PROTO_TO_CPP_TYPE:
        lines.append("    // 기본 타입: iterator 기반 복사")
        lines.append(f"    list->Add({name}.begin(), {name}.end());")
        lines.append("")

    # 2) 메시지 구조체
    elif ft in struct_map:
        lines.append("    // 메시지 구조체: 각 요소 필드별 setter 호출")
        lines.append(f"    for (const auto& elem : {name}) {{")
        lines.append(f"        auto* msg = pkt.add_{name.lower()}();")
        # 각 서브 필드 별 setter 추가
        for sub_t, sub_n, _ in struct_map[ft]:
            lower_n = sub_n.lower()
            lines.append(f"        msg->set_{lower_n}(elem.{sub_n});")
        lines.append("    }")
        lines.append("")

    # 3) fallback (기타 타입)
    else:
        lines.append("    // 기타 타입: 기본 방식 복사")
        lines.append(f"    list->Add({name}.begin(), {name}.end());")
        lines.append("")

    return '\n'.join(lines)


def setter_lines(fields, struct_map, pkg):
    out = []
    for ft, fn, rep in fields:
        if rep:
            out.append(expand_repeated(ft, fn, struct_map, pkg))
        else:
            if ft in struct_map:
                inner = get_inner_cpp_type(ft, pkg)
                out += [
                    "    {  // 구조체 단일 필드 시작",
                    f"        {inner}* sub = pkt.mutable_{fn.lower()}();"
                ]
                for sub_t, sub_n, _ in struct_map[ft]:
                    out.append(f"        sub->set_{sub_n.lower()}({fn}.{sub_n});")
                out.append("    }  // 구조체 단일 필드 끝")
                out.append("")
            else:
                out.append(f"    pkt.set_{fn.lower()}({fn});")
                out.append("")
    return '\n'.join(out)


def generate_header(msgs, pkg, struct_map):
    lines = [HEADER_PREFIX, ""]
    for name, fields in msgs:
        args = []
        for ft, fn, rep in fields:
            t = get_cpp_type(ft, pkg, struct_map)
            args.append(f"{'std::vector<' + t + '>& ' + fn if rep else t + ' ' + fn}")
        sig = ', '.join(args)
        for sfx in ['FOR_All', 'FOR_SINGLE', 'FOR_AROUND']:
            pre = f"void {name}_{sfx}(CSession* pSession"
            if sfx == 'FOR_AROUND':
                pre += ", CRoom* pRoom"
            pre += (", " + sig if sig else "") + ");"
            lines.append(pre)
        lines.append("")
    return '\n'.join(lines)


def generate_cpp(msgs, struct_map, pkg):
    lines = [CPP_PREFIX]
    for name, fields in msgs:
        base = name[3:]
        enum_id = f"SC_{to_pascal_case(base)}"
        args = []
        for ft, fn, rep in fields:
            t = get_cpp_type(ft, pkg, struct_map)
            args.append(f"{'std::vector<' + t + '>& ' + fn if rep else t + ' ' + fn}")
        sig = ', '.join(args)
        block = setter_lines(fields, struct_map, pkg)

        for sfx, send in [
            ('FOR_All', 'BroadcastData(pSession, Packet, Packet->GetDataSize());'),
            ('FOR_SINGLE', 'UnicastPacket(pSession, &header, Packet);'),
            ('FOR_AROUND', 'for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }'),
        ]:
            sig_pref = f"void {name}_{sfx}(CSession* pSession"
            if sfx == 'FOR_AROUND':
                sig_pref += ", CRoom* pRoom"
            sig_pref += (", " + sig if sig else "") + ")"
            lines.append(sig_pref)
            lines.append("{")
            pkg_name = f"{pkg}::" if pkg else ''
            lines.append(f"    {pkg_name}{name} pkt;\n")
            lines.append(block)
            lines.append("    int pktSize = pkt.ByteSizeLong();")
            lines.append("    PACKET_HEADER header;")
            lines.append("    header.byCode = dfNETWORK_PACKET_CODE;")
            lines.append("    header.bySize = pktSize;")
            lines.append(f"    header.byType = game::PacketID::{enum_id};\n")
            lines.append("    CPacket* Packet = packetPool.Alloc();")
            lines.append("    char buffer[512];")
            lines.append("    pkt.SerializeToArray(buffer, pktSize);")
            lines.append("    Packet->PutData(buffer, pktSize);")
            lines.append(f"    {send}\n")
            lines.append("    Packet->Clear();")
            lines.append("    packetPool.Free(Packet);")
            lines.append("}\n")
        lines.append("")
    return '\n'.join(lines)


def main():
    proto = 'Protocol.proto'
    msgs, struct_map, pkg = parse_proto(proto)
    Path('MakePacket.h').write_text(generate_header(msgs, pkg, struct_map), encoding='utf-8')
    Path('MakePacket.cpp').write_text(generate_cpp(msgs, struct_map, pkg), encoding='utf-8')
    print(f"Generated MakePacket.h/cpp from {proto}")


if __name__ == '__main__':
    main()
