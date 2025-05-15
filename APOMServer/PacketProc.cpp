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

#include "Protobuf/Protocol.pb.h"

#include "EncodingUtils.h"

static CObjectManager& objectManager = CObjectManager::GetInstance();
static CTimerManager& timerManager = CTimerManager::GetInstance();
static LogManager& logManager = LogManager::GetInstance();
static CRoomManager& roomManager = CRoomManager::GetInstance();
static CLoginManager& loginManager = CLoginManager::GetInstance();
static CAIManager& aiManager = CAIManager::GetInstance();

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

    case game::PacketID::CS_AcceptParty:
    {
        UINT32 FromPlayerID;
        UINT32 ToPlayerID;

        game::CS_ACCEPT_PARTY pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        FromPlayerID = pkt.fromplayerid();
        ToPlayerID = pkt.toplayerid();

        return CS_ACCEPT_PARTY(pSession, FromPlayerID, ToPlayerID);
    }
    break;

    case game::PacketID::CS_CancelEnteringDungeon:
    {
        bool bCancel;

        game::CS_CANCEL_ENTERING_DUNGEON pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        bCancel = pkt.bcancel();

        return CS_CANCEL_ENTERING_DUNGEON(pSession, bCancel);
    }
    break;

    case game::PacketID::CS_CreateMonster:
    {
        UINT32 monsterType;
        Position monsterPos;

        game::CS_CREATE_MONSTER pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        monsterType = pkt.monstertype();
        monsterPos = pkt.monsterpos();

        return CS_CREATE_MONSTER(pSession, monsterType, monsterPos);
    }
    break;

    case game::PacketID::CS_MakeParty:
    {
        UINT32 FromPlayerID;
        UINT32 ToPlayerID;

        game::CS_MAKE_PARTY pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        FromPlayerID = pkt.fromplayerid();
        ToPlayerID = pkt.toplayerid();

        return CS_MAKE_PARTY(pSession, FromPlayerID, ToPlayerID);
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

    case game::PacketID::CS_RequestEnterDungeon:
    {
        bool bEnter;

        game::CS_REQUEST_ENTER_DUNGEON pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        bEnter = pkt.benter();

        return CS_REQUEST_ENTER_DUNGEON(pSession, bEnter);
    }
    break;

    case game::PacketID::CS_ResponseEnterDungeonEnd:
    {
        bool bEnterEnd;

        game::CS_RESPONSE_ENTER_DUNGEON_END pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        bEnterEnd = pkt.benterend();

        return CS_RESPONSE_ENTER_DUNGEON_END(pSession, bEnterEnd);
    }
    break;

    case game::PacketID::CS_SpawnCharacter:
    {
        UINT32 playerID;
        Position playerPos;
        float cameraYaw;
        PlayerInfo playerInfo;

        game::CS_SPAWN_CHARACTER pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        playerID = pkt.playerid();
        playerPos = pkt.playerpos();
        cameraYaw = pkt.camerayaw();
        playerInfo = pkt.playerinfo();

        return CS_SPAWN_CHARACTER(pSession, playerID, playerPos, cameraYaw, playerInfo);
    }
    break;

    case game::PacketID::CS_StartAiCalculate:
    {
        bool bStart;

        game::CS_START_AI_CALCULATE pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        bStart = pkt.bstart();

        return CS_START_AI_CALCULATE(pSession, bStart);
    }
    break;

    case game::PacketID::CS_StopAiCalculate:
    {
        bool bStop;

        game::CS_STOP_AI_CALCULATE pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        bStop = pkt.bstop();

        return CS_STOP_AI_CALCULATE(pSession, bStop);
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

    case game::PacketID::CS_MonsterAttack:
    {
        UINT32 playerID;
        UINT32 damage;

        game::CS_MONSTER_ATTACK pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        playerID = pkt.playerid();
        damage = pkt.damage();

        return CS_MONSTER_ATTACK(pSession, playerID, damage);
    }
    break;

    case game::PacketID::CS_MonsterDie:
    {
        UINT32 aiID;

        game::CS_MONSTER_DIE pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        aiID = pkt.aiid();

        return CS_MONSTER_DIE(pSession, aiID);
    }
    break;

    case game::PacketID::CS_PlayerAttack:
    {
        UINT32 aiID;
        UINT32 attackDamage;

        game::CS_PLAYER_ATTACK pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        aiID = pkt.aiid();
        attackDamage = pkt.attackdamage();

        return CS_PLAYER_ATTACK(pSession, aiID, attackDamage);
    }
    break;

    case game::PacketID::CS_PlayerDie:
    {
        UINT32 playerID;

        game::CS_PLAYER_DIE pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        playerID = pkt.playerid();

        return CS_PLAYER_DIE(pSession, playerID);
    }
    break;

    case game::PacketID::CS_PositionSync:
    {
        float posX;
        float posY;
        float posZ;
        float cameraYaw;

        game::CS_POSITION_SYNC pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        posX = pkt.posx();
        posY = pkt.posy();
        posZ = pkt.posz();
        cameraYaw = pkt.camerayaw();

        return CS_POSITION_SYNC(pSession, posX, posY, posZ, cameraYaw);
    }
    break;

    case game::PacketID::CS_MonsterAi:
    {
        UINT32 aiID;
        Position targetMovementPos;
        Position bossPos;
        UINT32 bossState;
        float curSpeed;

        game::CS_MONSTER_AI pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        aiID = pkt.aiid();
        targetMovementPos = pkt.targetmovementpos();
        bossPos = pkt.bosspos();
        bossState = pkt.bossstate();
        curSpeed = pkt.curspeed();

        return CS_MONSTER_AI(pSession, aiID, targetMovementPos, bossPos, bossState, curSpeed);
    }
    break;

    case game::PacketID::CS_MonsterCondition:
    {
        UINT32 aiID;
        UINT32 currentHp;
        UINT32 maxHp;

        game::CS_MONSTER_CONDITION pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        aiID = pkt.aiid();
        currentHp = pkt.currenthp();
        maxHp = pkt.maxhp();

        return CS_MONSTER_CONDITION(pSession, aiID, currentHp, maxHp);
    }
    break;

    case game::PacketID::CS_MonsterRotate:
    {
        UINT32 aiID;
        float rotateY;

        game::CS_MONSTER_ROTATE pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        aiID = pkt.aiid();
        rotateY = pkt.rotatey();

        return CS_MONSTER_ROTATE(pSession, aiID, rotateY);
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
            tmp.playerJob = v.playerjob();
            tmp.level = v.level();
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
    // ���� �÷��̾��� ������ Room���� �������� ��
    CPlayer* pPlayer = (CPlayer*)pSession->pObj;

    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // ���� �����Ѵٸ�
    if (pRoom)
    {
        // �濡�� �ش� �÷��̾� ����
        pRoom->RemovePlayer(pPlayer->m_ID);

        // �濡 �ִ� ��� �÷��̾�� �ش� �÷��̾ ���ŵǾ��ٰ� �˸�
        SC_REMOVE_CHARACTER_FOR_AROUND(pSession, pRoom, pPlayer->m_ID);

        // �ش� ���� ��Ŷ ���� ������ ���� �÷��̾���
        if (pPlayer->m_ID == pRoom->authorityPlayerID)
        {
            // ���ο� �÷��̾�� ���� ������ �ѱ�
            if (!pRoom->GetActivePlayers().empty())
            {
                CPlayer* pPlayer = pRoom->GetActivePlayers().front();
                if (pPlayer)
                {
                    SC_START_AI_CALCULATE_FOR_SINGLE(pPlayer->m_pSession, true);
                    pRoom->authorityPlayerID = pPlayer->m_ID;
                }
                else
                {
                    pRoom->authorityPlayerID = 0;
                }
            }
            else
            {
                pRoom->authorityPlayerID = 0;
            }
        }


    }

    // �÷��̾� ��ȯ
    playerPool.Free((CPlayer*)pSession->pObj);

    CNetIOManager::GetInstance().disconnectSessionCnt++;

    return;
}

bool CS_FIND_ID_REQUEST(CSession* pSession, std::string email)
{
    // id ã�� ��û
     
    // ��� ����� �� �ش� �̸����� ���� ���̵� ã�´�
    for (const auto& [login, _] : loginManager.GetUserMap()) {
        if (login.email == email) {

            SC_FIND_ID_RESPONSE_FOR_SINGLE(pSession, true, login.id);
            return true;
        }
    }

    // ã�� ������ ���
    SC_FIND_ID_RESPONSE_FOR_SINGLE(pSession, false, "");
    return true;
}

bool CS_FIND_PW_REQUEST(CSession* pSession, std::string id, std::string email)
{
    // ��й�ȣ ã�� ��û

    for (const auto& [login, _] : loginManager.GetUserMap()) {
        if (login.id == id && login.email == email) {
            // ã���� ���, �н����带 ����
            SC_FIND_PW_RESPONSE_FOR_SINGLE(pSession, true, login.pw);
            return true;
        }
    }

    // ã�� ������ ���
    SC_FIND_PW_RESPONSE_FOR_SINGLE(pSession, false, "");
    return true;
}

bool CS_LOGIN_REQUEST(CSession* pSession, std::string id, std::string password)
{
    // �α��� ��û

    for (const auto& [login, _] : loginManager.GetUserMap()) {

        // id�� ã���� ���
        if (login.id == id) {

            // ��й�ȣ���� ã���� ���
            if (login.pw == password) {
                SC_LOGIN_RESPONSE_FOR_SINGLE(pSession, true, 0);
                return true;
            }

            // ��� ��ȣ�� Ʋ�� ���
            else {
                SC_LOGIN_RESPONSE_FOR_SINGLE(pSession, false, 2);   // ���� �ڵ� 2 : ��й�ȣ ����ġ
                return true;
            }
        }
    }

    // ã�� ������ ���
    SC_LOGIN_RESPONSE_FOR_SINGLE(pSession, false, 1);   // ���� �ڵ� 1 : �������� �ʴ� ID
    return true;
}

bool CS_REQUEST_CHARACTER_INFO(CSession* pSession, std::string id)
{
    // id�� ������� �÷��̾� ���� ����
    for (const auto& [login, _] : loginManager.GetUserMap()) {
        if (login.id == id) {
            SC_RESPONSE_CHARACTER_INFO_FOR_SINGLE(pSession, _);
            return true;
        }
    }

    // �α��ο� ���������״� �翬�� ������ ���̶� ���������� Ȥ�� ������ �� ������ ���� ��� ���� ���� ó��
    return false;
}

bool CS_REQUEST_ITEM_INFO(CSession* pSession, std::string id)
{
    // id�� ������� ������ ���� ����. ������ ����.

    return false;
}

bool CS_SIGNUP_REQUEST(CSession* pSession, std::string id, std::string email, std::string password)
{
    // ȸ������ �õ�

    if (loginManager.IsDuplicateId(id)) {
        SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, false, 1);  // ���� �ڵ� 1 : �̹� �����ϴ� id
        return true;
    }

    if (loginManager.IsDuplicateEmail(email)) {
        SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, false, 2);  // ���� �ڵ� 2 : �̹� �����ϴ� �̸���
        return true;
    }

    LoginInfo login{ id, email, password };
    PlayerInfo player{ "default", 2, 1 }; // �ʱⰪ

    if (loginManager.AddUser(login, player)) {
        loginManager.SaveToFile("users.json");  // ������ ����
        SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, true, 0);   // �α��� ����
        return true;
    }

    SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, false, 3);   // ���� �ڵ� 3 : �� �� ���� ������ �˻縦 �������� �ұ��ϰ� �̹� �����ϴ� �̸��� or ID. ������ ������ �ǽɵ�.
    return true;
}

bool CS_ACCEPT_PARTY(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID)
{
    return false;
}

bool CS_CANCEL_ENTERING_DUNGEON(CSession* pSession, bool bCancel)
{
    return false;
}

bool CS_CREATE_MONSTER(CSession* pSession, UINT32 monsterType, Position monsterPos)
{
    static UINT32 aiID = 1;

    // ���� �÷��̾��� ������ Room���� �������� ��
    CPlayer* pPlayer = (CPlayer*)pSession->pObj;

    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    if (pRoom)
    {
        //// AIContext (����)
        //AIContext aiContext{};
        //AIEntity* pAIEntity;

        //for (int i = 0; i < 1; ++i)
        //{
        //    // context�� �� ���� ����
        //    aiContext.ptargetRoom = pRoom;

        //    // AIEntity �ν��Ͻ� ����
        //    pAIEntity = new AIEntity(aiContext, nullptr);

        //    // AI �Ŵ����� AI ��ü ���
        //    aiManager.AddEntity(pAIEntity);

        //    // �濡 AI ��ü ���� ���
        //    pRoom->AddEntity(pAIEntity);
        //}

        SC_CREATE_MONSTER_FOR_AROUND(nullptr, pRoom, /*aiContext.ID*/ aiID++, monsterType, monsterPos);
    }

    return true;
}

bool CS_MAKE_PARTY(CSession* pSession, UINT32 FromPlayerID, UINT32 ToPlayerID)
{
    return false;
}

bool CS_TEST_PACKET1(CSession* pSession, const std::vector<UINT32>& tempData)
{
    return false;
}

bool CS_TEST_PACKET2(CSession* pSession, const std::vector<PlayerInfo>& tempData)
{
    return false;
}

bool CS_CHAT(CSession* pSession, UINT32 targetID, std::string message, UINT32 channel)
{
    CPlayer* pPlayer = (CPlayer*)pSession->pObj;

    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_CHAT_FOR_AROUND(nullptr, pRoom, pPlayer->m_ID, message, channel);

    return true;
}

bool CS_CHECK_TIMEOUT(CSession* pSession, bool bCheck)
{
    // �ش� ������ timeout �Ⱓ ����
    // �޽����� �� �� ��ü�� �̹� üũ�Ǵ� �ð��� ���ŵ�

    return true;
}

bool CS_KEYINFO(CSession* pSession, UINT32 keyInfo, float cameraYaw)
{
    // Ŭ���̾�Ʈ�κ��� Ű ������ �� ���, ��� Ŭ���̾�Ʈ���� �ش� Ű ������ �ѷ������
    
    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. �濡 �ִ� �ڽ��� ������ ��� �÷��̾�鿡�� ��Ŷ ����.
    SC_KEYINFO_FOR_AROUND(nullptr, pRoom, pPlayer->m_ID, keyInfo, cameraYaw);

    return true;
}

bool CS_MONSTER_ATTACK(CSession* pSession, UINT32 playerID, UINT32 damage)
{ 
    // �÷��̾ �������� ����

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_PLAYER_DAMAGED_FOR_AROUND(nullptr, pRoom, playerID, damage);

    return true;
}

bool CS_MONSTER_DIE(CSession* pSession, UINT32 aiID)
{
    // ���Ͱ� ����
    
    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_MONSTER_DIE_FOR_AROUND(nullptr, pRoom, aiID);

    return true;
}

bool CS_PLAYER_DIE(CSession* pSession, UINT32 playerID)
{ 
    // �÷��̾ ����

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_PLAYER_DIE_FOR_AROUND(nullptr, pRoom, playerID);

    return true;
}

bool CS_PLAYER_ATTACK(CSession* pSession, UINT32 aiID, UINT32 attackDamage)
{
    // AI �Ŵ������� AI�� ã�Ƽ� ü���� �����ϰ�, �ش� ü���� ��ȯ����

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_MONSTER_DAMAGED_FOR_AROUND(nullptr, pRoom, aiID, attackDamage);

    return true;
}

bool CS_POSITION_SYNC(CSession* pSession, float posX, float posY, float posZ, float cameraYaw)
{
    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_POSITION_SYNC_FOR_AROUND(pSession, pRoom, pPlayer->m_ID, posX, posY, posZ, cameraYaw);

    // ��ġ ����
    pPlayer->SetPosition(posX, posY, posZ);

    return true;
}

bool CS_MONSTER_AI(CSession* pSession, UINT32 aiID, Position targetMovementPos, Position bossPos, UINT32 bossState, float curSpeed)
{
    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_MONSTER_AI_FOR_AROUND(nullptr, pRoom, aiID, targetMovementPos, bossPos, bossState, curSpeed);

    return true;
}

bool CS_MONSTER_CONDITION(CSession* pSession, UINT32 aiID, UINT32 currentHp, UINT32 maxHp)
{
    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_MONSTER_CONDITION_FOR_AROUND(nullptr, pRoom, aiID, currentHp, maxHp);

    return true;
}

bool CS_MONSTER_ROTATE(CSession* pSession, UINT32 aiID, float rotateY)
{
    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_MONSTER_ROTATE_FOR_AROUND(nullptr, pRoom, aiID, rotateY);

    return true;
}

bool CS_REGISTER_REQUEST(CSession* pSession, bool bRequest)
{
    // 1. ����� �÷��̾� ��ü ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� ã�� - �ϴ� �����̶�� �����Ͽ�, �� �������� �ִ� 10���� �� �� �ֵ��� �Ѵ�.
    CRoom* room = CRoomManager::GetInstance().FindAvailableDungeon();
    if (!room)
    {
        // ���� ���� �ο� �ʰ� ó��
        return false;
    }

    // 3. ������ �÷��̾� �߰�
    // ���� ������ �÷��̾ ����
    // ó���� Waiting �ʿ� ��
    if (!room->AddPlayer(pPlayer))
    {
        // �뿡 �߰� ���� ó��
        return false;
    }

    // �濡 ���� ������ ���� �÷��̾ �ִ��� ���� ���Ǳ�
    if (room->authorityPlayerID == 0)
    {
        SC_START_AI_CALCULATE_FOR_SINGLE(pSession, true);
        room->authorityPlayerID = pPlayer->m_ID;    // m_ID�� 1������ ������
    }

    // 4. ���� ����

    // ���߿� json ���Ͽ��� �����͸� �о�ͼ� ����. ������ ���Ƿ� �Ѵ�.
    
    // ���� ������ �÷��̾�� ������ �����϶�� �˸�
    PlayerInfo playerInfo;
    playerInfo.playerJob = 2;   // 2���� ����, �켱�� �̷��� �����ϰ� ���߿� �ٲ۴�.
    playerInfo.level = 1;       // pPlayer->GetLevel();
    playerInfo.playerNickname = pPlayer->GetName();

    Position playerPos = { 0, 0, 0 };
    SC_SPAWN_CHARACTER_FOR_SINGLE(
        pSession, pPlayer->m_ID,
        playerPos,
        0,
        playerInfo
    );

    // �̹� �ִ� activePlayer�鿡�� ���� �뿡 ������ �÷��̾��� ������ ����
    SC_SPAWN_CHARACTER_FOR_AROUND(pSession, room, pPlayer->m_ID,
        playerPos,
        0,
        playerInfo);


    // ���� ������ �÷��̾�� �̹� �ִ� activePlayer�� ������ ����
    float posX, posY, posZ;
    float rotationAxisX, rotationAxisY;
    for (const auto& activePlayer : room->m_activePlayers)
    {
        if (activePlayer)
        {
            activePlayer->getPosition(posX, posY, posZ);
            activePlayer->GetRotationAxisXY(rotationAxisX, rotationAxisY);

            SC_SPAWN_CHARACTER_FOR_SINGLE(
                pSession, activePlayer->m_ID,
                playerPos,
                0,
                playerInfo
            );
        }
    }

    // active�� �̵�
    room->MoveToActive(pPlayer->m_ID);

    // �濡 �ִ� ai�� ��� �����ϴ� ��Ŷ ����
    for (const auto& entity : room->GetEntities())
    {
        if (entity)
        {
            const AIContext& ctx = entity->GetContext();
            Position pos{ 53.67, 1.21, -33.59 };
            SC_CREATE_MONSTER_FOR_SINGLE(pSession, ctx.ID, 1, pos);
        }
    }

    return true;
}

bool CS_REQUEST_ENTER_DUNGEON(CSession* pSession, bool bEnter)
{
    return false;
}

bool CS_RESPONSE_ENTER_DUNGEON_END(CSession* pSession, bool bEnterEnd)
{
    return false;
}

bool CS_SPAWN_CHARACTER(CSession* pSession, UINT32 playerID, Position playerPos, float cameraYaw, PlayerInfo playerInfo)
{
    return false;
}

bool CS_START_AI_CALCULATE(CSession* pSession, bool bStart)
{
    return false;
}

bool CS_STOP_AI_CALCULATE(CSession* pSession, bool bStop)
{
    return false;
}
