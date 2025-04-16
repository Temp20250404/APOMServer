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

bool CS_FIND_ID_REQUEST(CSession* pSession, std::string email)
{
    // id 찾기 요청
     
    // 모든 사용자 중 해당 이메일을 가진 아이디를 찾는다
    for (const auto& [login, _] : loginManager.GetUserMap()) {
        if (login.email == email) {

            SC_FIND_ID_RESPONSE_FOR_SINGLE(pSession, true, login.id);
            return true;
        }
    }

    // 찾지 못했을 경우
    SC_FIND_ID_RESPONSE_FOR_SINGLE(pSession, false, "");
    return true;
}

bool CS_FIND_PW_REQUEST(CSession* pSession, std::string id, std::string email)
{
    // 비밀번호 찾기 요청

    for (const auto& [login, _] : loginManager.GetUserMap()) {
        if (login.id == id && login.email == email) {
            // 찾았을 경우
            SC_FIND_PW_RESPONSE_FOR_SINGLE(pSession, true, login.id);
            return true;
        }
    }

    // 찾지 못했을 경우
    SC_FIND_PW_RESPONSE_FOR_SINGLE(pSession, false, "");
    return true;
}

bool CS_LOGIN_REQUEST(CSession* pSession, std::string id, std::string password)
{
    // 로그인 요청

    for (const auto& [login, _] : loginManager.GetUserMap()) {

        // id를 찾았을 경우
        if (login.id == id) {

            // 비밀번호까지 찾았을 경우
            if (login.pw == password) {
                SC_LOGIN_RESPONSE_FOR_SINGLE(pSession, true, 0);
                return true;
            }

            // 비밀 번호가 틀릴 경우
            else {
                SC_LOGIN_RESPONSE_FOR_SINGLE(pSession, false, 2);   // 에러 코드 2 : 비밀번호 불일치
                return true;
            }
        }
    }

    // 찾지 못했을 경우
    SC_LOGIN_RESPONSE_FOR_SINGLE(pSession, false, 1);   // 에러 코드 1 : 존재하지 않는 ID
    return true;
}

bool CS_SIGNUP_REQUEST(CSession* pSession, std::string id, std::string email, std::string password)
{
    // 회원가입 시도

    if (loginManager.IsDuplicateId(id)) {
        SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, false, 1);  // 에러 코드 1 : 이미 존재하는 id
        return true;
    }

    if (loginManager.IsDuplicateEmail(email)) {
        SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, false, 2);  // 에러 코드 2 : 이미 존재하는 이메일
        return true;
    }

    LoginInfo login{ id, email, password };
    PlayerInfo player{ "default", 100, 100, 0 }; // 초기값

    if (loginManager.AddUser(login, player)) {
        loginManager.SaveToFile("users.json");  // 데이터 저장
        SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, true, 0);   // 로그인 성공
        return true;
    }

    SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, false, 3);   // 에러 코드 3 : 알 수 없는 이유로 검사를 했음에도 불구하고 이미 존재하는 이메일 or ID. 데이터 오염이 의심됨.
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

bool CS_PLAYER_ATTACK(CSession* pSession, UINT32 aiID, UINT32 attackType)
{
    // AI 매니저에서 AI를 찾아서 체력을 감소하고, 해당 체력을 반환해줌

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    AIEntity* pEntity = aiManager.FindEntityByID(aiID);
    if (pEntity)
    {
        pEntity->GetDamaged(5); // 일단 임의로 5 데미지를 부여함
        const AIContext& context = pEntity->GetContext();
        SC_PLAYER_ATTACK_FOR_AROUND(nullptr, pRoom, pPlayer->m_ID, attackType, aiID, 5, context.maxHP, context.currentHP); // 임의로 부여한 5 데미지
    }

    return true;
}

bool CS_POSITION_SYNC(CSession* pSession, float posX, float posY, float cameraYaw)
{
    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_POSITION_SYNC_FOR_AROUND(pSession, pRoom, pPlayer->m_ID, posX, posY, cameraYaw);

    return true;
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

    Position playerPos = { 0, 0, 0 };
    SC_SPAWN_CHARACTER_FOR_SINGLE(
        pSession, pPlayer->m_ID,
        playerPos,
        0,
        playerInfo
    );

    // 이미 있던 activePlayer들에게 새로 룸에 접속한 플레이어의 정보를 전송
    SC_SPAWN_CHARACTER_FOR_AROUND(pSession, room, pPlayer->m_ID,
        playerPos,
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
                playerPos,
                0,
                playerInfo
            );
        }
    }

    // active로 이동
    room->MoveToActive(pPlayer->m_ID);

    return true;
}