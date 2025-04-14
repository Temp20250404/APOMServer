import re
from pathlib import Path

# --- 헤더에 추가할 텍스트 ---
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

PROTO_TO_CPP_TYPE = {
    "fixed32": "UINT32",
    "sfixed32": "INT32",
    "fixed64": "UINT64",
    "sfixed64": "INT64",
    "uint32": "UINT32",
    "int32": "INT32",
    "uint64": "UINT64",
    "int64": "INT64",
    "float": "float",
    "double": "double",
    "bool": "bool",
    "string": "std::string",
}


def to_pascal_case(snake_str):
    return ''.join(word.capitalize() for word in snake_str.lower().split('_'))


def parse_proto(proto_path):
    text = Path(proto_path).read_text(encoding='utf-8')

    # --- package 이름 추출: 예를 들어 "package game;"인 경우 "game"을 추출 ---
    package_pattern = re.compile(r'package\s+([A-Za-z0-9_]+)\s*;')
    package_match = package_pattern.search(text)
    package_name = package_match.group(1) if package_match else ""

    # --- enum 처리 (PacketID에 한정, 필요한 경우 확장 가능) ---
    enum_dict = {}
    enum_pattern = re.compile(r'enum\s+PacketID\s*{([^}]+)}', re.MULTILINE)
    enum_block = enum_pattern.search(text)
    if enum_block:
        for line in enum_block.group(1).splitlines():
            match = re.match(r'\s*(SC_[A-Z0-9_]+)\s*=\s*(\d+);', line)
            if match:
                enum_dict[match.group(1)] = match.group(1)

    # --- 메시지(및 구조체) 파싱 ---
    # SC_ 접두사가 있는 메시지를 대상으로 함.
    message_pattern = re.compile(r'message\s+(SC_[A-Z0-9_]+)\s*{([^}]+)}', re.MULTILINE)
    # 전체 message(구조체) 파싱
    struct_pattern = re.compile(r'message\s+([A-Za-z0-9_]+)\s*{([^}]+)}', re.MULTILINE)
    messages = []
    struct_map = {}

    for struct_match in struct_pattern.finditer(text):
        name = struct_match.group(1)
        body = struct_match.group(2)
        fields = []
        for line in body.splitlines():
            repeated_match = re.match(r'\s*repeated\s+(\w+)\s+(\w+)\s*=\s*\d+;', line)
            single_match = re.match(r'\s*(\w+)\s+(\w+)\s*=\s*\d+;', line)
            if repeated_match:
                t, n = repeated_match.groups()
                fields.append((t.strip(), n.strip(), True))
            elif single_match:
                t, n = single_match.groups()
                fields.append((t.strip(), n.strip(), False))
        struct_map[name] = fields

    for message_match in message_pattern.finditer(text):
        msg_name = message_match.group(1)
        fields_block = message_match.group(2)

        fields = []
        for line in fields_block.splitlines():
            repeated_match = re.match(r'\s*repeated\s+(\w+)\s+(\w+)\s*=\s*\d+;', line)
            single_match = re.match(r'\s*(\w+)\s+(\w+)\s*=\s*\d+;', line)
            if repeated_match:
                field_type, field_name = repeated_match.groups()
                fields.append((field_type.strip(), field_name, True))
            elif single_match:
                field_type, field_name = single_match.groups()
                fields.append((field_type.strip(), field_name, False))

        messages.append((msg_name, fields))

    return messages, enum_dict, struct_map, package_name


def get_cpp_type(field_type, package_name, struct_map):
    """
    프로토 타입을 C++ 함수 인자용 타입으로 변환합니다.
    - 기본 타입(PROTO_TO_CPP_TYPE에 정의됨)은 그대로 반환
    - message로 정의된 타입 (struct_map에 있는 경우)은 package 네임스페이스 없이 반환
    - 그 외(주로 enum 타입)는 package 네임스페이스를 붙여 반환
    """
    if field_type in PROTO_TO_CPP_TYPE:
        return PROTO_TO_CPP_TYPE[field_type]
    if field_type in struct_map:
        return field_type
    return f"{package_name}::{field_type}" if package_name else field_type


def get_inner_cpp_type(field_type, package_name):
    """
    프로토콜 전송용 내부 코드(세터 코드)에서 사용하는 타입을 반환합니다.
    - 만약 message 타입이고, CS_나 SC_ 접두사가 붙지 않은 경우, package 네임스페이스를 붙여 반환합니다.
      (예: PlayerInfo -> game::PlayerInfo)
    - 그 외의 경우는 그대로 반환합니다.
    """
    if field_type.startswith("CS_") or field_type.startswith("SC_"):
        return field_type
    return f"{package_name}::{field_type}" if package_name else field_type


def expand_repeated_struct(type_name, field_name, struct_map, package_name):
    """
    repeated 필드에서 message 타입(내부 구조체)이 사용될 때 setter 코드를 생성합니다.
    내부 코드에서는 전송을 위해 package 네임스페이스가 붙은 타입이 사용되어야 합니다.
    """
    lines = []
    if type_name not in struct_map:
        # 기본 타입 및 enum인 경우에는 기본 반복문 코드를 생성합니다.
        return [
            f"    for (const auto& data : {field_name}) {{",
            f"        auto* item = pkt.add_{field_name.lower()}();",
            f"        *item = data;",
            "    }"
        ]
    lines.append(f"    for (const auto& data : {field_name}) {{")
    # 내부 전송용으로 타입에 package 네임스페이스 적용: 예, game::PlayerInfo
    inner_type = get_inner_cpp_type(type_name, package_name)
    lines.append(f"        {inner_type}* item = pkt.add_{field_name.lower()}();")
    for sub_type, sub_name, _ in struct_map[type_name]:
        if sub_type in struct_map:
            inner_sub_type = get_inner_cpp_type(sub_type, package_name)
            lines.append(f"        {inner_sub_type}* nested = item->mutable_{sub_name.lower()}();")
            for n_type, n_name, _ in struct_map[sub_type]:
                lines.append(f"        nested->set_{n_name.lower()}(data.{sub_name}.{n_name});")
        else:
            lines.append(f"        item->set_{sub_name.lower()}(data.{sub_name});")
    lines.append("    }")
    return lines


def setter_lines(fields, struct_map, package_name):
    """
    각 필드에 대해 setter 호출 코드를 생성합니다.
    - 만약 필드 타입이 message 타입(struct_map에 있으면),
      내부 코드에서는 전송을 위해 package 네임스페이스(예: game::)가 붙은 타입을 사용합니다.
    - 기본 타입은 그대로 setter 함수를 호출합니다.
    """
    result = []
    for field_type, field_name, is_repeated in fields:
        if is_repeated:
            result.extend(expand_repeated_struct(field_type, field_name, struct_map, package_name))
        else:
            if field_type in struct_map:
                result.append("    {")
                # 내부 전송용으로 message 타입에 package 네임스페이스 적용
                inner_type = get_inner_cpp_type(field_type, package_name)
                result.append(f"        {inner_type}* sub = pkt.mutable_{field_name.lower()}();")
                for sub_type, sub_name, _ in struct_map[field_type]:
                    if sub_type in struct_map:
                        inner_sub_type = get_inner_cpp_type(sub_type, package_name)
                        result.append(f"        {inner_sub_type}* nested = sub->mutable_{sub_name.lower()}();")
                        for n_type, n_name, _ in struct_map[sub_type]:
                            result.append(f"        nested->set_{n_name.lower()}({field_name}.{sub_name}.{n_name});")
                    else:
                        result.append(f"        sub->set_{sub_name.lower()}({field_name}.{sub_name});")
                result.append("    }")
            else:
                result.append(f"    pkt.set_{field_name.lower()}({field_name});")
    return '\n'.join(result)


def generate_header(messages, package_name, struct_map):
    """
    헤더 파일 내용을 생성합니다.
    함수 인자의 message 타입(내부 구조체)은 package 네임스페이스 없이 사용합니다.
    """
    lines = [HEADER_PREFIX]
    for msg_name, fields in messages:
        args = []
        for field_type, field_name, is_repeated in fields:
            # 함수 인자용 타입 변환: message 타입은 package 없이 그대로 출력됨.
            cpp_type_str = get_cpp_type(field_type, package_name, struct_map)
            if is_repeated:
                args.append(f"std::vector<{cpp_type_str}>& {field_name}")
            else:
                args.append(f"{cpp_type_str} {field_name}")
        arg_str = ', '.join(args)
        lines.append(f"void {msg_name}_FOR_All(CSession* pSession, {arg_str});")
        lines.append(f"void {msg_name}_FOR_SINGLE(CSession* pSession, {arg_str});")
        lines.append(f"void {msg_name}_FOR_AROUND(CSession* pSession, CRoom* pRoom, {arg_str});\n")
    return '\n'.join(lines)


def generate_cpp(messages, enum_dict, struct_map, package_name):
    """
    C++ 구현 파일 내용을 생성합니다.
    - 최상위 메시지 객체 생성 시에는 package 네임스페이스가 적용되어야 하며,
    - setter 코드에서는 내부 구조체(message 타입 중 CS_/SC_가 없는 타입)에 대해 package 네임스페이스를 추가합니다.
    """
    cpp = [CPP_PREFIX]

    for msg_name, fields in messages:
        # msg_name은 프로토 파일에서 정의된 메시지 타입 이름입니다.
        # enum_name은 PacketID 열거형 내의 식별자에 사용됩니다.
        msg_suffix = msg_name[3:]
        enum_name = f"SC_{to_pascal_case(msg_suffix)}"

        args = []
        for field_type, field_name, is_repeated in fields:
            # 함수 인자 타입: message 타입은 package 네임스페이스 없이 사용
            cpp_type_str = get_cpp_type(field_type, package_name, struct_map)
            if is_repeated:
                args.append(f"std::vector<{cpp_type_str}>& {field_name}")
            else:
                args.append(f"{cpp_type_str} {field_name}")
        arg_str = ', '.join(args)

        # setter_lines()를 통해 각 필드에 대해 setter 호출 코드를 생성합니다.
        setter_block = setter_lines(fields, struct_map, package_name)

        for suffix, body in [
            ("FOR_All", "BroadcastData(pSession, Packet, Packet->GetDataSize());"),
            ("FOR_SINGLE", "UnicastPacket(pSession, &header, Packet);"),
            ("FOR_AROUND", """for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }""")
        ]:
            prefix = f"void {msg_name}_{suffix}(CSession* pSession, "
            if suffix == "FOR_AROUND":
                prefix += "CRoom* pRoom, "
            prefix += arg_str + ")"
            cpp.append(prefix)
            cpp.append("{")

            # === 메시지 객체 생성 시 package 네임스페이스 적용 부분 ===
            # top-level 메시지 (예, SC_SPAWN_CHARACTER)은 생성할 때 package 네임스페이스를 붙여야 합니다.
            # 따라서 package_name이 존재하면 "game::SC_SPAWN_CHARACTER"와 같이 객체를 생성합니다.
            if package_name:
                msg_inst = f"{package_name}::{msg_name}"
            else:
                msg_inst = msg_name
            cpp.append(f"    {msg_inst} pkt;\n")
            # ========================================================

            cpp.append(setter_block + "\n")
            cpp.append("    int pktSize = pkt.ByteSizeLong();\n")
            cpp.append("    PACKET_HEADER header;")
            cpp.append("    header.byCode = dfNETWORK_PACKET_CODE;")
            cpp.append("    header.bySize = pktSize;")
            cpp.append(f"    header.byType = game::PacketID::{enum_name};\n")
            cpp.append("    int headerSize = sizeof(PACKET_HEADER);")
            cpp.append("    CPacket* Packet = packetPool.Alloc();\n")
            cpp.append("    char buffer[512];")
            cpp.append("    pkt.SerializeToArray(buffer, pktSize);")
            cpp.append("    Packet->PutData(buffer, pktSize);\n")
            cpp.append(f"    {body}")
            cpp.append("    Packet->Clear();")
            cpp.append("    packetPool.Free(Packet);")
            cpp.append("}\n")

    return '\n'.join(cpp)


def main():
    proto_file = "Protocol.proto"
    out_h = "MakePacket.h"
    out_cpp = "MakePacket.cpp"

    messages, enum_dict, struct_map, package_name = parse_proto(proto_file)

    header_content = generate_header(messages, package_name, struct_map)
    cpp_content = generate_cpp(messages, enum_dict, struct_map, package_name)

    Path(out_h).write_text(header_content, encoding='utf-8')
    Path(out_cpp).write_text(cpp_content, encoding='utf-8')

    print(f"Generated {out_h} and {out_cpp} from {proto_file}")


if __name__ == "__main__":
    main()
