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

        // 해당 방의 패킷 생성 권한을 가진 플레이어라면
        if (pPlayer->m_ID == pRoom->authorityPlayerID)
        {
            // 새로운 플레이어에게 권한 생성을 넘김
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
            // 찾았을 경우, 패스워드를 전송
            SC_FIND_PW_RESPONSE_FOR_SINGLE(pSession, true, login.pw);
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

bool CS_REQUEST_CHARACTER_INFO(CSession* pSession, std::string id)
{
    // id를 기반으로 플레이어 정보 추출
    for (const auto& [login, _] : loginManager.GetUserMap()) {
        if (login.id == id) {
            SC_RESPONSE_CHARACTER_INFO_FOR_SINGLE(pSession, _);
            return true;
        }
    }

    // 로그인에 성공했을테니 당연히 존재할 것이라 생각하지만 혹시 버그일 수 있으니 없을 경우 연결 종료 처리
    return false;
}

bool CS_REQUEST_ITEM_INFO(CSession* pSession, std::string id)
{
    // id를 기반으로 아이템 정보 추출. 지금은 없음.

    return false;
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
    PlayerInfo player{ "default", 2, 1 }; // 초기값

    if (loginManager.AddUser(login, player)) {
        loginManager.SaveToFile("users.json");  // 데이터 저장
        SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, true, 0);   // 로그인 성공
        return true;
    }

    SC_SIGNUP_RESPONSE_FOR_SINGLE(pSession, false, 3);   // 에러 코드 3 : 알 수 없는 이유로 검사를 했음에도 불구하고 이미 존재하는 이메일 or ID. 데이터 오염이 의심됨.
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

    // 나간 플레이어의 정보를 Room에서 나가도록 함
    CPlayer* pPlayer = (CPlayer*)pSession->pObj;

    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    if (pRoom)
    {
        //// AIContext (보스)
        //AIContext aiContext{};
        //AIEntity* pAIEntity;

        //for (int i = 0; i < 1; ++i)
        //{
        //    // context에 방 정보 삽입
        //    aiContext.ptargetRoom = pRoom;

        //    // AIEntity 인스턴스 생성
        //    pAIEntity = new AIEntity(aiContext, nullptr);

        //    // AI 매니저에 AI 객체 등록
        //    aiManager.AddEntity(pAIEntity);

        //    // 방에 AI 객체 정보 등록
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

bool CS_MONSTER_ATTACK(CSession* pSession, UINT32 playerID, UINT32 damage)
{ 
    // 플레이어가 데미지를 입음

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_PLAYER_DAMAGED_FOR_AROUND(nullptr, pRoom, playerID, damage);

    return true;
}

bool CS_MONSTER_DIE(CSession* pSession, UINT32 aiID)
{
    // 몬스터가 죽음
    
    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_MONSTER_DIE_FOR_AROUND(nullptr, pRoom, aiID);

    return true;
}

bool CS_PLAYER_DIE(CSession* pSession, UINT32 playerID)
{ 
    // 플레이어가 죽음

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_PLAYER_DIE_FOR_AROUND(nullptr, pRoom, playerID);

    return true;
}

bool CS_PLAYER_ATTACK(CSession* pSession, UINT32 aiID, UINT32 attackDamage)
{
    // AI 매니저에서 AI를 찾아서 체력을 감소하고, 해당 체력을 반환해줌

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_MONSTER_DAMAGED_FOR_AROUND(nullptr, pRoom, aiID, attackDamage);

    return true;
}

bool CS_POSITION_SYNC(CSession* pSession, float posX, float posY, float posZ, float cameraYaw)
{
    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_POSITION_SYNC_FOR_AROUND(pSession, pRoom, pPlayer->m_ID, posX, posY, posZ, cameraYaw);

    // 위치 세팅
    pPlayer->SetPosition(posX, posY, posZ);

    return true;
}

bool CS_MONSTER_AI(CSession* pSession, UINT32 aiID, Position targetMovementPos, Position bossPos, UINT32 bossState, float curSpeed)
{
    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_MONSTER_AI_FOR_AROUND(nullptr, pRoom, aiID, targetMovementPos, bossPos, bossState, curSpeed);

    return true;
}

bool CS_MONSTER_CONDITION(CSession* pSession, UINT32 aiID, UINT32 currentHp, UINT32 maxHp)
{
    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_MONSTER_CONDITION_FOR_AROUND(nullptr, pRoom, aiID, currentHp, maxHp);

    return true;
}

bool CS_MONSTER_ROTATE(CSession* pSession, UINT32 aiID, float rotateY)
{
    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    SC_MONSTER_ROTATE_FOR_AROUND(nullptr, pRoom, aiID, rotateY);

    return true;
}

bool CS_REGISTER_REQUEST(CSession* pSession, bool bRequest)
{
    // 1. 연결된 플레이어 객체 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

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

    // 방에 생성 권한을 가진 플레이어가 있는지 유무 살피기
    if (room->authorityPlayerID == 0)
    {
        SC_START_AI_CALCULATE_FOR_SINGLE(pSession, true);
        room->authorityPlayerID = pPlayer->m_ID;    // m_ID는 1번부터 시작함
    }

    // 4. 접속 성공

    // 나중엔 json 파일에서 데이터를 읽어와서 적용. 지금은 임의로 한다.
    
    // 새로 접속한 플레이어에게 스스로 생성하라고 알림
    PlayerInfo playerInfo;
    playerInfo.playerJob = 2;   // 2번은 아쳐, 우선은 이렇게 설정하고 나중에 바꾼다.
    playerInfo.level = 1;       // pPlayer->GetLevel();
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

    // 방에 있는 ai를 모두 생성하는 패킷 전송
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
