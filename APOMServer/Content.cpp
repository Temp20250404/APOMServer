
#include "pch.h"
#include "Content.h"
#include "Player.h"
#include "MakePacket.h"
#include "ObjectManager.h"

#include "MemoryPoolManager.h"

#include "Protobuf/Protocol.pb.h"
#include "SessionManager.h"
#include "Packet.h"

#include "RoomManager.h"

#include "ProtoStruct.h"

#include "EncodingUtils.h"

CObject* CreateAcceptObject(void)
{
    // 오브젝트 생성
    CPlayer* pPlayer = playerPool.Alloc();

    pPlayer->Init(0, 0, 0, 100);

    return pPlayer;
}

void LoginAcceptObject(CObject* pObj)
{
    CSession* pSession = pObj->m_pSession;

    // 1. 연결된 플레이어 객체 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pObj);

    // 2. 빈 던전 찾기 - 일단 던전이라는 가정하에, 한 던전에는 최대 10명이 들어갈 수 있도록 한다.
    CRoom* room = CRoomManager::GetInstance().FindAvailableDungeon();
    if (!room)
    {
        // 서버 수용 인원 초과 처리
        return;
    }

    // 3. 던전에 플레이어 추가
    // 던전 정보를 플레이어에 주입
    // 처음엔 Waiting 쪽에 들어감
    if (!room->AddPlayer(pPlayer))
    {
        // 룸에 추가 실패 처리
        return;
    }
    // active로 이동
    room->MoveToActive(pPlayer->m_ID);

    // 4. 접속 성공

    //=====================================================================================================================================
    // 4-1. 기존 던전에 있던 플레이어의 초기 정보를 새로 접속한 플레이어에게 전송
    //=====================================================================================================================================

    // 새로 접속한 플레이어에게 스스로 생성하라고 알림
    PlayerInfo playerInfo;
    playerInfo.playerJobIcon = 0;
    playerInfo.playerMaxHp = 100;
    playerInfo.playerMaxMp = 0;
    playerInfo.playerNickname = "user" + std::to_string(pPlayer->m_ID);

    std::wstring wideStr = Utf8ToWString(playerInfo.playerNickname);
    pPlayer->SetName(WStringToUtf8(wideStr));

    SC_SPAWN_CHARACTER_FOR_SINGLE(
        pSession, pPlayer->m_ID,
        0, 0,
        0,
        playerInfo
    );


    // 이미 있던 activePlayer들에게 새로 룸에 접속한 플레이어의 정보를 전송
    SC_SPAWN_CHARACTER_FOR_All(pSession, pPlayer->m_ID,
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

}