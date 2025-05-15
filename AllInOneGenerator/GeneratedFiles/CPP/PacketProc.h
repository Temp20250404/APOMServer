#pragma once

#include "Session.h"
#include "Protobuf/Protocol.pb.h"
#include "ProtoStruct.h"

extern int g_iSyncCount;

class CObject;
class CSession;

bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket);
void DisconnectSessionProc(CSession* pSession);

bool CS_FIND_ID_REQUEST(CSession* pSession, std::string email);
bool CS_FIND_PW_REQUEST(CSession* pSession, std::string id, std::string email);
bool CS_LOGIN_REQUEST(CSession* pSession, std::string id, std::string password);
bool CS_REQUEST_CHARACTER_INFO(CSession* pSession, std::string id);
bool CS_REQUEST_ITEM_INFO(CSession* pSession, std::string id);
bool CS_SIGNUP_REQUEST(CSession* pSession, std::string id, std::string email, std::string password);
bool CS_ACCEPT_PARTY(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID);
bool CS_CANCEL_ENTERING_DUNGEON(CSession* pSession, bool bCancel);
bool CS_CREATE_MONSTER(CSession* pSession, UINT32 monsterType, Position monsterPos);
bool CS_MAKE_PARTY(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID);
bool CS_REGISTER_REQUEST(CSession* pSession, bool bRequest);
bool CS_REQUEST_ENTER_DUNGEON(CSession* pSession, bool bEnter);
bool CS_RESPONSE_ENTER_DUNGEON_END(CSession* pSession, bool bEnterEnd);
bool CS_SPAWN_CHARACTER(CSession* pSession, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo);
bool CS_START_AI_CALCULATE(CSession* pSession, bool bStart);
bool CS_STOP_AI_CALCULATE(CSession* pSession, bool bStop);
bool CS_CHAT(CSession* pSession, UINT32 targetID, std::string message, UINT32 channel);
bool CS_KEYINFO(CSession* pSession, UINT32 keyInfo, float cameraYaw);
bool CS_MONSTER_ATTACK(CSession* pSession, UINT32 playerID, UINT32 damage);
bool CS_MONSTER_DIE(CSession* pSession, UINT32 aiID);
bool CS_PLAYER_ATTACK(CSession* pSession, UINT32 aiID, UINT32 attackDamage);
bool CS_PLAYER_DIE(CSession* pSession, UINT32 playerID);
bool CS_POSITION_SYNC(CSession* pSession, float posX, float posY, float posZ, float cameraYaw);
bool CS_MONSTER_AI(CSession* pSession, UINT32 aiID, Position targetMovementPos, Position bossPos, UINT32 bossState, float curSpeed);
bool CS_MONSTER_CONDITION(CSession* pSession, UINT32 aiID, UINT32 currentHp, UINT32 maxHp);
bool CS_MONSTER_ROTATE(CSession* pSession, UINT32 aiID, float rotateY);
bool CS_CHECK_TIMEOUT(CSession* pSession, bool bCheck);
bool CS_TEST_PACKET1(CSession* pSession, const std::vector<UINT32>& tempData);
bool CS_TEST_PACKET2(CSession* pSession, const std::vector<PlayerInfo>& tempData);