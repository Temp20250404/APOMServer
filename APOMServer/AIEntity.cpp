
#include "AIEntity.h"
#include "RoomManager.h"
#include "Player.h"

UINT32 AIEntity::g_id = 1;

AIEntity::AIEntity(const AIContext& aiContext, BTBuilder builder)
    : m_id(g_id++)
    , m_context(aiContext)
    , m_behaviorTree(nullptr)
{
    m_context.ID = m_id;

    //m_context.ptargetRoom = aiContext.ptargetRoom;

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
    float bossX = m_context.currentPos.posX;
    float bossY = m_context.currentPos.posY;
    float bossZ = m_context.currentPos.posZ;

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

        // ���� �������� �÷��̾���� �Ÿ��� ª�ٸ�
        if (m_context.detectionRange > distance)
        {
            // ����
            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestPlayer = player;
            }
        }
    }

    // ���� ����� �÷��̾ Ÿ������ ����
    m_context.pTargetPlayer = closestPlayer;

    if (m_context.pTargetPlayer)
    {
        std::cout << "�÷��̾� ������. �Ÿ� : " << closestDistance << "\n";

        m_context.hasTargetPlayer = true;
        m_context.playerDistance = closestDistance;

        // 1) �÷��̾� ���� ��ġ
        float px, py, pz;
        m_context.pTargetPlayer->getPosition(px, py, pz);

        // 2) currentPos �� playerPos ���� ����
        const auto& cur = m_context.currentPos;
        float dx = px - cur.posX;
        float dy = py - cur.posY;
        float dz = pz - cur.posZ;

        // 3) ��ü �Ÿ�(����� ���� �����ϰ� closestDistance ����ص� ����)
        float dist = std::sqrtf(dx * dx + dy * dy + dz * dz);

        // 4) �̵��ؾ� �� �Ÿ� = (��ü �Ÿ� - ���� ��Ÿ�)
        float moveDist = dist - m_context.attackRange;
        if (moveDist <= 0.0f)
        {
            // �̹� ��Ÿ� �̳���� ���� ��ġ ����
            m_context.targetPos = cur;
        }
        else
        {
            // 5) ���� ����(normalize)
            float invDist = 1.0f / dist;
            dx *= invDist;
            dy *= invDist;
            dz *= invDist;

            // 6) ���� ��ǥ ��ġ ���
            m_context.targetPos.posX = cur.posX + dx * moveDist;
            m_context.targetPos.posY = cur.posY + dy * moveDist;
            m_context.targetPos.posZ = cur.posZ + dz * moveDist;
        }

        // 7) ����� ���: ���� ������ targetPos
        const auto& tp = m_context.targetPos;
        std::cout << "Ÿ�� ��ġ ���� : "
            << tp.posX << ", "
            << tp.posY << ", "
            << tp.posZ << "\n";
    }
    else
    {
        std::cout << "�÷��̾� �������� ����\n";

        m_context.hasTargetPlayer = false;
        m_context.playerDistance = 0.f;
    }
}

void AIEntity::GetDamaged(UINT damage)
{
    m_context.currentHP -= damage;

    if (m_context.currentHP <= 0)
        m_context.currentHP = 0;
}
