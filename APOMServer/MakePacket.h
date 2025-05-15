#pragma once

#include "ProtoStruct.h"
#include "Protobuf/Protocol.pb.h"

class CSession;
class CRoom;


void SC_FIND_ID_RESPONSE_FOR_All(CSession* pSession, bool success, std::string id);
void SC_FIND_ID_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, std::string id);
void SC_FIND_ID_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, std::string id);

void SC_FIND_PW_RESPONSE_FOR_All(CSession* pSession, bool success, std::string pw);
void SC_FIND_PW_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, std::string pw);
void SC_FIND_PW_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, std::string pw);

void SC_LOGIN_RESPONSE_FOR_All(CSession* pSession, bool success, UINT32 errorCode);
void SC_LOGIN_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, UINT32 errorCode);
void SC_LOGIN_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, UINT32 errorCode);

void SC_REQUEST_ITEM_INFO_FOR_All(CSession* pSession, std::vector<UINT32>& itemInfo);
void SC_REQUEST_ITEM_INFO_FOR_SINGLE(CSession* pSession, std::vector<UINT32>& itemInfo);
void SC_REQUEST_ITEM_INFO_FOR_AROUND(CSession* pSession, CRoom* pRoom, std::vector<UINT32>& itemInfo);

void SC_RESPONSE_CHARACTER_INFO_FOR_All(CSession* pSession, PlayerInfo playerInfo);
void SC_RESPONSE_CHARACTER_INFO_FOR_SINGLE(CSession* pSession, PlayerInfo playerInfo);
void SC_RESPONSE_CHARACTER_INFO_FOR_AROUND(CSession* pSession, CRoom* pRoom, PlayerInfo playerInfo);

void SC_SIGNUP_RESPONSE_FOR_All(CSession* pSession, bool success, UINT32 errorCode);
void SC_SIGNUP_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, UINT32 errorCode);
void SC_SIGNUP_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, UINT32 errorCode);

void SC_ACCEPT_PARTY_FOR_All(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID, PlayerInfo playerInfo);
void SC_ACCEPT_PARTY_FOR_SINGLE(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID, PlayerInfo playerInfo);
void SC_ACCEPT_PARTY_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 FromPlayerID, UINT32 ToPlayerID, PlayerInfo playerInfo);

void SC_CANCEL_ENTERING_DUNGEON_FOR_All(CSession* pSession, bool bCancel, UINT32 playerID);
void SC_CANCEL_ENTERING_DUNGEON_FOR_SINGLE(CSession* pSession, bool bCancel, UINT32 playerID);
void SC_CANCEL_ENTERING_DUNGEON_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bCancel, UINT32 playerID);

void SC_CREATE_MONSTER_FOR_All(CSession* pSession, UINT32 aiID, UINT32 monsterType, Position monsterPos);
void SC_CREATE_MONSTER_FOR_SINGLE(CSession* pSession, UINT32 aiID, UINT32 monsterType, Position monsterPos);
void SC_CREATE_MONSTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID, UINT32 monsterType, Position monsterPos);

void SC_MAKE_PARTY_FOR_All(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID);
void SC_MAKE_PARTY_FOR_SINGLE(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID);
void SC_MAKE_PARTY_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 FromPlayerID, UINT32 ToPlayerID);

void SC_PREPARE_ENTERING_DUNGEON_FOR_All(CSession* pSession, bool bEnter);
void SC_PREPARE_ENTERING_DUNGEON_FOR_SINGLE(CSession* pSession, bool bEnter);
void SC_PREPARE_ENTERING_DUNGEON_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bEnter);

void SC_REMOVE_CHARACTER_FOR_All(CSession* pSession, UINT32 playerID);
void SC_REMOVE_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerID);
void SC_REMOVE_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID);

void SC_RESPONSE_ENTER_DUNGEON_BEGIN_FOR_All(CSession* pSession, bool bEnterBegin);
void SC_RESPONSE_ENTER_DUNGEON_BEGIN_FOR_SINGLE(CSession* pSession, bool bEnterBegin);
void SC_RESPONSE_ENTER_DUNGEON_BEGIN_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bEnterBegin);

void SC_RESPONSE_ENTER_DUNGEON_END_FOR_All(CSession* pSession, bool bEnterEnd);
void SC_RESPONSE_ENTER_DUNGEON_END_FOR_SINGLE(CSession* pSession, bool bEnterEnd);
void SC_RESPONSE_ENTER_DUNGEON_END_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bEnterEnd);

void SC_SPAWN_CHARACTER_FOR_All(CSession* pSession, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo);
void SC_SPAWN_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo);
void SC_SPAWN_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo);

void SC_START_AI_CALCULATE_FOR_All(CSession* pSession, bool bStart);
void SC_START_AI_CALCULATE_FOR_SINGLE(CSession* pSession, bool bStart);
void SC_START_AI_CALCULATE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bStart);

void SC_STOP_AI_CALCULATE_FOR_All(CSession* pSession, bool bStop);
void SC_STOP_AI_CALCULATE_FOR_SINGLE(CSession* pSession, bool bStop);
void SC_STOP_AI_CALCULATE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool bStop);

void SC_CHAT_FOR_All(CSession* pSession, UINT32 playerID, std::string message, UINT32 channel);
void SC_CHAT_FOR_SINGLE(CSession* pSession, UINT32 playerID, std::string message, UINT32 channel);
void SC_CHAT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, std::string message, UINT32 channel);

void SC_KEYINFO_FOR_All(CSession* pSession, UINT32 playerID, UINT32 keyInfo, float cameraYaw);
void SC_KEYINFO_FOR_SINGLE(CSession* pSession, UINT32 playerID, UINT32 keyInfo, float cameraYaw);
void SC_KEYINFO_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, UINT32 keyInfo, float cameraYaw);

void SC_MONSTER_DAMAGED_FOR_All(CSession* pSession, UINT32 aiID, UINT32 damage);
void SC_MONSTER_DAMAGED_FOR_SINGLE(CSession* pSession, UINT32 aiID, UINT32 damage);
void SC_MONSTER_DAMAGED_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID, UINT32 damage);

void SC_MONSTER_DIE_FOR_All(CSession* pSession, UINT32 aiID);
void SC_MONSTER_DIE_FOR_SINGLE(CSession* pSession, UINT32 aiID);
void SC_MONSTER_DIE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID);

void SC_PLAYER_DAMAGED_FOR_All(CSession* pSession, UINT32 playerID, UINT32 damage);
void SC_PLAYER_DAMAGED_FOR_SINGLE(CSession* pSession, UINT32 playerID, UINT32 damage);
void SC_PLAYER_DAMAGED_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, UINT32 damage);

void SC_PLAYER_DIE_FOR_All(CSession* pSession, UINT32 playerID);
void SC_PLAYER_DIE_FOR_SINGLE(CSession* pSession, UINT32 playerID);
void SC_PLAYER_DIE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID);

void SC_POSITION_SYNC_FOR_All(CSession* pSession, UINT32 playerID, float posX, float posY, float posZ, float cameraYaw);
void SC_POSITION_SYNC_FOR_SINGLE(CSession* pSession, UINT32 playerID, float posX, float posY, float posZ, float cameraYaw);
void SC_POSITION_SYNC_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, float posX, float posY, float posZ, float cameraYaw);

void SC_MONSTER_AI_FOR_All(CSession* pSession, UINT32 aiID, Position targetMovementPos, Position bossPos, UINT32 bossState, float curSpeed);
void SC_MONSTER_AI_FOR_SINGLE(CSession* pSession, UINT32 aiID, Position targetMovementPos, Position bossPos, UINT32 bossState, float curSpeed);
void SC_MONSTER_AI_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID, Position targetMovementPos, Position bossPos, UINT32 bossState, float curSpeed);

void SC_MONSTER_CONDITION_FOR_All(CSession* pSession, UINT32 aiID, UINT32 currentHp, UINT32 maxHp);
void SC_MONSTER_CONDITION_FOR_SINGLE(CSession* pSession, UINT32 aiID, UINT32 currentHp, UINT32 maxHp);
void SC_MONSTER_CONDITION_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID, UINT32 currentHp, UINT32 maxHp);

void SC_MONSTER_ROTATE_FOR_All(CSession* pSession, UINT32 aiID, float rotateY);
void SC_MONSTER_ROTATE_FOR_SINGLE(CSession* pSession, UINT32 aiID, float rotateY);
void SC_MONSTER_ROTATE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID, float rotateY);

void SC_TEST_PACKET_FOR_All(CSession* pSession, std::vector<PlayerInfo>& tempData);
void SC_TEST_PACKET_FOR_SINGLE(CSession* pSession, std::vector<PlayerInfo>& tempData);
void SC_TEST_PACKET_FOR_AROUND(CSession* pSession, CRoom* pRoom, std::vector<PlayerInfo>& tempData);
