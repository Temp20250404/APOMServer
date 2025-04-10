using Game;
using Google.Protobuf;
using ServerCore;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

class PacketHandler
{
    // SC_CHAT 패킷을 처리하는 함수
    public static void SC_Chat(PacketSession session, IMessage packet)
    {
        SC_CHAT chatPacket = packet as SC_CHAT;

        // TODO: SC_Chat 패킷 처리 로직을 여기에 구현
    }

    // SC_KEYINFO 패킷을 처리하는 함수
    public static void SC_Keyinfo(PacketSession session, IMessage packet)
    {
        SC_KEYINFO keyinfoPacket = packet as SC_KEYINFO;

        // TODO: SC_Keyinfo 패킷 처리 로직을 여기에 구현
    }

    // SC_LOGIN_RESPONSE 패킷을 처리하는 함수
    public static void SC_LoginResponse(PacketSession session, IMessage packet)
    {
        SC_LOGIN_RESPONSE loginResponsePacket = packet as SC_LOGIN_RESPONSE;

        // TODO: SC_LoginResponse 패킷 처리 로직을 여기에 구현
    }

    // SC_POSITION_SYNC 패킷을 처리하는 함수
    public static void SC_PositionSync(PacketSession session, IMessage packet)
    {
        SC_POSITION_SYNC positionSyncPacket = packet as SC_POSITION_SYNC;

        // TODO: SC_PositionSync 패킷 처리 로직을 여기에 구현
    }

    // SC_REGISTER_RESPONSE 패킷을 처리하는 함수
    public static void SC_RegisterResponse(PacketSession session, IMessage packet)
    {
        SC_REGISTER_RESPONSE registerResponsePacket = packet as SC_REGISTER_RESPONSE;

        // TODO: SC_RegisterResponse 패킷 처리 로직을 여기에 구현
    }

    // SC_REMOVE_CHARACTER 패킷을 처리하는 함수
    public static void SC_RemoveCharacter(PacketSession session, IMessage packet)
    {
        SC_REMOVE_CHARACTER removeCharacterPacket = packet as SC_REMOVE_CHARACTER;

        // TODO: SC_RemoveCharacter 패킷 처리 로직을 여기에 구현
    }

    // SC_SPAWN_CHARACTER 패킷을 처리하는 함수
    public static void SC_SpawnCharacter(PacketSession session, IMessage packet)
    {
        SC_SPAWN_CHARACTER spawnCharacterPacket = packet as SC_SPAWN_CHARACTER;

        // TODO: SC_SpawnCharacter 패킷 처리 로직을 여기에 구현
    }
}
