
#include "AIEntity.h"
#include "RoomManager.h"
#include "Player.h"

UINT32 AIEntity::g_id = 0;

AIEntity::AIEntity(const AIContext& aiContext, BTBuilder builder)
{
    m_id = g_id++;
    m_context = aiContext;
    // ���޹��� builder �Լ��� ����Ͽ� �ൿ Ʈ�� ����
    m_behaviorTree = builder(m_context);
}

AIEntity::~AIEntity()
{
    delete m_behaviorTree;
}

void AIEntity::Update()
{
    // ���� ����� �÷��̾ Ÿ������ ����. ���߿� ��׷� ������ ���� ����� �߰��Ǹ� ���⸸ �ǵ��̸� ��
    UpdateTarget();

    NodeStatus status = m_behaviorTree->Tick();
    if (status != NodeStatus::RUNNING)
        m_behaviorTree->Initialize();
}

void AIEntity::UpdateTarget()
{
    // �� ������ ������ �ٷ� ����
    if (m_context.ptargetRoom == nullptr)
        return;

    // ������ ���� ��ġ
    float bossX = m_context.posX;
    float bossY = m_context.posY;
    float bossZ = m_context.posZ;

    float closestDistance = FLT_MAX;
    CPlayer* closestPlayer = nullptr;

    // �濡 �ִ� ��� Ȱ�� �÷��̾ ���� �ݺ�
    const std::vector<CPlayer*>& activePlayers = m_context.ptargetRoom->GetActivePlayers();
    for (CPlayer* player : activePlayers)
    {
        float px, py, pz;
        player->getPosition(px, py, pz);
        // ��Ŭ���� �Ÿ��� ���
        float distance = std::sqrt((bossX - px) * (bossX - px) +
            (bossY - py) * (bossY - py) +
            (bossZ - pz) * (bossZ - pz));
        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestPlayer = player;
        }
    }

    // ���� ����� �÷��̾ Ÿ������ ����
    m_context.pTargetPlayer = closestPlayer;
}