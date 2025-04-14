#include "pch.h"
#include "MakePacket.h"
#include "SessionManager.h"
#include "Packet.h"
#include "Player.h"
#include "Room.h"
#include "MemoryPoolManager.h"
#include "Protobuf/Protocol.pb.h"

void SC_LOGIN_RESPONSE_FOR_All(CSession* pSession, bool success, UINT32 playerID, std::string characterName)
{
    game::SC_LOGIN_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_playerid(playerID);
    pkt.set_charactername(characterName);

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

void SC_LOGIN_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, UINT32 playerID, std::string characterName)
{
    game::SC_LOGIN_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_playerid(playerID);
    pkt.set_charactername(characterName);

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

void SC_LOGIN_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, UINT32 playerID, std::string characterName)
{
    game::SC_LOGIN_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_playerid(playerID);
    pkt.set_charactername(characterName);

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

void SC_REGISTER_RESPONSE_FOR_All(CSession* pSession, bool success, std::string message)
{
    game::SC_REGISTER_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_message(message);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_RegisterResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_REGISTER_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, std::string message)
{
    game::SC_REGISTER_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_message(message);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_RegisterResponse;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_REGISTER_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, std::string message)
{
    game::SC_REGISTER_RESPONSE pkt;

    pkt.set_success(success);
    pkt.set_message(message);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_RegisterResponse;

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

void SC_SPAWN_CHARACTER_FOR_All(CSession* pSession, UINT32 playerID, float posX, float posY, float cameraYaw, PlayerInfo playerInfo)
{
    game::SC_SPAWN_CHARACTER pkt;

    pkt.set_playerid(playerID);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
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

void SC_SPAWN_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerID, float posX, float posY, float cameraYaw, PlayerInfo playerInfo)
{
    game::SC_SPAWN_CHARACTER pkt;

    pkt.set_playerid(playerID);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
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

void SC_SPAWN_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, float posX, float posY, float cameraYaw, PlayerInfo playerInfo)
{
    game::SC_SPAWN_CHARACTER pkt;

    pkt.set_playerid(playerID);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
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

void SC_BOSS_PHASE_FOR_All(CSession* pSession, UINT32 bossID, BOSS_PHASE currentPhase, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, UINT32 targetPlayerID, float distanceToPlayer)
{
    game::SC_BOSS_PHASE pkt;

    pkt.set_bossid(bossID);
    pkt.set_currentphase(currentPhase);
    pkt.set_currenthp(currentHp);
    pkt.set_maxhp(maxHp);
    {
        game::Position* sub = pkt.mutable_targetmovementpos();
        sub->set_posx(targetMovementPos.posX);
        sub->set_posy(targetMovementPos.posY);
        sub->set_posz(targetMovementPos.posZ);
    }
    pkt.set_targetplayerid(targetPlayerID);
    pkt.set_distancetoplayer(distanceToPlayer);

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

void SC_BOSS_PHASE_FOR_SINGLE(CSession* pSession, UINT32 bossID, BOSS_PHASE currentPhase, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, UINT32 targetPlayerID, float distanceToPlayer)
{
    game::SC_BOSS_PHASE pkt;

    pkt.set_bossid(bossID);
    pkt.set_currentphase(currentPhase);
    pkt.set_currenthp(currentHp);
    pkt.set_maxhp(maxHp);
    {
        game::Position* sub = pkt.mutable_targetmovementpos();
        sub->set_posx(targetMovementPos.posX);
        sub->set_posy(targetMovementPos.posY);
        sub->set_posz(targetMovementPos.posZ);
    }
    pkt.set_targetplayerid(targetPlayerID);
    pkt.set_distancetoplayer(distanceToPlayer);

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

void SC_BOSS_PHASE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 bossID, BOSS_PHASE currentPhase, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, UINT32 targetPlayerID, float distanceToPlayer)
{
    game::SC_BOSS_PHASE pkt;

    pkt.set_bossid(bossID);
    pkt.set_currentphase(currentPhase);
    pkt.set_currenthp(currentHp);
    pkt.set_maxhp(maxHp);
    {
        game::Position* sub = pkt.mutable_targetmovementpos();
        sub->set_posx(targetMovementPos.posX);
        sub->set_posy(targetMovementPos.posY);
        sub->set_posz(targetMovementPos.posZ);
    }
    pkt.set_targetplayerid(targetPlayerID);
    pkt.set_distancetoplayer(distanceToPlayer);

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
