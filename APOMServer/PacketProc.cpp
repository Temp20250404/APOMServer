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
    // 나간 플레이어의 정보를 Room에서 나가도록 함
    CPlayer* pPlayer = (CPlayer*)pSession->pObj;

    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 방이 존재한다면
    if (pRoom)
    {
        // 방에서 해당 플레이어 제거
        pRoom->RemovePlayer(pPlayer->m_ID);

        // 방에 있는 모든 플레이어에게 해당 플레이어가 제거되었다고 알림
        SC_REMOVE_CHARACTER_FOR_AROUND(pSession, pRoom, pPlayer->m_ID);
    }

    // 플레이어 반환
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
    // 해당 세션의 timeout 기간 증가
    // 메시지가 온 것 자체로 이미 체크되는 시간이 갱신됨

    return true;
}

bool CS_KEYINFO(CSession* pSession, UINT32 keyInfo, float cameraYaw)
{
    // 클라이언트로부터 키 정보가 온 경우, 모든 클라이언트에게 해당 키 정보를 뿌려줘야함
    
    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 방에 있는 자신을 포함한 모든 플레이어들에게 패킷 전송.
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
    // 1. 연결된 플레이어 객체 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 플레이어 이름 적용
    pPlayer->SetName(userName);

    // 2. 빈 던전 찾기 - 일단 던전이라는 가정하에, 한 던전에는 최대 10명이 들어갈 수 있도록 한다.
    CRoom* room = CRoomManager::GetInstance().FindAvailableDungeon();
    if (!room)
    {
        // 서버 수용 인원 초과 처리
        return false;
    }

    // 3. 던전에 플레이어 추가
    // 던전 정보를 플레이어에 주입
    // 처음엔 Waiting 쪽에 들어감
    if (!room->AddPlayer(pPlayer))
    {
        // 룸에 추가 실패 처리
        return false;
    }

    // 4. 접속 성공

    // 새로 접속한 플레이어에게 스스로 생성하라고 알림
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

    // 이미 있던 activePlayer들에게 새로 룸에 접속한 플레이어의 정보를 전송
    SC_SPAWN_CHARACTER_FOR_AROUND(pSession, room, pPlayer->m_ID,
        0, 0,
        0,
        playerInfo);


    // 새로 접속한 플레이어에게 이미 있던 activePlayer들 정보를 전송
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

    // active로 이동
    room->MoveToActive(pPlayer->m_ID);

    return true;
}















//
//
//bool CS_ATTACK(CSession* pSession, UINT32 hittedTargetId, float normalX, float normalY, float normalZ, float posX, float posY, float posZ)
//{
//    // 클라이언트가 총을 쐈을 때 보내지는 패킷. 방의 모든 플레이어들에게 전송해야한다.
//
//    // 1. 연결된 플레이어 추출
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. 방 정보 검색
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. 방에 있는 모든 플레이어들에게 패킷 전송. 단, 자기 자신은 제외한다.
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
//    // 무기를 교체한다는 의미
//
//    // 1. 연결된 플레이어 추출
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. 방 정보 검색
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. 방에 있는 모든 플레이어들에게 패킷 전송. 단, 자기자신은 제외
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
//    // 클라이언트가 아이템을 주웠을 때 전송하는 패킷
//
//    // 1. 연결된 플레이어 추출
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. 방 정보 검색
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//    
//    // 3. 아이템 획득 요청 시도
//    if (pRoom->OnItemPickupRequest(pPlayer->m_ID, itemId))
//    {
//        // 획득 성공
//        SC_ITEM_PICK_SUCCESS_FOR_SINGLE(pSession, pPlayer->m_ID, itemId);
//    }
//    else
//    {
//        // 획득 실패
//        SC_ITEM_PICK_FAIL_FOR_SINGLE(pSession, pPlayer->m_ID, itemId);
//    }
//
//    return true;
//}
//
//bool CS_KEY_INPUT(CSession* pSession, bool keyW, bool keyA, bool keyS, bool keyD, float rotateAxisX, float rotateAxisY, UINT32 Jump, float normalX, float normalY, float normalZ)
//{
//    // KeyInput 정보는 자신을 포함한 같은 방에 있는 모든 이들에게 전송한다.
//
//    // 1. 연결된 플레이어 추출
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. 자기 자신에게 입력된 키 정보, 회전 정보 대입
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
//    // 3. 방 정보 검색
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 4. 방에 있는 모든 플레이어들에게 패킷 전송
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
//    // 위치 정보는 방에 있는 모든 플레이어들에게 전송되어야한다.
//
//    // 1. 연결된 플레이어 추출
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. 방 정보 검색
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. 방에 있는 모든 플레이어들에게 패킷 전송. 단, 자기 자신은 제외한다.
//    for (const auto& activePlayer : pRoom->m_activePlayers)
//    {
//        // CS_POS_INTERPOLATION를 클라이언트에서 서버로 보낸다는 것은 activePlayer라는 의미
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
//    // 재시작 요청 패킷
//    
//    // 현재 플레이어는 Room의 waiting에 속해 있고, 재시작 요청시 active로 이동한다.
//    
//    // 1. 연결된 플레이어 추출
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. 방 정보 추출
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. Waiting에서 Active로 이동
//    pRoom->MoveToActive(pPlayer->m_ID);
//
//    // 4. 서버에서 클라이언트에게 자신을 생성하라는 메시지 전송
//    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
//        pPlayer->GetSpawnPosIndex(),    // 팀 id에 따라서 부여된 스폰 번호
//        100,                            // 최대 HP
//        pPlayer->GetTeamId()            // 팀 ID
//    );
//
//    // 5. 방에 있는 모든 플레이어들에게 새로 접속한 플레이어 정보 전송
//    KDAInfo kdaInfo;
//    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
//    {
//            pPlayer->GetKDAInfo(kdaInfo);
//
//            // 자기 자신은 제외
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
//            // 자기 자신은 제외
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
//    // 방에 있는 모든 플레이어에게 메시지를 전송
//
//    // 1. 연결된 플레이어 추출
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. 방 정보 추출
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. 메시지 전송. 메시지는 active 상태일 때만 클라이언트에서 전송이 가능하기에 activePlayer 들에게만 보낸다.
//    // 자기 자신은 메시지를 이미 보냈으니깐 다시 반환하지 않는다.
//    std::wstring wideStr;
//    for (const auto& activePlayer : pRoom->m_activePlayers)
//    {
//        // 메시지를 보낸 플레이어가 아닐때
//        if (activePlayer != pPlayer)
//        {
//            // 매시지 전송
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
//    // 같은 팀 플레이어에게만 메시지를 전송
//
//    // 1. 연결된 플레이어 추출
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. 방 정보 추출
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. 메시지 전송. 메시지는 active 상태일 때만 클라이언트에서 전송이 가능하기에 activePlayer 들에게만 보낸다.
//    // 자기 자신은 메시지를 이미 보냈으니깐 다시 반환하지 않는다.
//    std::wstring wideStr;
//    for (const auto& activePlayer : pRoom->m_activePlayers)
//    {
//        // 같은 team이고, 메시지를 보낸 플레이어가 아닐때
//        if (activePlayer->GetTeamId() == pPlayer->GetTeamId() && activePlayer != pPlayer)
//        {
//            // 매시지 전송
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
//    // 닉네임이 왔다는 것은 이제 waiting에서 active로 옮긴다는 의미
//
//    // 1. 연결된 플레이어 추출
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 채팅 메시지 예제
//    // 확인용. 한글을 보내면 wideStr에서 한글을 확인할 수 있다.
//    //SC_SEND_MESSAGE_FOR_SINGLE(pSession, pPlayer->m_ID, WStringToUtf8(wideStr));
//
//    // 2. 이름 부여
//    std::wstring wideStr = Utf8ToWString(name);
//    pPlayer->SetName(WStringToUtf8(wideStr));
//
//    // 3. waiting에서 active로 이동
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//    pRoom->MoveToActive(pPlayer->m_ID);
//     
//    // 4. 서버에서 클라이언트에게 자신을 생성하라는 메시지 전송
//    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
//        pPlayer->GetSpawnPosIndex(),    // 팀 id에 따라서 부여된 스폰 번호
//        100,                    // 최대 HP
//        pPlayer->GetTeamId()    // 팀 ID
//    );
//
//
//    // 5. 방에 있는 모든 플레이어들에게 새로 접속한 플레이어 정보 전송
//    KDAInfo kdaInfo;
//    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
//    {
//        pPlayer->GetKDAInfo(kdaInfo);
//
//        // 자기 자신은 제외
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
//        // 자기 자신은 제외
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
//    // 클라이언트 쪽에서 서버에게 데미지를 준 플레이어 정보를 전송
//
//    // 1. 연결된 플레이어 추출
//    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);
//
//    // 2. 방 정보 추출
//    CDungeon* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
//
//    // 3. 타겟 플레이어 정보 추출
//    CPlayer* pTargetPlayer = pRoom->FindPlayerById(playerId);
//    
//    // 타겟 플레이어가 존재한다면
//    if (pTargetPlayer)
//    {
//        // 체력이 0 보다 작다면
//        if (hp <= 0)
//        {
//            // 죽인 인원의 kil을 올림
//            pPlayer->AddKill();
//
//            // 죽은 인원의 death가 올라감
//            pTargetPlayer->AddDeath();
//
//            // 마지막에 공격한 인원의 assist를 올림
//            CPlayer* pAssistPlayer = pRoom->FindPlayerById(pTargetPlayer->GetLastAttackedPlayerID());
//            if (pAssistPlayer)
//            {
//                if (pPlayer != pAssistPlayer)
//                    pAssistPlayer->AddAssist();
//            }
//
//            // 플레이어는 죽고, active에서 waiting 상태로 변경됨
//            pRoom->MoveToWaiting(playerId);
//
//            // 플레이어가 다운되었음을 방의 모든 플레이어들에게 전송
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
//            // 해당 플레이어에게 데미지 부여
//            pTargetPlayer->SetCurHp(hp);
//            pTargetPlayer->SetLastAttackedPlayerID(pPlayer->m_ID);  // pPlayer가 마지막으로 pTargetPlayer를 공격했다고 갱신
//
//            // 관련 정보를 방의 모든 플레이어들에게 전송
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