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
    case game::PacketID::CS_Chat:
    {
        std::string message;
        std::string channel;

        game::CS_CHAT pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        message = pkt.message();
        channel = pkt.channel();

        return CS_CHAT(pSession, message, channel);
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
    case game::PacketID::CS_LoginRequest:
    {
        std::string userName;
        std::string password;

        game::CS_LOGIN_REQUEST pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        userName = pkt.username();
        password = pkt.password();

        return CS_LOGIN_REQUEST(pSession, userName, password);
    }
    break;
    case game::PacketID::CS_PositionSync:
    {
        float posX;
        float posY;

        game::CS_POSITION_SYNC pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        posX = pkt.posx();
        posY = pkt.posy();

        return CS_POSITION_SYNC(pSession, posX, posY);
    }
    break;
    case game::PacketID::CS_RegisterRequest:
    {
        std::string userName;

        game::CS_REGISTER_REQUEST pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        userName = pkt.username();

        return CS_REGISTER_REQUEST(pSession, userName);
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
bool CS_CHAT(CSession* pSession, std::string message, std::string channel)
{
    return false;
}

bool CS_CHECK_TIMEOUT(CSession* pSession, bool bCheck)
{
    return false;
}

bool CS_KEYINFO(CSession* pSession, UINT32 keyInfo, float cameraYaw)
{
    return false;
}

bool CS_LOGIN_REQUEST(CSession* pSession, std::string userName, std::string password)
{
    return false;
}

bool CS_POSITION_SYNC(CSession* pSession, float posX, float posY)
{
    return false;
}

bool CS_REGISTER_REQUEST(CSession* pSession, std::string userName)
{
    return false;
}
