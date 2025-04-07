#pragma once

#include "ProtoStruct.h"

class CSession;
class CRoom;

void SC_CHAT_FOR_All(CSession* pSession, UINT32 playerID, std::string message, std::string channel);
void SC_CHAT_FOR_SINGLE(CSession* pSession, UINT32 playerID, std::string message, std::string channel);
void SC_CHAT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, std::string message, std::string channel);

void SC_KEYINFO_FOR_All(CSession* pSession, UINT32 playerID, UINT32 keyInfo, float cameraYaw);
void SC_KEYINFO_FOR_SINGLE(CSession* pSession, UINT32 playerID, UINT32 keyInfo, float cameraYaw);
void SC_KEYINFO_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, UINT32 keyInfo, float cameraYaw);

void SC_LOGIN_RESPONSE_FOR_All(CSession* pSession, bool success, UINT32 playerID, std::string characterName);
void SC_LOGIN_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, UINT32 playerID, std::string characterName);
void SC_LOGIN_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, UINT32 playerID, std::string characterName);

void SC_POSITION_SYNC_FOR_All(CSession* pSession, UINT32 playerID, float posX, float posY);
void SC_POSITION_SYNC_FOR_SINGLE(CSession* pSession, UINT32 playerID, float posX, float posY);
void SC_POSITION_SYNC_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, float posX, float posY);

void SC_REGISTER_RESPONSE_FOR_All(CSession* pSession, bool success, std::string message);
void SC_REGISTER_RESPONSE_FOR_SINGLE(CSession* pSession, bool success, std::string message);
void SC_REGISTER_RESPONSE_FOR_AROUND(CSession* pSession, CRoom* pRoom, bool success, std::string message);

void SC_SPAWN_CHARACTER_FOR_All(CSession* pSession, UINT32 playerID, float posX, float posY, float cameraYaw, PlayerInfo playerInfo);
void SC_SPAWN_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerID, float posX, float posY, float cameraYaw, PlayerInfo playerInfo);
void SC_SPAWN_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerID, float posX, float posY, float cameraYaw, PlayerInfo playerInfo);
