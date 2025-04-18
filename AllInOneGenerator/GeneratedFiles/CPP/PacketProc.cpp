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
#include "RoomManager.h"
#include "LoginManager.h"
#include "AIManager.h"
#include "EncodingUtils.h"

static CObjectManager& objectManager = CObjectManager::GetInstance();
static CTimerManager& timerManager     = CTimerManager::GetInstance();
static LogManager&    logManager       = LogManager::GetInstance();
static CRoomManager&  roomManager      = CRoomManager::GetInstance();
static CLoginManager& loginManager     = CLoginManager::GetInstance();
static CAIManager&    aiManager        = CAIManager::GetInstance();

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

    case game::PacketID::CS_RequestCharacterInfo:
    {
        std::string id;

        game::CS_REQUEST_CHARACTER_INFO pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        id = pkt.id();

        return CS_REQUEST_CHARACTER_INFO(pSession, id);
    }
    break;

    case game::PacketID::CS_RequestItemInfo:
    {
        std::string id;

        game::CS_REQUEST_ITEM_INFO pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        id = pkt.id();

        return CS_REQUEST_ITEM_INFO(pSession, id);
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

    case game::PacketID::CS_RegisterRequest:
    {
        bool bRequest;

        game::CS_REGISTER_REQUEST pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        bRequest = pkt.brequest();

        return CS_REGISTER_REQUEST(pSession, bRequest);
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

    case game::PacketID::CS_TestPacket1:
    {
        std::vector<UINT32> tempData;

        game::CS_TEST_PACKET1 pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        auto* __list_tempData = pkt.mutable_tempdata();
        tempData.reserve(__list_tempData->size());
        for (const auto& v : *__list_tempData) tempData.push_back(v);


        return CS_TEST_PACKET1(pSession, tempData);
    }
    break;

    case game::PacketID::CS_TestPacket2:
    {
        std::vector<PlayerInfo> tempData;

        game::CS_TEST_PACKET2 pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        auto* __list_tempData = pkt.mutable_tempdata();
        tempData.reserve(__list_tempData->size());
        for (const auto& v : *__list_tempData)
        {
            PlayerInfo tmp;
            tmp.playerNickname = v.playernickname();
            tmp.playerMaxHp = v.playermaxhp();
            tmp.playerMaxMp = v.playermaxmp();
            tmp.playerJobIcon = v.playerjobicon();
            tempData.push_back(tmp);
        }


        return CS_TEST_PACKET2(pSession, tempData);
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
    // TODO: Implement handler
    return false;
}

bool CS_FIND_PW_REQUEST(CSession* pSession, std::string id, std::string email)
{
    // TODO: Implement handler
    return false;
}

bool CS_LOGIN_REQUEST(CSession* pSession, std::string id, std::string password)
{
    // TODO: Implement handler
    return false;
}

bool CS_REQUEST_CHARACTER_INFO(CSession* pSession, std::string id)
{
    // TODO: Implement handler
    return false;
}

bool CS_REQUEST_ITEM_INFO(CSession* pSession, std::string id)
{
    // TODO: Implement handler
    return false;
}

bool CS_SIGNUP_REQUEST(CSession* pSession, std::string id, std::string email, std::string password)
{
    // TODO: Implement handler
    return false;
}

bool CS_REGISTER_REQUEST(CSession* pSession, bool bRequest)
{
    // TODO: Implement handler
    return false;
}

bool CS_CHAT(CSession* pSession, UINT32 targetID, std::string message, UINT32 channel)
{
    // TODO: Implement handler
    return false;
}

bool CS_KEYINFO(CSession* pSession, UINT32 keyInfo, float cameraYaw)
{
    // TODO: Implement handler
    return false;
}

bool CS_PLAYER_ATTACK(CSession* pSession, UINT32 aiID, UINT32 attackType)
{
    // TODO: Implement handler
    return false;
}

bool CS_POSITION_SYNC(CSession* pSession, float posX, float posY, float cameraYaw)
{
    // TODO: Implement handler
    return false;
}

bool CS_CHECK_TIMEOUT(CSession* pSession, bool bCheck)
{
    // TODO: Implement handler
    return false;
}

bool CS_TEST_PACKET1(CSession* pSession, const std::vector<UINT32>& tempData)
{
    // TODO: Implement handler
    return false;
}

bool CS_TEST_PACKET2(CSession* pSession, const std::vector<PlayerInfo>& tempData)
{
    // TODO: Implement handler
    return false;
}
