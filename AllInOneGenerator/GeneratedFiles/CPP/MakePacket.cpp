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

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_FIND_PW_RESPONSE_FOR_All(CSession* pSession, bool success, std::string pw)
{
    game::SC_FIND_PW_RESPONSE pkt;

    pkt.set_success(success);

    pkt.set_pw(pw);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_FindPwResponse;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_FIND_PW_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, std::string pw)
{
    game::SC_FIND_PW_RESPONSE pkt;

    pkt.set_success(success);

    pkt.set_pw(pw);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_FindPwResponse;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_FIND_PW_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, std::string pw)
{
    game::SC_FIND_PW_RESPONSE pkt;

    pkt.set_success(success);

    pkt.set_pw(pw);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_FindPwResponse;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

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

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_REQUEST_ITEM_INFO_FOR_All(CSession* pSession, std::vector<UINT32>& itemInfo)
{
    game::SC_REQUEST_ITEM_INFO pkt;

    auto* list = pkt.mutable_iteminfo();
    list->Reserve(static_cast<int>(itemInfo.size()));

    // 기본 타입: iterator 기반 복사
    list->Add(itemInfo.begin(), itemInfo.end());

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_RequestItemInfo;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_REQUEST_ITEM_INFO_FOR_SINGLE(CSession* pSession, std::vector<UINT32>& itemInfo)
{
    game::SC_REQUEST_ITEM_INFO pkt;

    auto* list = pkt.mutable_iteminfo();
    list->Reserve(static_cast<int>(itemInfo.size()));

    // 기본 타입: iterator 기반 복사
    list->Add(itemInfo.begin(), itemInfo.end());

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_RequestItemInfo;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_REQUEST_ITEM_INFO_FOR_AROUND(CSession* pSession, CRoom* pRoom, std::vector<UINT32>& itemInfo)
{
    game::SC_REQUEST_ITEM_INFO pkt;

    auto* list = pkt.mutable_iteminfo();
    list->Reserve(static_cast<int>(itemInfo.size()));

    // 기본 타입: iterator 기반 복사
    list->Add(itemInfo.begin(), itemInfo.end());

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_RequestItemInfo;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_RESPONSE_CHARACTER_INFO_FOR_All(CSession* pSession, PlayerInfo playerInfo)
{
    game::SC_RESPONSE_CHARACTER_INFO pkt;

    {  // 구조체 단일 필드 시작
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playerjob(playerInfo.playerJob);
        sub->set_level(playerInfo.level);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ResponseCharacterInfo;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_RESPONSE_CHARACTER_INFO_FOR_SINGLE(CSession* pSession, PlayerInfo playerInfo)
{
    game::SC_RESPONSE_CHARACTER_INFO pkt;

    {  // 구조체 단일 필드 시작
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playerjob(playerInfo.playerJob);
        sub->set_level(playerInfo.level);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ResponseCharacterInfo;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_RESPONSE_CHARACTER_INFO_FOR_AROUND(CSession* pSession, CRoom* pRoom, PlayerInfo playerInfo)
{
    game::SC_RESPONSE_CHARACTER_INFO pkt;

    {  // 구조체 단일 필드 시작
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playerjob(playerInfo.playerJob);
        sub->set_level(playerInfo.level);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ResponseCharacterInfo;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

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

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_ACCEPT_PARTY_FOR_All(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID, PlayerInfo playerInfo)
{
    game::SC_ACCEPT_PARTY pkt;

    pkt.set_fromplayerid(FromPlayerID);

    pkt.set_toplayerid(ToPlayerID);

    {  // 구조체 단일 필드 시작
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playerjob(playerInfo.playerJob);
        sub->set_level(playerInfo.level);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_AcceptParty;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ACCEPT_PARTY_FOR_SINGLE(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID, PlayerInfo playerInfo)
{
    game::SC_ACCEPT_PARTY pkt;

    pkt.set_fromplayerid(FromPlayerID);

    pkt.set_toplayerid(ToPlayerID);

    {  // 구조체 단일 필드 시작
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playerjob(playerInfo.playerJob);
        sub->set_level(playerInfo.level);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_AcceptParty;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ACCEPT_PARTY_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 FromPlayerID, UINT32 ToPlayerID, PlayerInfo playerInfo)
{
    game::SC_ACCEPT_PARTY pkt;

    pkt.set_fromplayerid(FromPlayerID);

    pkt.set_toplayerid(ToPlayerID);

    {  // 구조체 단일 필드 시작
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playerjob(playerInfo.playerJob);
        sub->set_level(playerInfo.level);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_AcceptParty;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_CANCEL_ENTERING_DUNGEON_FOR_All(CSession* pSession, bool bCancel, UINT32 playerID)
{
    game::SC_CANCEL_ENTERING_DUNGEON pkt;

    pkt.set_bcancel(bCancel);

    pkt.set_playerid(playerID);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CancelEnteringDungeon;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CANCEL_ENTERING_DUNGEON_FOR_SINGLE(CSession* pSession, bool bCancel, UINT32 playerID)
{
    game::SC_CANCEL_ENTERING_DUNGEON pkt;

    pkt.set_bcancel(bCancel);

    pkt.set_playerid(playerID);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CancelEnteringDungeon;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CANCEL_ENTERING_DUNGEON_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bCancel, UINT32 playerID)
{
    game::SC_CANCEL_ENTERING_DUNGEON pkt;

    pkt.set_bcancel(bCancel);

    pkt.set_playerid(playerID);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CancelEnteringDungeon;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_CREATE_MONSTER_FOR_All(CSession* pSession, UINT32 aiID, UINT32 monsterType, Position monsterPos)
{
    game::SC_CREATE_MONSTER pkt;

    pkt.set_aiid(aiID);

    pkt.set_monstertype(monsterType);

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_monsterpos();
        sub->set_posx(monsterPos.posX);
        sub->set_posy(monsterPos.posY);
        sub->set_posz(monsterPos.posZ);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateMonster;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_MONSTER_FOR_SINGLE(CSession* pSession, UINT32 aiID, UINT32 monsterType, Position monsterPos)
{
    game::SC_CREATE_MONSTER pkt;

    pkt.set_aiid(aiID);

    pkt.set_monstertype(monsterType);

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_monsterpos();
        sub->set_posx(monsterPos.posX);
        sub->set_posy(monsterPos.posY);
        sub->set_posz(monsterPos.posZ);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateMonster;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_MONSTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID, UINT32 monsterType, Position monsterPos)
{
    game::SC_CREATE_MONSTER pkt;

    pkt.set_aiid(aiID);

    pkt.set_monstertype(monsterType);

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_monsterpos();
        sub->set_posx(monsterPos.posX);
        sub->set_posy(monsterPos.posY);
        sub->set_posz(monsterPos.posZ);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateMonster;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_MAKE_PARTY_FOR_All(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID)
{
    game::SC_MAKE_PARTY pkt;

    pkt.set_fromplayerid(FromPlayerID);

    pkt.set_toplayerid(ToPlayerID);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MakeParty;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_MAKE_PARTY_FOR_SINGLE(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID)
{
    game::SC_MAKE_PARTY pkt;

    pkt.set_fromplayerid(FromPlayerID);

    pkt.set_toplayerid(ToPlayerID);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MakeParty;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_MAKE_PARTY_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 FromPlayerID, UINT32 ToPlayerID)
{
    game::SC_MAKE_PARTY pkt;

    pkt.set_fromplayerid(FromPlayerID);

    pkt.set_toplayerid(ToPlayerID);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MakeParty;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_PREPARE_ENTERING_DUNGEON_FOR_All(CSession* pSession, bool bEnter)
{
    game::SC_PREPARE_ENTERING_DUNGEON pkt;

    pkt.set_benter(bEnter);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PrepareEnteringDungeon;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_PREPARE_ENTERING_DUNGEON_FOR_SINGLE(CSession* pSession, bool bEnter)
{
    game::SC_PREPARE_ENTERING_DUNGEON pkt;

    pkt.set_benter(bEnter);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PrepareEnteringDungeon;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_PREPARE_ENTERING_DUNGEON_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bEnter)
{
    game::SC_PREPARE_ENTERING_DUNGEON pkt;

    pkt.set_benter(bEnter);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PrepareEnteringDungeon;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

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

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_RESPONSE_ENTER_DUNGEON_BEGIN_FOR_All(CSession* pSession, bool bEnterBegin)
{
    game::SC_RESPONSE_ENTER_DUNGEON_BEGIN pkt;

    pkt.set_benterbegin(bEnterBegin);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ResponseEnterDungeonBegin;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_RESPONSE_ENTER_DUNGEON_BEGIN_FOR_SINGLE(CSession* pSession, bool bEnterBegin)
{
    game::SC_RESPONSE_ENTER_DUNGEON_BEGIN pkt;

    pkt.set_benterbegin(bEnterBegin);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ResponseEnterDungeonBegin;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_RESPONSE_ENTER_DUNGEON_BEGIN_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bEnterBegin)
{
    game::SC_RESPONSE_ENTER_DUNGEON_BEGIN pkt;

    pkt.set_benterbegin(bEnterBegin);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ResponseEnterDungeonBegin;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_RESPONSE_ENTER_DUNGEON_END_FOR_All(CSession* pSession, bool bEnterEnd)
{
    game::SC_RESPONSE_ENTER_DUNGEON_END pkt;

    pkt.set_benterend(bEnterEnd);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ResponseEnterDungeonEnd;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_RESPONSE_ENTER_DUNGEON_END_FOR_SINGLE(CSession* pSession, bool bEnterEnd)
{
    game::SC_RESPONSE_ENTER_DUNGEON_END pkt;

    pkt.set_benterend(bEnterEnd);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ResponseEnterDungeonEnd;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_RESPONSE_ENTER_DUNGEON_END_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bEnterEnd)
{
    game::SC_RESPONSE_ENTER_DUNGEON_END pkt;

    pkt.set_benterend(bEnterEnd);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ResponseEnterDungeonEnd;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_SPAWN_CHARACTER_FOR_All(CSession* pSession, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo)
{
    game::SC_SPAWN_CHARACTER pkt;

    pkt.set_playerid(playerID);

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_playerpos();
        sub->set_posx(playerPos.posX);
        sub->set_posy(playerPos.posY);
        sub->set_posz(playerPos.posZ);
    }  // 구조체 단일 필드 끝

    pkt.set_camerayaw(cameraYaw);

    {  // 구조체 단일 필드 시작
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playerjob(playerInfo.playerJob);
        sub->set_level(playerInfo.level);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SpawnCharacter;

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

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_playerpos();
        sub->set_posx(playerPos.posX);
        sub->set_posy(playerPos.posY);
        sub->set_posz(playerPos.posZ);
    }  // 구조체 단일 필드 끝

    pkt.set_camerayaw(cameraYaw);

    {  // 구조체 단일 필드 시작
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playerjob(playerInfo.playerJob);
        sub->set_level(playerInfo.level);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SpawnCharacter;

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

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_playerpos();
        sub->set_posx(playerPos.posX);
        sub->set_posy(playerPos.posY);
        sub->set_posz(playerPos.posZ);
    }  // 구조체 단일 필드 끝

    pkt.set_camerayaw(cameraYaw);

    {  // 구조체 단일 필드 시작
        game::PlayerInfo* sub = pkt.mutable_playerinfo();
        sub->set_playernickname(playerInfo.playerNickname);
        sub->set_playerjob(playerInfo.playerJob);
        sub->set_level(playerInfo.level);
    }  // 구조체 단일 필드 끝

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SpawnCharacter;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_START_AI_CALCULATE_FOR_All(CSession* pSession, bool bStart)
{
    game::SC_START_AI_CALCULATE pkt;

    pkt.set_bstart(bStart);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_StartAiCalculate;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_START_AI_CALCULATE_FOR_SINGLE(CSession* pSession, bool bStart)
{
    game::SC_START_AI_CALCULATE pkt;

    pkt.set_bstart(bStart);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_StartAiCalculate;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_START_AI_CALCULATE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bStart)
{
    game::SC_START_AI_CALCULATE pkt;

    pkt.set_bstart(bStart);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_StartAiCalculate;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_STOP_AI_CALCULATE_FOR_All(CSession* pSession, bool bStop)
{
    game::SC_STOP_AI_CALCULATE pkt;

    pkt.set_bstop(bStop);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_StopAiCalculate;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_STOP_AI_CALCULATE_FOR_SINGLE(CSession* pSession, bool bStop)
{
    game::SC_STOP_AI_CALCULATE pkt;

    pkt.set_bstop(bStop);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_StopAiCalculate;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_STOP_AI_CALCULATE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bStop)
{
    game::SC_STOP_AI_CALCULATE pkt;

    pkt.set_bstop(bStop);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_StopAiCalculate;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

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

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

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

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_MONSTER_DAMAGED_FOR_All(CSession* pSession, UINT32 aiID, UINT32 damage)
{
    game::SC_MONSTER_DAMAGED pkt;

    pkt.set_aiid(aiID);

    pkt.set_damage(damage);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterDamaged;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_MONSTER_DAMAGED_FOR_SINGLE(CSession* pSession, UINT32 aiID, UINT32 damage)
{
    game::SC_MONSTER_DAMAGED pkt;

    pkt.set_aiid(aiID);

    pkt.set_damage(damage);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterDamaged;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_MONSTER_DAMAGED_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID, UINT32 damage)
{
    game::SC_MONSTER_DAMAGED pkt;

    pkt.set_aiid(aiID);

    pkt.set_damage(damage);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterDamaged;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_MONSTER_DIE_FOR_All(CSession* pSession, UINT32 aiID)
{
    game::SC_MONSTER_DIE pkt;

    pkt.set_aiid(aiID);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterDie;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_MONSTER_DIE_FOR_SINGLE(CSession* pSession, UINT32 aiID)
{
    game::SC_MONSTER_DIE pkt;

    pkt.set_aiid(aiID);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterDie;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_MONSTER_DIE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID)
{
    game::SC_MONSTER_DIE pkt;

    pkt.set_aiid(aiID);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterDie;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

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

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

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

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

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

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_MONSTER_AI_FOR_All(CSession* pSession, UINT32 aiID, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, Position bossPos, UINT32 bossState, float curSpeed)
{
    game::SC_MONSTER_AI pkt;

    pkt.set_aiid(aiID);

    pkt.set_currenthp(currentHp);

    pkt.set_maxhp(maxHp);

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_targetmovementpos();
        sub->set_posx(targetMovementPos.posX);
        sub->set_posy(targetMovementPos.posY);
        sub->set_posz(targetMovementPos.posZ);
    }  // 구조체 단일 필드 끝

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_bosspos();
        sub->set_posx(bossPos.posX);
        sub->set_posy(bossPos.posY);
        sub->set_posz(bossPos.posZ);
    }  // 구조체 단일 필드 끝

    pkt.set_bossstate(bossState);

    pkt.set_curspeed(curSpeed);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterAi;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_MONSTER_AI_FOR_SINGLE(CSession* pSession, UINT32 aiID, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, Position bossPos, UINT32 bossState, float curSpeed)
{
    game::SC_MONSTER_AI pkt;

    pkt.set_aiid(aiID);

    pkt.set_currenthp(currentHp);

    pkt.set_maxhp(maxHp);

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_targetmovementpos();
        sub->set_posx(targetMovementPos.posX);
        sub->set_posy(targetMovementPos.posY);
        sub->set_posz(targetMovementPos.posZ);
    }  // 구조체 단일 필드 끝

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_bosspos();
        sub->set_posx(bossPos.posX);
        sub->set_posy(bossPos.posY);
        sub->set_posz(bossPos.posZ);
    }  // 구조체 단일 필드 끝

    pkt.set_bossstate(bossState);

    pkt.set_curspeed(curSpeed);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterAi;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_MONSTER_AI_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, Position bossPos, UINT32 bossState, float curSpeed)
{
    game::SC_MONSTER_AI pkt;

    pkt.set_aiid(aiID);

    pkt.set_currenthp(currentHp);

    pkt.set_maxhp(maxHp);

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_targetmovementpos();
        sub->set_posx(targetMovementPos.posX);
        sub->set_posy(targetMovementPos.posY);
        sub->set_posz(targetMovementPos.posZ);
    }  // 구조체 단일 필드 끝

    {  // 구조체 단일 필드 시작
        game::Position* sub = pkt.mutable_bosspos();
        sub->set_posx(bossPos.posX);
        sub->set_posy(bossPos.posY);
        sub->set_posz(bossPos.posZ);
    }  // 구조체 단일 필드 끝

    pkt.set_bossstate(bossState);

    pkt.set_curspeed(curSpeed);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterAi;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_MONSTER_ROTATE_FOR_All(CSession* pSession, UINT32 aiID, float rotateY)
{
    game::SC_MONSTER_ROTATE pkt;

    pkt.set_aiid(aiID);

    pkt.set_rotatey(rotateY);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterRotate;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_MONSTER_ROTATE_FOR_SINGLE(CSession* pSession, UINT32 aiID, float rotateY)
{
    game::SC_MONSTER_ROTATE pkt;

    pkt.set_aiid(aiID);

    pkt.set_rotatey(rotateY);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterRotate;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_MONSTER_ROTATE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID, float rotateY)
{
    game::SC_MONSTER_ROTATE pkt;

    pkt.set_aiid(aiID);

    pkt.set_rotatey(rotateY);

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_MonsterRotate;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}


void SC_TEST_PACKET_FOR_All(CSession* pSession, std::vector<PlayerInfo>& tempData)
{
    game::SC_TEST_PACKET pkt;

    auto* list = pkt.mutable_tempdata();
    list->Reserve(static_cast<int>(tempData.size()));

    // 메시지 구조체: 각 요소 필드별 setter 호출
    for (const auto& elem : tempData) {
        auto* msg = pkt.add_tempdata();
        msg->set_playernickname(elem.playerNickname);
        msg->set_playerjob(elem.playerJob);
        msg->set_level(elem.level);
    }

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_TestPacket;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_TEST_PACKET_FOR_SINGLE(CSession* pSession, std::vector<PlayerInfo>& tempData)
{
    game::SC_TEST_PACKET pkt;

    auto* list = pkt.mutable_tempdata();
    list->Reserve(static_cast<int>(tempData.size()));

    // 메시지 구조체: 각 요소 필드별 setter 호출
    for (const auto& elem : tempData) {
        auto* msg = pkt.add_tempdata();
        msg->set_playernickname(elem.playerNickname);
        msg->set_playerjob(elem.playerJob);
        msg->set_level(elem.level);
    }

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_TestPacket;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_TEST_PACKET_FOR_AROUND(CSession* pSession, CRoom* pRoom, std::vector<PlayerInfo>& tempData)
{
    game::SC_TEST_PACKET pkt;

    auto* list = pkt.mutable_tempdata();
    list->Reserve(static_cast<int>(tempData.size()));

    // 메시지 구조체: 각 요소 필드별 setter 호출
    for (const auto& elem : tempData) {
        auto* msg = pkt.add_tempdata();
        msg->set_playernickname(elem.playerNickname);
        msg->set_playerjob(elem.playerJob);
        msg->set_level(elem.level);
    }

    int pktSize = pkt.ByteSizeLong();
    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_TestPacket;

    CPacket* Packet = packetPool.Alloc();
    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);
    for(auto& pl: pRoom->m_activePlayers){ if(pl->m_pSession != pSession) UnicastPacket(pl->m_pSession, &header, Packet); }

    Packet->Clear();
    packetPool.Free(Packet);
}

