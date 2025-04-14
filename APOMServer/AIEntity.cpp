
#include "AIEntity.h"
#include "RoomManager.h"
#include "Player.h"

UINT32 AIEntity::g_id = 0;

AIEntity::AIEntity(const AIContext& aiContext, BTBuilder builder)
{
    m_id = g_id++;
    m_context = aiContext;
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
    float bossX = m_context.posX;
    float bossY = m_context.posY;
    float bossZ = m_context.posZ;

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
        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestPlayer = player;
        }
    }

    // 가장 가까운 플레이어를 타겟으로 설정
    m_context.pTargetPlayer = closestPlayer;
}