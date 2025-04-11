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

#include "Protobuf/Protocol.pb.h"

#include "EncodingUtils.h"

static CObjectManager& objectManager = CObjectManager::GetInstance();
static CTimerManager& timerManager = CTimerManager::GetInstance();
static LogManager& logManager = LogManager::GetInstance();
static CRoomManager& roomManager = CRoomManager::GetInstance();

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

bool CS_CHAT(CSession* pSession, std::string message, std::string channel)
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

    SC_SPAWN_CHARACTER_FOR_SINGLE(
        pSession, pPlayer->m_ID,
        0, 0,
        0,
        playerInfo
    );

    // �̹� �ִ� activePlayer�鿡�� ���� �뿡 ������ �÷��̾��� ������ ����
    SC_SPAWN_CHARACTER_FOR_AROUND(pSession, room, pPlayer->m_ID,
        0, 0,
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
                0, 0,
                0,
                playerInfo
            );
        }
    }

    // active�� �̵�
    room->MoveToActive(pPlayer->m_ID);

    return true;
}















//
//
//bool CS_ATTACK(CSession* pSession, UINT32 hittedTargetId, float normalX, float normalY, float normalZ, float posX, float posY, float posZ)
//{
//    // Ŭ���̾�Ʈ�� ���� ���� �� �������� ��Ŷ. ���� ��� �÷��̾�鿡�� �����ؾ��Ѵ�.
//
//    // 1. ����� �÷��̾� ����
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. �� ���� �˻�
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. �濡 �ִ� ��� �÷��̾�鿡�� ��Ŷ ����. ��, �ڱ� �ڽ��� �����Ѵ�.
//    for (const auto& activePlayer : pRoom->m_activePlayers)
//    {
//        if (activePlayer == pPlayer)
//            continue;
//
//        SC_ATTACK_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, hittedTargetId, normalX, normalY, normalZ, posX, posY, posZ);
//    }
//    for (const auto& waitingPlayer : pRoom->m_activePlayers)
//    {
//        SC_ATTACK_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, hittedTargetId, normalX, normalY, normalZ, posX, posY, posZ);
//    }
//    return true;
//}
//
//bool CS_CHANGE_WEAPON(CSession* pSession, UINT32 weapon)
//{
//    // ���⸦ ��ü�Ѵٴ� �ǹ�
//
//    // 1. ����� �÷��̾� ����
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. �� ���� �˻�
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. �濡 �ִ� ��� �÷��̾�鿡�� ��Ŷ ����. ��, �ڱ��ڽ��� ����
//    for (const auto& activePlayer : pRoom->m_activePlayers)
//    {
//        SC_CHANGE_WEAPON_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, weapon);
//    }
//    for (const auto& waitingPlayer : pRoom->m_activePlayers)
//    {
//        SC_CHANGE_WEAPON_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, weapon);
//    }
//
//    return true;
//}
//
//bool CS_ITEM_PICKED(CSession* pSession, UINT32 itemId)
//{
//    // Ŭ���̾�Ʈ�� �������� �ֿ��� �� �����ϴ� ��Ŷ
//
//    // 1. ����� �÷��̾� ����
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. �� ���� �˻�
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//    
//    // 3. ������ ȹ�� ��û �õ�
//    if (pRoom->OnItemPickupRequest(pPlayer->m_ID, itemId))
//    {
//        // ȹ�� ����
//        SC_ITEM_PICK_SUCCESS_FOR_SINGLE(pSession, pPlayer->m_ID, itemId);
//    }
//    else
//    {
//        // ȹ�� ����
//        SC_ITEM_PICK_FAIL_FOR_SINGLE(pSession, pPlayer->m_ID, itemId);
//    }
//
//    return true;
//}
//
//bool CS_KEY_INPUT(CSession* pSession, bool keyW, bool keyA, bool keyS, bool keyD, float rotateAxisX, float rotateAxisY, UINT32 Jump, float normalX, float normalY, float normalZ)
//{
//    // KeyInput ������ �ڽ��� ������ ���� �濡 �ִ� ��� �̵鿡�� �����Ѵ�.
//
//    // 1. ����� �÷��̾� ����
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. �ڱ� �ڽſ��� �Էµ� Ű ����, ȸ�� ���� ����
//    std::array<bool, (int)PRESS_KEY::END> keyStates{};
//
//    keyStates[(int)PRESS_KEY::W] = keyW;
//    keyStates[(int)PRESS_KEY::A] = keyA;
//    keyStates[(int)PRESS_KEY::S] = keyS;
//    keyStates[(int)PRESS_KEY::D] = keyD;
//    keyStates[(int)PRESS_KEY::JUMP] = Jump;
//    pPlayer->SetMovementKeys(keyStates);
//
//    pPlayer->SetRotationAxisXY(rotateAxisX, rotateAxisY);
//
//    // 3. �� ���� �˻�
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 4. �濡 �ִ� ��� �÷��̾�鿡�� ��Ŷ ����
//    for (const auto& activePlayer : pRoom->m_activePlayers)
//    {
//        SC_KEY_INPUT_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, keyW, keyA, keyS, keyD, rotateAxisX, rotateAxisY, Jump, normalX, normalY, normalZ);
//    }
//    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
//    {
//        SC_KEY_INPUT_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, keyW, keyA, keyS, keyD, rotateAxisX, rotateAxisY, Jump, normalX, normalY, normalZ);
//    }
//
//    return true;
//}
//
//bool CS_POS_INTERPOLATION(CSession* pSession, float posX, float posY, float posZ)
//{
//    // ��ġ ������ �濡 �ִ� ��� �÷��̾�鿡�� ���۵Ǿ���Ѵ�.
//
//    // 1. ����� �÷��̾� ����
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. �� ���� �˻�
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. �濡 �ִ� ��� �÷��̾�鿡�� ��Ŷ ����. ��, �ڱ� �ڽ��� �����Ѵ�.
//    for (const auto& activePlayer : pRoom->m_activePlayers)
//    {
//        // CS_POS_INTERPOLATION�� Ŭ���̾�Ʈ���� ������ �����ٴ� ���� activePlayer��� �ǹ�
//        if (activePlayer == pPlayer)
//            continue;
//
//        SC_POS_INTERPOLATION_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, posX, posY, posZ);
//    }
//    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
//    {
//        SC_POS_INTERPOLATION_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, posX, posY, posZ);
//    }
//
//    return true;
//}
//
//bool CS_REQUEST_RESTART(CSession* pSession, UINT32 playerId, UINT32 weapon)
//{
//    // ����� ��û ��Ŷ
//    
//    // ���� �÷��̾�� Room�� waiting�� ���� �ְ�, ����� ��û�� active�� �̵��Ѵ�.
//    
//    // 1. ����� �÷��̾� ����
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. �� ���� ����
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. Waiting���� Active�� �̵�
//    pRoom->MoveToActive(pPlayer->m_ID);
//
//    // 4. �������� Ŭ���̾�Ʈ���� �ڽ��� �����϶�� �޽��� ����
//    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
//        pPlayer->GetSpawnPosIndex(),    // �� id�� ���� �ο��� ���� ��ȣ
//        100,                            // �ִ� HP
//        pPlayer->GetTeamId()            // �� ID
//    );
//
//    // 5. �濡 �ִ� ��� �÷��̾�鿡�� ���� ������ �÷��̾� ���� ����
//    KDAInfo kdaInfo;
//    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
//    {
//            pPlayer->GetKDAInfo(kdaInfo);
//
//            // �ڱ� �ڽ��� ����
//            if (waitingPlayer != pPlayer)
//            {
//                    SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(
//                            waitingPlayer->m_pSession,
//                            pPlayer->m_ID,
//                            pPlayer->GetSpawnPosIndex(),
//                            100, pPlayer->GetCurHp(),
//                            pPlayer->GetName(),
//                            kdaInfo,
//                            pPlayer->GetWeaponInfo(),
//                            pPlayer->GetTeamId()
//                    );
//            }
//    }
//    for (const auto& activePlayer : pRoom->m_activePlayers)
//    {
//            pPlayer->GetKDAInfo(kdaInfo);
//
//            // �ڱ� �ڽ��� ����
//            if (activePlayer != pPlayer)
//            {
//                    SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(
//                            activePlayer->m_pSession,
//                            pPlayer->m_ID,
//                            pPlayer->GetSpawnPosIndex(),
//                            100, pPlayer->GetCurHp(),
//                            pPlayer->GetName(),
//                            kdaInfo,
//                            pPlayer->GetWeaponInfo(),
//                            pPlayer->GetTeamId()
//                    );
//            }
//    }
//
//    return true;
//}
//
//bool CS_SEND_MESSAGE_ALL(CSession* pSession, UINT32 playerId, std::string message)
//{
//    // �濡 �ִ� ��� �÷��̾�� �޽����� ����
//
//    // 1. ����� �÷��̾� ����
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. �� ���� ����
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. �޽��� ����. �޽����� active ������ ���� Ŭ���̾�Ʈ���� ������ �����ϱ⿡ activePlayer �鿡�Ը� ������.
//    // �ڱ� �ڽ��� �޽����� �̹� �������ϱ� �ٽ� ��ȯ���� �ʴ´�.
//    std::wstring wideStr;
//    for (const auto& activePlayer : pRoom->m_activePlayers)
//    {
//        // �޽����� ���� �÷��̾ �ƴҶ�
//        if (activePlayer != pPlayer)
//        {
//            // �Ž��� ����
//            wideStr = Utf8ToWString(message);
//            SC_SEND_MESSAGE_ALL_FOR_SINGLE(activePlayer->m_pSession, playerId, WStringToUtf8(wideStr));
//        }
//    }
//
//    return true;
//}
//
//bool CS_SEND_MESSAGE_TEAM(CSession* pSession, UINT32 playerId, std::string message)
//{
//    // ���� �� �÷��̾�Ը� �޽����� ����
//
//    // 1. ����� �÷��̾� ����
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. �� ���� ����
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. �޽��� ����. �޽����� active ������ ���� Ŭ���̾�Ʈ���� ������ �����ϱ⿡ activePlayer �鿡�Ը� ������.
//    // �ڱ� �ڽ��� �޽����� �̹� �������ϱ� �ٽ� ��ȯ���� �ʴ´�.
//    std::wstring wideStr;
//    for (const auto& activePlayer : pRoom->m_activePlayers)
//    {
//        // ���� team�̰�, �޽����� ���� �÷��̾ �ƴҶ�
//        if (activePlayer->GetTeamId() == pPlayer->GetTeamId() && activePlayer != pPlayer)
//        {
//            // �Ž��� ����
//            wideStr = Utf8ToWString(message);
//            SC_SEND_MESSAGE_TEAM_FOR_SINGLE(pSession, playerId, WStringToUtf8(wideStr));
//        }
//    }
//
//    return true;
//}
//
//bool CS_SEND_NICKNAME(CSession* pSession, std::string name)
//{
//    // �г����� �Դٴ� ���� ���� waiting���� active�� �ű�ٴ� �ǹ�
//
//    // 1. ����� �÷��̾� ����
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // ä�� �޽��� ����
//    // Ȯ�ο�. �ѱ��� ������ wideStr���� �ѱ��� Ȯ���� �� �ִ�.
//    //SC_SEND_MESSAGE_FOR_SINGLE(pSession, pPlayer->m_ID, WStringToUtf8(wideStr));
//
//    // 2. �̸� �ο�
//    std::wstring wideStr = Utf8ToWString(name);
//    pPlayer->SetName(WStringToUtf8(wideStr));
//
//    // 3. waiting���� active�� �̵�
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//    pRoom->MoveToActive(pPlayer->m_ID);
//     
//    // 4. �������� Ŭ���̾�Ʈ���� �ڽ��� �����϶�� �޽��� ����
//    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
//        pPlayer->GetSpawnPosIndex(),    // �� id�� ���� �ο��� ���� ��ȣ
//        100,                    // �ִ� HP
//        pPlayer->GetTeamId()    // �� ID
//    );
//
//
//    // 5. �濡 �ִ� ��� �÷��̾�鿡�� ���� ������ �÷��̾� ���� ����
//    KDAInfo kdaInfo;
//    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
//    {
//        pPlayer->GetKDAInfo(kdaInfo);
//
//        // �ڱ� �ڽ��� ����
//        if (waitingPlayer != pPlayer)
//        {
//            SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(
//                waitingPlayer->m_pSession,
//                pPlayer->m_ID,
//                pPlayer->GetSpawnPosIndex(),
//                100, pPlayer->GetCurHp(),
//                pPlayer->GetName(),
//                kdaInfo,
//                pPlayer->GetWeaponInfo(),
//                pPlayer->GetTeamId()
//            );
//        }
//    }
//    for (const auto& activePlayer : pRoom->m_activePlayers)
//    {
//        pPlayer->GetKDAInfo(kdaInfo);
//
//        // �ڱ� �ڽ��� ����
//        if (activePlayer != pPlayer)
//        {
//            SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(
//                activePlayer->m_pSession,
//                pPlayer->m_ID,
//                pPlayer->GetSpawnPosIndex(),
//                100, pPlayer->GetCurHp(),
//                pPlayer->GetName(),
//                kdaInfo,
//                pPlayer->GetWeaponInfo(),
//                pPlayer->GetTeamId()
//            );
//        }
//    }
//
//    return true;
//}
//
//
//bool CS_SHOT_HIT(CSession* pSession, UINT32 playerId, UINT32 hp)
//{
//    // Ŭ���̾�Ʈ �ʿ��� �������� �������� �� �÷��̾� ������ ����
//
//    // 1. ����� �÷��̾� ����
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. �� ���� ����
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. Ÿ�� �÷��̾� ���� ����
//    CPlayer* pTargetPlayer = pRoom->FindPlayerById(playerId);
//    
//    // Ÿ�� �÷��̾ �����Ѵٸ�
//    if (pTargetPlayer)
//    {
//        // ü���� 0 ���� �۴ٸ�
//        if (hp <= 0)
//        {
//            // ���� �ο��� kil�� �ø�
//            pPlayer->AddKill();
//
//            // ���� �ο��� death�� �ö�
//            pTargetPlayer->AddDeath();
//
//            // �������� ������ �ο��� assist�� �ø�
//            CPlayer* pAssistPlayer = pRoom->FindPlayerById(pTargetPlayer->GetLastAttackedPlayerID());
//            if (pAssistPlayer)
//            {
//                if (pPlayer != pAssistPlayer)
//                    pAssistPlayer->AddAssist();
//            }
//
//            // �÷��̾�� �װ�, active���� waiting ���·� �����
//            pRoom->MoveToWaiting(playerId);
//
//            // �÷��̾ �ٿ�Ǿ����� ���� ��� �÷��̾�鿡�� ����
//            std::vector<PlayerInfo> v;
//            for (const auto& activePlayer : pRoom->m_activePlayers)
//            {
//                SC_CHARACTER_DOWN_FOR_SINGLE(activePlayer->m_pSession, playerId, pTargetPlayer->GetTeamId(), pTargetPlayer->GetLastAttackedPlayerID());
//                KDAInfo kda;
//                activePlayer->GetKDAInfo(kda);
//                v.push_back(PlayerInfo(activePlayer->m_ID, kda));
//            }
//             
//            for (const auto& activePlayer : pRoom->m_activePlayers)
//                SC_CHARACTER_KILL_LOG_FOR_SINGLE(activePlayer->m_pSession, v);
//
//            for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
//            {
//                SC_CHARACTER_DOWN_FOR_SINGLE(waitingPlayer->m_pSession, playerId, pTargetPlayer->GetTeamId(), pTargetPlayer->GetLastAttackedPlayerID());
//            }
//        }
//        else
//        {
//            // �ش� �÷��̾�� ������ �ο�
//            pTargetPlayer->SetCurHp(hp);
//            pTargetPlayer->SetLastAttackedPlayerID(pPlayer->m_ID);  // pPlayer�� ���������� pTargetPlayer�� �����ߴٰ� ����
//
//            // ���� ������ ���� ��� �÷��̾�鿡�� ����
//            for (const auto& activePlayer : pRoom->m_activePlayers)
//            {
//                SC_SHOT_HIT_FOR_SINGLE(activePlayer->m_pSession, playerId, hp);
//            }
//            for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
//            {
//                SC_SHOT_HIT_FOR_SINGLE(waitingPlayer->m_pSession, playerId, hp);
//            }
//        }
//    }
//
//    return true;
//}