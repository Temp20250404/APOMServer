
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
    // 처음엔 Waiting 쪽에 들어감. 플레이어가 참가한 던전의 정보를 보내기 시작. 방의 정보는 [ 새로 접속한 플레이어를 제외한 나머지 플레이어들의 정보, 키 입력 정보와 위치 정보 ]
    if (!room->AddPlayer(pPlayer))
    {
        // 룸에 추가 실패 처리
        return;
    }

    // 4. 접속 성공

    //=====================================================================================================================================
    // 4-1. 기존 던전에 있던 플레이어의 초기 정보를 새로 접속한 플레이어에게 전송
    //=====================================================================================================================================

    //// 던전에 있던 activePlayer들의 정보를 새로 룸에 접속한 플레이어에게 전송
    //float posX, posY, posZ;
    //float rotationAxisX, rotationAxisY;
    //KDAInfo kdaInfo;
    //for (const auto& activePlayer : room->m_activePlayers)
    //{
    //    if (activePlayer)
    //    {
    //        activePlayer->getPosition(posX, posY, posZ);
    //        activePlayer->GetRotationAxisXY(rotationAxisX, rotationAxisY);
    //        activePlayer->GetKDAInfo(kdaInfo);

    //        SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(
    //            pSession, activePlayer->m_ID,
    //            activePlayer->GetSpawnPosIndex(),
    //            activePlayer->GetMaxHp(), activePlayer->GetCurHp(),
    //            activePlayer->GetName(), kdaInfo,
    //            activePlayer->GetWeaponInfo(), activePlayer->GetTeamId());
    //    }
    //}
}