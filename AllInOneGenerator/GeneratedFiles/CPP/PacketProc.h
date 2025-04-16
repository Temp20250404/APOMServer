#pragma once

#include "Session.h"
#include "Protobuf/Protocol.pb.h"

extern int g_iSyncCount;

class CObject;
class CSession;

bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket);
void DisconnectSessionProc(CSession* pSession);

bool CS_FIND_ID_REQUEST(CSession* pSession, std::string email);
bool CS_FIND_PW_REQUEST(CSession* pSession, std::string id, std::string email);
bool CS_LOGIN_REQUEST(CSession* pSession, std::string id, std::string password);
bool CS_SIGNUP_REQUEST(CSession* pSession, std::string id, std::string email, std::string password);
bool CS_CHAT(CSession* pSession, UINT32 targetID, std::string message, UINT32 channel);
bool CS_KEYINFO(CSession* pSession, UINT32 keyInfo, float cameraYaw);
bool CS_PLAYER_ATTACK(CSession* pSession, UINT32 aiID, UINT32 attackType);
bool CS_POSITION_SYNC(CSession* pSession, float posX, float posY, float cameraYaw);
bool CS_CHECK_TIMEOUT(CSession* pSession, bool bCheck);