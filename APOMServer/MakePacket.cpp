#include "pch.h"
#include "MakePacket.h"
#include "SessionManager.h"
#include "Packet.h"
#include "Player.h"
#include "Room.h"
#include "MemoryPoolManager.h"
#include "Protobuf/Protocol.pb.h"

void SC_FIND_ID_RESPONSE_FOR_All(CSession* pSession, bool success, std::string id)
{
    game::SC_FIND_ID_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_id(id);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_FindIdResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_FIND_ID_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, std::string id)
{
    game::SC_FIND_ID_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_id(id);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_FindIdResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_FIND_ID_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, std::string id)
{
    game::SC_FIND_ID_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_id(id);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_FindIdResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_FIND_PW_RESPONSE_FOR_All(CSession* pSession, bool success, std::string id)
{
    game::SC_FIND_PW_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_id(id);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_FindPwResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_FIND_PW_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, std::string id)
{
    game::SC_FIND_PW_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_id(id);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_FindPwResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_FIND_PW_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, std::string id)
{
    game::SC_FIND_PW_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_id(id);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_FindPwResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_LOGIN_RESPONSE_FOR_All(CSession* pSession, bool success, UINT32 errorCode)
{
    game::SC_LOGIN_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_errorcode(errorCode);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_LoginResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_LOGIN_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, UINT32 errorCode)
{
    game::SC_LOGIN_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_errorcode(errorCode);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_LoginResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_LOGIN_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, UINT32 errorCode)
{
    game::SC_LOGIN_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_errorcode(errorCode);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_LoginResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SIGNUP_RESPONSE_FOR_All(CSession* pSession, bool success, UINT32 errorCode)
{
    game::SC_SIGNUP_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_errorcode(errorCode);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SignupResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SIGNUP_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, UINT32 errorCode)
{
    game::SC_SIGNUP_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_errorcode(errorCode);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SignupResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SIGNUP_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, UINT32 errorCode)
{
    game::SC_SIGNUP_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_errorcode(errorCode);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SignupResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_TRANSFER_CHARACTER_INFO_FOR_All(CSession* pSession, PlayerInfo playerInfo)
{
    game::SC_TRANSFER_CHARACTER_INFO pkt;

    {
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playermaxhp(playerInfo.playerMaxHp);
        sub->set_playermaxmp(playerInfo.playerMaxMp);
        sub->set_playerjobicon(playerInfo.playerJobIcon);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_TransferCharacterInfo;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_TRANSFER_CHARACTER_INFO_FOR_SINGLE(CSession* pSession, PlayerInfo playerInfo)
{
    game::SC_TRANSFER_CHARACTER_INFO pkt;

    {
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playermaxhp(playerInfo.playerMaxHp);
        sub->set_playermaxmp(playerInfo.playerMaxMp);
        sub->set_playerjobicon(playerInfo.playerJobIcon);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_TransferCharacterInfo;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_TRANSFER_CHARACTER_INFO_FOR_AROUND(CSession* pSession, CRoom* pRoom, PlayerInfo playerInfo)
{
    game::SC_TRANSFER_CHARACTER_INFO pkt;

    {
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playermaxhp(playerInfo.playerMaxHp);
        sub->set_playermaxmp(playerInfo.playerMaxMp);
        sub->set_playerjobicon(playerInfo.playerJobIcon);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_TransferCharacterInfo;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_MONSTER_FOR_All(CSession* pSession, UINT32 aiID, UINT32 monsterTypeID, Position monsterPos)
{
    game::SC_CREATE_MONSTER pkt;

    pkt.set_aiid(aiID);
    pkt.set_monstertypeid(monsterTypeID);
    {
        game::Position* sub = pkt.mutable_monsterpos();
        sub->set_posx(monsterPos.posX);
        sub->set_posy(monsterPos.posY);
        sub->set_posz(monsterPos.posZ);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateMonster;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_MONSTER_FOR_SINGLE(CSession* pSession, UINT32 aiID, UINT32 monsterTypeID, Position monsterPos)
{
    game::SC_CREATE_MONSTER pkt;

    pkt.set_aiid(aiID);
    pkt.set_monstertypeid(monsterTypeID);
    {
        game::Position* sub = pkt.mutable_monsterpos();
        sub->set_posx(monsterPos.posX);
        sub->set_posy(monsterPos.posY);
        sub->set_posz(monsterPos.posZ);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateMonster;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_MONSTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID, UINT32 monsterTypeID, Position monsterPos)
{
    game::SC_CREATE_MONSTER pkt;

    pkt.set_aiid(aiID);
    pkt.set_monstertypeid(monsterTypeID);
    {
        game::Position* sub = pkt.mutable_monsterpos();
        sub->set_posx(monsterPos.posX);
        sub->set_posy(monsterPos.posY);
        sub->set_posz(monsterPos.posZ);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateMonster;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_REMOVE_CHARACTER_FOR_All(CSession* pSession, UINT32 playerID)
{
    game::SC_REMOVE_CHARACTER pkt;

    pkt.set_playerid(playerID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_RemoveCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_REMOVE_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerID)
{
    game::SC_REMOVE_CHARACTER pkt;

    pkt.set_playerid(playerID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_RemoveCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_REMOVE_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID)
{
    game::SC_REMOVE_CHARACTER pkt;

    pkt.set_playerid(playerID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_RemoveCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SPAWN_CHARACTER_FOR_All(CSession* pSession, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo)
{
    game::SC_SPAWN_CHARACTER pkt;

    pkt.set_playerid(playerID);
    {
        game::Position* sub = pkt.mutable_playerpos();
        sub->set_posx(playerPos.posX);
        sub->set_posy(playerPos.posY);
        sub->set_posz(playerPos.posZ);
    }
    pkt.set_camerayaw(cameraYaw);
    {
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playermaxhp(playerInfo.playerMaxHp);
        sub->set_playermaxmp(playerInfo.playerMaxMp);
        sub->set_playerjobicon(playerInfo.playerJobIcon);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SpawnCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SPAWN_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo)
{
    game::SC_SPAWN_CHARACTER pkt;

    pkt.set_playerid(playerID);
    {
        game::Position* sub = pkt.mutable_playerpos();
        sub->set_posx(playerPos.posX);
        sub->set_posy(playerPos.posY);
        sub->set_posz(playerPos.posZ);
    }
    pkt.set_camerayaw(cameraYaw);
    {
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playermaxhp(playerInfo.playerMaxHp);
        sub->set_playermaxmp(playerInfo.playerMaxMp);
        sub->set_playerjobicon(playerInfo.playerJobIcon);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SpawnCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SPAWN_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo)
{
    game::SC_SPAWN_CHARACTER pkt;

    pkt.set_playerid(playerID);
    {
        game::Position* sub = pkt.mutable_playerpos();
        sub->set_posx(playerPos.posX);
        sub->set_posy(playerPos.posY);
        sub->set_posz(playerPos.posZ);
    }
    pkt.set_camerayaw(cameraYaw);
    {
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playermaxhp(playerInfo.playerMaxHp);
        sub->set_playermaxmp(playerInfo.playerMaxMp);
        sub->set_playerjobicon(playerInfo.playerJobIcon);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SpawnCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHAT_FOR_All(CSession* pSession, UINT32 playerID, std::string message, UINT32 channel)
{
    game::SC_CHAT pkt;

    pkt.set_playerid(playerID);
    pkt.set_message(message);
    pkt.set_channel(channel);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Chat;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHAT_FOR_SINGLE(CSession* pSession, UINT32 playerID, std::string message, UINT32 channel)
{
    game::SC_CHAT pkt;

    pkt.set_playerid(playerID);
    pkt.set_message(message);
    pkt.set_channel(channel);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Chat;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHAT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, std::string message, UINT32 channel)
{
    game::SC_CHAT pkt;

    pkt.set_playerid(playerID);
    pkt.set_message(message);
    pkt.set_channel(channel);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Chat;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_KEYINFO_FOR_All(CSession* pSession, UINT32 playerID, UINT32 keyInfo, float cameraYaw)
{
    game::SC_KEYINFO pkt;

    pkt.set_playerid(playerID);
    pkt.set_keyinfo(keyInfo);
    pkt.set_camerayaw(cameraYaw);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Keyinfo;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_KEYINFO_FOR_SINGLE(CSession* pSession, UINT32 playerID, UINT32 keyInfo, float cameraYaw)
{
    game::SC_KEYINFO pkt;

    pkt.set_playerid(playerID);
    pkt.set_keyinfo(keyInfo);
    pkt.set_camerayaw(cameraYaw);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Keyinfo;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_KEYINFO_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, UINT32 keyInfo, float cameraYaw)
{
    game::SC_KEYINFO pkt;

    pkt.set_playerid(playerID);
    pkt.set_keyinfo(keyInfo);
    pkt.set_camerayaw(cameraYaw);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Keyinfo;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_PLAYER_ATTACK_FOR_All(CSession* pSession, UINT32 playerID, UINT32 attackType, UINT32 aiID, UINT32 damage, UINT32 targetMaxHP, UINT32 targetCurHP)
{
    game::SC_PLAYER_ATTACK pkt;

    pkt.set_playerid(playerID);
    pkt.set_attacktype(attackType);
    pkt.set_aiid(aiID);
    pkt.set_damage(damage);
    pkt.set_targetmaxhp(targetMaxHP);
    pkt.set_targetcurhp(targetCurHP);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PlayerAttack;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_PLAYER_ATTACK_FOR_SINGLE(CSession* pSession, UINT32 playerID, UINT32 attackType, UINT32 aiID, UINT32 damage, UINT32 targetMaxHP, UINT32 targetCurHP)
{
    game::SC_PLAYER_ATTACK pkt;

    pkt.set_playerid(playerID);
    pkt.set_attacktype(attackType);
    pkt.set_aiid(aiID);
    pkt.set_damage(damage);
    pkt.set_targetmaxhp(targetMaxHP);
    pkt.set_targetcurhp(targetCurHP);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PlayerAttack;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_PLAYER_ATTACK_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, UINT32 attackType, UINT32 aiID, UINT32 damage, UINT32 targetMaxHP, UINT32 targetCurHP)
{
    game::SC_PLAYER_ATTACK pkt;

    pkt.set_playerid(playerID);
    pkt.set_attacktype(attackType);
    pkt.set_aiid(aiID);
    pkt.set_damage(damage);
    pkt.set_targetmaxhp(targetMaxHP);
    pkt.set_targetcurhp(targetCurHP);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PlayerAttack;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_PLAYER_DAMAGED_FOR_All(CSession* pSession, UINT32 playerID, UINT32 damage)
{
    game::SC_PLAYER_DAMAGED pkt;

    pkt.set_playerid(playerID);
    pkt.set_damage(damage);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PlayerDamaged;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_PLAYER_DAMAGED_FOR_SINGLE(CSession* pSession, UINT32 playerID, UINT32 damage)
{
    game::SC_PLAYER_DAMAGED pkt;

    pkt.set_playerid(playerID);
    pkt.set_damage(damage);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PlayerDamaged;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_PLAYER_DAMAGED_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, UINT32 damage)
{
    game::SC_PLAYER_DAMAGED pkt;

    pkt.set_playerid(playerID);
    pkt.set_damage(damage);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PlayerDamaged;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_PLAYER_DIE_FOR_All(CSession* pSession, UINT32 playerID)
{
    game::SC_PLAYER_DIE pkt;

    pkt.set_playerid(playerID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PlayerDie;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_PLAYER_DIE_FOR_SINGLE(CSession* pSession, UINT32 playerID)
{
    game::SC_PLAYER_DIE pkt;

    pkt.set_playerid(playerID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PlayerDie;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_PLAYER_DIE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID)
{
    game::SC_PLAYER_DIE pkt;

    pkt.set_playerid(playerID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PlayerDie;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_POSITION_SYNC_FOR_All(CSession* pSession, UINT32 playerID, float posX, float posY, float cameraYaw)
{
    game::SC_POSITION_SYNC pkt;

    pkt.set_playerid(playerID);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_camerayaw(cameraYaw);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PositionSync;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_POSITION_SYNC_FOR_SINGLE(CSession* pSession, UINT32 playerID, float posX, float posY, float cameraYaw)
{
    game::SC_POSITION_SYNC pkt;

    pkt.set_playerid(playerID);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_camerayaw(cameraYaw);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PositionSync;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_POSITION_SYNC_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, float posX, float posY, float cameraYaw)
{
    game::SC_POSITION_SYNC pkt;

    pkt.set_playerid(playerID);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_camerayaw(cameraYaw);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PositionSync;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_BOSS_PHASE_FOR_All(CSession* pSession, UINT32 bossID, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, Position bossPos, game::BOSS_STATE bossState, float curSpeed)
{
    game::SC_BOSS_PHASE pkt;

    pkt.set_bossid(bossID);
    pkt.set_currenthp(currentHp);
    pkt.set_maxhp(maxHp);
    {
        game::Position* sub = pkt.mutable_targetmovementpos();
        sub->set_posx(targetMovementPos.posX);
        sub->set_posy(targetMovementPos.posY);
        sub->set_posz(targetMovementPos.posZ);
    }
    {
        game::Position* sub = pkt.mutable_bosspos();
        sub->set_posx(bossPos.posX);
        sub->set_posy(bossPos.posY);
        sub->set_posz(bossPos.posZ);
    }
    pkt.set_bossstate(bossState);
    pkt.set_curspeed(curSpeed);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_BossPhase;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_BOSS_PHASE_FOR_SINGLE(CSession* pSession, UINT32 bossID, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, Position bossPos, game::BOSS_STATE bossState, float curSpeed)
{
    game::SC_BOSS_PHASE pkt;

    pkt.set_bossid(bossID);
    pkt.set_currenthp(currentHp);
    pkt.set_maxhp(maxHp);
    {
        game::Position* sub = pkt.mutable_targetmovementpos();
        sub->set_posx(targetMovementPos.posX);
        sub->set_posy(targetMovementPos.posY);
        sub->set_posz(targetMovementPos.posZ);
    }
    {
        game::Position* sub = pkt.mutable_bosspos();
        sub->set_posx(bossPos.posX);
        sub->set_posy(bossPos.posY);
        sub->set_posz(bossPos.posZ);
    }
    pkt.set_bossstate(bossState);
    pkt.set_curspeed(curSpeed);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_BossPhase;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_BOSS_PHASE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 bossID, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, Position bossPos, game::BOSS_STATE bossState, float curSpeed)
{
    std::cout << "=== SC_BOSS_PHASE_FOR_AROUND ===" << std::endl;
    std::cout << "Boss ID: " << bossID << std::endl;
    std::cout << "Current HP: " << currentHp << std::endl;
    std::cout << "Max HP: " << maxHp << std::endl;
    std::cout << "Target Movement Pos: (" << targetMovementPos.posX << ", " << targetMovementPos.posY << ", " << targetMovementPos.posZ << ")" << std::endl;
    std::cout << "Boss Position: (" << bossPos.posX << ", " << bossPos.posY<< ", " << bossPos.posZ << ")" << std::endl;
    std::cout << "Boss State: ";
    
    switch (bossState)
    {
    case game::BOSS_STATE_ATTACK:
        std::cout << "ATTACK";
        break;
    case game::BOSS_STATE_CHASE:
        std::cout << "CHASE";
        break;
    case game::BOSS_STATE_DIE:
        std::cout << "DIE";
        break;
    case game::BOSS_STATE_IDLE:
        std::cout << "IDLE";
        break;
    case game::BOSS_STATE_SKILL1:
        std::cout << "SKILL1";
        break;
    case game::BOSS_STATE_SKILL2:
        std::cout << "SKILL2";
        break;
    case game::BOSS_STATE_SKILL3:
        std::cout << "SKILL3";
        break;
    case game::BOSS_STATE_WALK:
        std::cout << "WALK";
        break;
    default:
        break;
    }
    std::cout << "\n";

    std::cout << "Current Speed: " << curSpeed << std::endl;

    game::SC_BOSS_PHASE pkt;

    pkt.set_bossid(bossID);
    pkt.set_currenthp(currentHp);
    pkt.set_maxhp(maxHp);
    {
        game::Position* sub = pkt.mutable_targetmovementpos();
        sub->set_posx(targetMovementPos.posX);
        sub->set_posy(targetMovementPos.posY);
        sub->set_posz(targetMovementPos.posZ);
    }
    {
        game::Position* sub = pkt.mutable_bosspos();
        sub->set_posx(bossPos.posX);
        sub->set_posy(bossPos.posY);
        sub->set_posz(bossPos.posZ);
    }
    pkt.set_bossstate(bossState);
    pkt.set_curspeed(curSpeed);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_BossPhase;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}
