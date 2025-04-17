
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

    // 전달받은 builder 함수를 사용하여 행동 트리 생성
    m_behaviorTree = builder(m_context);
}

AIEntity::~AIEntity()
{
    delete m_behaviorTree;
}

void AIEntity::Update()
{
    // 가장 가까운 플레이어를 타겟으로 설정. 나중에 어그로 게이지 같은 기능이 추가되면 여기만 건들이면 끝
    UpdateTarget();

    NodeStatus status = m_behaviorTree->Tick();
    if (status != NodeStatus::RUNNING)
        m_behaviorTree->Initialize();
}

void AIEntity::UpdateTarget()
{
    // 방 정보가 없으면 바로 리턴
    if (m_context.ptargetRoom == nullptr)
        return;

    // 보스의 현재 위치
    float bossX = m_context.currentPos.posX;
    float bossY = m_context.currentPos.posY;
    float bossZ = m_context.currentPos.posZ;

    float closestDistance = FLT_MAX;
    CPlayer* closestPlayer = nullptr;

    // 방에 있는 모든 활성 플레이어에 대해 반복
    const std::vector<CPlayer*>& activePlayers = m_context.ptargetRoom->GetActivePlayers();
    for (CPlayer* player : activePlayers)
    {
        float px, py, pz;
        player->getPosition(px, py, pz);

        // 유클리드 거리를 계산
        float distance = std::sqrt((bossX - px) * (bossX - px) +
            (bossY - py) * (bossY - py) +
            (bossZ - pz) * (bossZ - pz));

        // 인지 범위보다 플레이어와의 거리가 짧다면
        if (m_context.detectionRange > distance)
        {
            // 갱신
            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestPlayer = player;
            }
        }
    }

    // 가장 가까운 플레이어를 타겟으로 설정
    m_context.pTargetPlayer = closestPlayer;

    if (m_context.pTargetPlayer)
    {
        std::cout << "플레이어 감지됨. 거리 : " << closestDistance << "\n";

        m_context.hasTargetPlayer = true;
        m_context.playerDistance = closestDistance;

        // 1) 플레이어 실제 위치
        float px, py, pz;
        m_context.pTargetPlayer->getPosition(px, py, pz);

        // 2) currentPos → playerPos 방향 벡터
        const auto& cur = m_context.currentPos;
        float dx = px - cur.posX;
        float dy = py - cur.posY;
        float dz = pz - cur.posZ;

        // 3) 전체 거리(디버깅 값과 동일하게 closestDistance 사용해도 무방)
        float dist = std::sqrtf(dx * dx + dy * dy + dz * dz);

        // 4) 이동해야 할 거리 = (전체 거리 - 공격 사거리)
        float moveDist = dist - m_context.attackRange;
        if (moveDist <= 0.0f)
        {
            // 이미 사거리 이내라면 현재 위치 유지
            m_context.targetPos = cur;
        }
        else
        {
            // 5) 단위 벡터(normalize)
            float invDist = 1.0f / dist;
            dx *= invDist;
            dy *= invDist;
            dz *= invDist;

            // 6) 최종 목표 위치 계산
            m_context.targetPos.posX = cur.posX + dx * moveDist;
            m_context.targetPos.posY = cur.posY + dy * moveDist;
            m_context.targetPos.posZ = cur.posZ + dz * moveDist;
        }

        // 7) 디버그 출력: 실제 설정된 targetPos
        const auto& tp = m_context.targetPos;
        std::cout << "타겟 위치 갱신 : "
            << tp.posX << ", "
            << tp.posY << ", "
            << tp.posZ << "\n";
    }
    else
    {
        std::cout << "플레이어 감지되지 않음\n";

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
