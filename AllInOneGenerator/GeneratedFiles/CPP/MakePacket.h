#pragma once

#include "ProtoStruct.h"
#include "Protobuf/Protocol.pb.h"

class CSession;
class CRoom;

void SC_FIND_ID_RESPONSE_FOR_All(CSession* pSession, bool success, std::string id);
void SC_FIND_ID_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, std::string id);
void SC_FIND_ID_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, std::string id);

void SC_FIND_PW_RESPONSE_FOR_All(CSession* pSession, bool success, std::string id);
void SC_FIND_PW_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, std::string id);
void SC_FIND_PW_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, std::string id);

void SC_LOGIN_RESPONSE_FOR_All(CSession* pSession, bool success, UINT32 errorCode);
void SC_LOGIN_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, UINT32 errorCode);
void SC_LOGIN_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, UINT32 errorCode);

void SC_SIGNUP_RESPONSE_FOR_All(CSession* pSession, bool success, UINT32 errorCode);
void SC_SIGNUP_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, UINT32 errorCode);
void SC_SIGNUP_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, UINT32 errorCode);

void SC_TRANSFER_CHARACTER_INFO_FOR_All(CSession* pSession, PlayerInfo playerInfo);
void SC_TRANSFER_CHARACTER_INFO_FOR_SINGLE(CSession* pSession, PlayerInfo playerInfo);
void SC_TRANSFER_CHARACTER_INFO_FOR_AROUND(CSession* pSession, CRoom* pRoom, PlayerInfo playerInfo);

void SC_CREATE_MONSTER_FOR_All(CSession* pSession, UINT32 aiID, UINT32 monsterTypeID, Position monsterPos);
void SC_CREATE_MONSTER_FOR_SINGLE(CSession* pSession, UINT32 aiID, UINT32 monsterTypeID, Position monsterPos);
void SC_CREATE_MONSTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 aiID, UINT32 monsterTypeID, Position monsterPos);

void SC_REMOVE_CHARACTER_FOR_All(CSession* pSession, UINT32 playerID);
void SC_REMOVE_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerID);
void SC_REMOVE_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID);

void SC_SPAWN_CHARACTER_FOR_All(CSession* pSession, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo);
void SC_SPAWN_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo);
void SC_SPAWN_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo);

void SC_CHAT_FOR_All(CSession* pSession, UINT32 playerID, std::string message, UINT32 channel);
void SC_CHAT_FOR_SINGLE(CSession* pSession, UINT32 playerID, std::string message, UINT32 channel);
void SC_CHAT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, std::string message, UINT32 channel);

void SC_KEYINFO_FOR_All(CSession* pSession, UINT32 playerID, UINT32 keyInfo, float cameraYaw);
void SC_KEYINFO_FOR_SINGLE(CSession* pSession, UINT32 playerID, UINT32 keyInfo, float cameraYaw);
void SC_KEYINFO_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, UINT32 keyInfo, float cameraYaw);

void SC_PLAYER_ATTACK_FOR_All(CSession* pSession, UINT32 playerID, UINT32 attackType, UINT32 aiID, UINT32 damage, UINT32 targetMaxHP, UINT32 targetCurHP);
void SC_PLAYER_ATTACK_FOR_SINGLE(CSession* pSession, UINT32 playerID, UINT32 attackType, UINT32 aiID, UINT32 damage, UINT32 targetMaxHP, UINT32 targetCurHP);
void SC_PLAYER_ATTACK_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, UINT32 attackType, UINT32 aiID, UINT32 damage, UINT32 targetMaxHP, UINT32 targetCurHP);

void SC_PLAYER_DAMAGED_FOR_All(CSession* pSession, UINT32 playerID, UINT32 damage);
void SC_PLAYER_DAMAGED_FOR_SINGLE(CSession* pSession, UINT32 playerID, UINT32 damage);
void SC_PLAYER_DAMAGED_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, UINT32 damage);

void SC_PLAYER_DIE_FOR_All(CSession* pSession, UINT32 playerID);
void SC_PLAYER_DIE_FOR_SINGLE(CSession* pSession, UINT32 playerID);
void SC_PLAYER_DIE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID);

void SC_POSITION_SYNC_FOR_All(CSession* pSession, UINT32 playerID, float posX, float posY, float cameraYaw);
void SC_POSITION_SYNC_FOR_SINGLE(CSession* pSession, UINT32 playerID, float posX, float posY, float cameraYaw);
void SC_POSITION_SYNC_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, float posX, float posY, float cameraYaw);

void SC_BOSS_PHASE_FOR_All(CSession* pSession, UINT32 bossID, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, Position bossPos, game::BOSS_STATE bossState, float curSpeed);
void SC_BOSS_PHASE_FOR_SINGLE(CSession* pSession, UINT32 bossID, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, Position bossPos, game::BOSS_STATE bossState, float curSpeed);
void SC_BOSS_PHASE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 bossID, UINT32 currentHp, UINT32 maxHp, Position targetMovementPos, Position bossPos, game::BOSS_STATE bossState, float curSpeed);
