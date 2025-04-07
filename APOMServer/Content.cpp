
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
    // ������Ʈ ����
    CPlayer* pPlayer = playerPool.Alloc();

    pPlayer->Init(0, 0, 0, 100);

    return pPlayer;
}

void LoginAcceptObject(CObject* pObj)
{
    CSession* pSession = pObj->m_pSession;

    // 1. ����� �÷��̾� ��ü ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pObj);

    // 2. �� ���� ã�� - �ϴ� �����̶�� �����Ͽ�, �� �������� �ִ� 10���� �� �� �ֵ��� �Ѵ�.
    CRoom* room = CRoomManager::GetInstance().FindAvailableDungeon();
    if (!room)
    {
        // ���� ���� �ο� �ʰ� ó��
        return;
    }

    // 3. ������ �÷��̾� �߰�
    // ���� ������ �÷��̾ ����
    // ó���� Waiting �ʿ� ��
    if (!room->AddPlayer(pPlayer))
    {
        // �뿡 �߰� ���� ó��
        return;
    }
    // active�� �̵�
    room->MoveToActive(pPlayer->m_ID);

    // 4. ���� ����

    //=====================================================================================================================================
    // 4-1. ���� ������ �ִ� �÷��̾��� �ʱ� ������ ���� ������ �÷��̾�� ����
    //=====================================================================================================================================

    // ���� ������ �÷��̾�� ������ �����϶�� �˸�
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


    // �̹� �ִ� activePlayer�鿡�� ���� �뿡 ������ �÷��̾��� ������ ����
    SC_SPAWN_CHARACTER_FOR_All(pSession, pPlayer->m_ID,
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

}