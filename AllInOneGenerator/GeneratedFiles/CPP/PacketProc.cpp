#include "pch.h"
#include "PacketProc.h"
#include "Packet.h"
#include "MakePacket.h"

#include "SessionManager.h"

#include "ObjectManager.h"

#include "Player.h"

#include "MemoryPoolManager.h"
#include "TimerManager.h"
#include "LogManager.h"

#include "Protobuf/Protocol.pb.h"

static CObjectManager& objectManager = CObjectManager::GetInstance();
static CTimerManager& timerManager = CTimerManager::GetInstance();
static LogManager& logManager = LogManager::GetInstance();

int g_iSyncCount = 0;

bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket)
{
    switch (packetType)
    {
    case game::PacketID::CS_FindIdRequest:
    {
        std::string email;

        game::CS_FIND_ID_REQUEST pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        email = pkt.email();

        return CS_FIND_ID_REQUEST(pSession, email);
    }
    break;
    case game::PacketID::CS_FindPwRequest:
    {
        std::string id;
        std::string email;

        game::CS_FIND_PW_REQUEST pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        id = pkt.id();
        email = pkt.email();

        return CS_FIND_PW_REQUEST(pSession, id, email);
    }
    break;
    case game::PacketID::CS_LoginRequest:
    {
        std::string id;
        std::string password;

        game::CS_LOGIN_REQUEST pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        id = pkt.id();
        password = pkt.password();

        return CS_LOGIN_REQUEST(pSession, id, password);
    }
    break;
    case game::PacketID::CS_SignupRequest:
    {
        std::string id;
        std::string email;
        std::string password;

        game::CS_SIGNUP_REQUEST pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        id = pkt.id();
        email = pkt.email();
        password = pkt.password();

        return CS_SIGNUP_REQUEST(pSession, id, email, password);
    }
    break;
    case game::PacketID::CS_Chat:
    {
        UINT32 targetID;
        std::string message;
        UINT32 channel;

        game::CS_CHAT pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        targetID = pkt.targetid();
        message = pkt.message();
        channel = pkt.channel();

        return CS_CHAT(pSession, targetID, message, channel);
    }
    break;
    case game::PacketID::CS_Keyinfo:
    {
        UINT32 keyInfo;
        float cameraYaw;

        game::CS_KEYINFO pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        keyInfo = pkt.keyinfo();
        cameraYaw = pkt.camerayaw();

        return CS_KEYINFO(pSession, keyInfo, cameraYaw);
    }
    break;
    case game::PacketID::CS_PlayerAttack:
    {
        UINT32 aiID;
        UINT32 attackType;

        game::CS_PLAYER_ATTACK pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        aiID = pkt.aiid();
        attackType = pkt.attacktype();

        return CS_PLAYER_ATTACK(pSession, aiID, attackType);
    }
    break;
    case game::PacketID::CS_PositionSync:
    {
        float posX;
        float posY;
        float cameraYaw;

        game::CS_POSITION_SYNC pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        posX = pkt.posx();
        posY = pkt.posy();
        cameraYaw = pkt.camerayaw();

        return CS_POSITION_SYNC(pSession, posX, posY, cameraYaw);
    }
    break;
    case game::PacketID::CS_CheckTimeout:
    {
        bool bCheck;

        game::CS_CHECK_TIMEOUT pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        bCheck = pkt.bcheck();

        return CS_CHECK_TIMEOUT(pSession, bCheck);
    }
    break;
    default:
        break;
    }
    return false;
}

void DisconnectSessionProc(CSession* pSession)
{
    return;
}
bool CS_FIND_ID_REQUEST(CSession* pSession, std::string email)
{
    return false;
}

bool CS_FIND_PW_REQUEST(CSession* pSession, std::string id, std::string email)
{
    return false;
}

bool CS_LOGIN_REQUEST(CSession* pSession, std::string id, std::string password)
{
    return false;
}

bool CS_SIGNUP_REQUEST(CSession* pSession, std::string id, std::string email, std::string password)
{
    return false;
}

bool CS_CHAT(CSession* pSession, UINT32 targetID, std::string message, UINT32 channel)
{
    return false;
}

bool CS_KEYINFO(CSession* pSession, UINT32 keyInfo, float cameraYaw)
{
    return false;
}

bool CS_PLAYER_ATTACK(CSession* pSession, UINT32 aiID, UINT32 attackType)
{
    return false;
}

bool CS_POSITION_SYNC(CSession* pSession, float posX, float posY, float cameraYaw)
{
    return false;
}

bool CS_CHECK_TIMEOUT(CSession* pSession, bool bCheck)
{
    return false;
}
