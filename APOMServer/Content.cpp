
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
    // ������Ʈ ����
    CPlayer* pPlayer = playerPool.Alloc();

    pPlayer->Init(0, 0, 0, 100, 100);   // ���� ��ġ�� 0, 0, 0 / �ִ� hp,mp�� 100���� ����

    return pPlayer;
}

void LoginAcceptObject(CObject* pObj)
{
    CNetIOManager::GetInstance().acceptSessionCnt++;
}