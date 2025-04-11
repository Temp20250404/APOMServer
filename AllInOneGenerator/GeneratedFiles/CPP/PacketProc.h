#pragma once

#include "Session.h"
#include "Protobuf/Protocol.pb.h"

extern int g_iSyncCount;

class CObject;
class CSession;

bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket);
void DisconnectSessionProc(CSession* pSession);

bool CS_CHAT(CSession* pSession, UINT32 targetID, std::string message, UINT32 channel);
bool CS_CHECK_TIMEOUT(CSession* pSession, bool bCheck);
bool CS_KEYINFO(CSession* pSession, UINT32 keyInfo, float cameraYaw);
bool CS_LOGIN_REQUEST(CSession* pSession, std::string userName, std::string password);
bool CS_POSITION_SYNC(CSession* pSession, float posX, float posY);
bool CS_REGISTER_REQUEST(CSession* pSession, std::string userName);