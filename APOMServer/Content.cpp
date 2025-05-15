
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

#include "NetIOManager.h"

CObject* CreateAcceptObject(void)
{
    // 오브젝트 생성
    CPlayer* pPlayer = playerPool.Alloc();

    pPlayer->Init(0, 0, 0, 100, 100);   // 현재 위치를 0, 0, 0 / 최대 hp,mp를 100으로 설정

    return pPlayer;
}

void LoginAcceptObject(CObject* pObj)
{
    CNetIOManager::GetInstance().acceptSessionCnt++;
}