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
            // ã���� ���
            SC_FIND_PW_RESPONSE_FOR_SINGLE(pSession, true, login.id);
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
    PlayerInfo player{ "default", 100, 100, 0 }; // �ʱⰪ

    if (loginManager.AddUser(login, player)) {
        loginManager.SaveToFile("users.json");  // ������ ����
        SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, true, 0);   // �α��� ����
        return true;
    }

    SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, false, 3);   // ���� �ڵ� 3 : �� �� ���� ������ �˻縦 �������� �ұ��ϰ� �̹� �����ϴ� �̸��� or ID. ������ ������ �ǽɵ�.
    return true;
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

bool CS_PLAYER_ATTACK(CSession* pSession, UINT32 aiID, UINT32 attackType)
{
    // AI �Ŵ������� AI�� ã�Ƽ� ü���� �����ϰ�, �ش� ü���� ��ȯ����

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    AIEntity* pEntity = aiManager.FindEntityByID(aiID);
    if (pEntity)
    {
        pEntity->GetDamaged(5); // �ϴ� ���Ƿ� 5 �������� �ο���
        const AIContext& context = pEntity->GetContext();
        SC_PLAYER_ATTACK_FOR_AROUND(nullptr, pRoom, pPlayer->m_ID, attackType, aiID, 5, context.maxHP, context.currentHP); // ���Ƿ� �ο��� 5 ������
    }

    return true;
}

bool CS_POSITION_SYNC(CSession* pSession, float posX, float posY, float cameraYaw)
{
    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_POSITION_SYNC_FOR_AROUND(pSession, pRoom, pPlayer->m_ID, posX, posY, cameraYaw);

    return true;
}

bool CS_REGISTER_REQUEST(CSession* pSession, std::string userName)
{
    // 1. ����� �÷��̾� ��ü ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // �÷��̾� �̸� ����
    pPlayer->SetName(userName);

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

    // 4. ���� ����

    // ���� ������ �÷��̾�� ������ �����϶�� �˸�
    PlayerInfo playerInfo;
    playerInfo.playerJobIcon = 0;
    playerInfo.playerMaxHp = pPlayer->GetMaxHp();
    playerInfo.playerMaxMp = pPlayer->GetMaxMp();
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

    return true;
}