#include "Script_Boss.h"

#include "BehaviourTree.h"
#include "AIContext.h"

#include "MakePacket.h"

#include "Room.h"

#include "MathUtils.h"

/*
Behavior Tree 구성도:
------------------------
Root (Selector)
├─ [Sequence] DIE Branch
│       ├─ [Condition] if currentHP <= 0
│       └─ [Action] SC_BOSS_PHASE 호출. 사망 애니메이션 재생 시간동안 RUNNING 상태 유지. 이후 bDeath를 true로 변경

├─ [Sequence] SKILL3 Branch
│       ├─ [Condition] if currentHP / maxHP <= 0.2  // 전체 체력의 20% 이하일 경우
│       └─ [Action] SC_BOSS_PHASE 호출. 1회만 호출

├─ [Sequence] SKILL2 Branch
│       ├─ [Condition] if currentHP / maxHP <= 0.5  // 전체 체력의 50% 이하일 경우
│       └─ [Action] SC_BOSS_PHASE 호출. 1회만 호출

├─ [Sequence] SKILL1 Branch
│       ├─ [Condition] if currentHP / maxHP <= 0.8  // 전체 체력의 80% 이하일 경우
│       └─ [Action] SC_BOSS_PHASE 호출. 1회만 호출

├─ [Sequence] ATTACK Branch
│       ├─ [Condition] if 플레이어가 공격 거리 내에 있을 경우
│       └─ [Action] 공격. 공격 애니메이션 재생시간 동안은 RUNNING 상태 유지

├─ [Sequence] CHASE Branch
│       ├─ [Condition] if 타겟 플레이어가 존재할 경우, 공격 가능 거리보다 거리가 멀 경우
│       └─ [Action] 이동속도에 맞게, 해당 플레이어 위치로 이동

├─ [Sequence] WALK Branch
│       ├─ [Condition] if 타겟 플레이어가 없을 경우, IDLE TIME이 0 이하인 경우
│       └─ [Action] 목표 위치로 이동. 목표 위치로 이동하면 IDLE TIME = 3초 설정

└─ [Action] IDLE
│       ├─ [Condition] IDLE TIME이 0 이상인 경우
        └─ [Action] IDLE TIME이 0이 되면 새로운 목표 위치를 설정
*/

BTNode* CreateBossBT(AIContext& context)
{
    // ==========================================================================  
    // [헬퍼: updateTarget 검사]  
    // = 매 프레임 타겟 업데이트는 별도로 처리되므로,  
    // = 단순히 pTargetPlayer가 nullptr이 아닌지만 검사하여 타겟의 존재 여부를 판단함  
    // ==========================================================================  
    auto hasValidTarget = [&context]() -> bool {
        return (context.pTargetPlayer != nullptr);
        };
    // ==========================================================================  

    // ==========================================================================  
    // 보스의 현재 위치를 Position 형식으로 생성  
    // = 보스의 위치는 context.posX, posY, posZ에서 가져옴  
    // = 이후 여러 단계에서 보스의 위치(bossPos)를 패킷 전송 인자로 사용함  
    // ==========================================================================  
    Position bossPos = context.currentPos;
    // ==========================================================================  

    // ==========================================================================  
    // [DIE Branch 시작] : 사망 브랜치  
    // = 보스가 사망하는 경우를 처리하는 분기  
    // = 조건: currentHP가 0이면 보스가 사망했다고 판단  
    // = 액션: 사망 애니메이션(dieAnimTime) 동안 phaseTimer를 통해 애니메이션 재생을 유지하며,  
    // =         최초 애니메이션 시작 시 패킷 전송(SC_BOSS_PHASE_FOR_AROUND 호출)  
    // =         애니메이션 종료 시 bDeath를 true로 설정하고, 최종 상태를 패킷으로 전송함  
    // ==========================================================================  
    std::vector<BTNode*> dieBranch;

    // 조건 노드: currentHP가 0이면 SUCCESS, 그렇지 않으면 FAILURE 반환
    dieBranch.push_back(
        new ConditionNode([&context]() -> bool {
            return context.currentHP == 0;
            })
    );

    // 액션 노드: 사망 애니메이션 실행 및 패킷 전송  
    dieBranch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {

            if (context.eCurState != game::BOSS_STATE_DIE)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_DIE, 0);

                context.eCurState = game::BOSS_STATE_DIE;
            }

            float initTime = context.dieAnimTime;   // 애니메이션 시작 시간(초)

            // phaseTimer가 초기화되지 않았다면 초기화
            if (context.phaseTimer <= 0)
                context.phaseTimer = initTime;

            // 애니메이션 실행 중이면 phaseTimer 값을 감소시키고 RUNNING 상태 반환
            if (context.phaseTimer > 0)
            {
                context.phaseTimer -= context.deltaTime;
                return NodeStatus::RUNNING;
            }

            // 애니메이션 실행이 완료된 경우, 보스의 사망 상태로 전환
            context.bDeath = true;

            return NodeStatus::SUCCESS;
            })
    );

    BTNode* dieSequence = new SequenceNode(dieBranch);
    // ==========================================================================  

    // ==========================================================================  
    // [SKILL3 Branch 시작] : 스킬3 발동 브랜치  
    // = 보스의 HP가 전체의 20% 이하이며 스킬3을 사용하지 않은 경우 처리  
    // = 액션: skill3AnimTime 동안 스킬3 애니메이션을 실행하고,  
    // =         애니메이션 시작 시에만 패킷 전송하며,  
    // =         애니메이션이 끝나면 usedSkill3를 true로 설정함  
    // ==========================================================================  
    std::vector<BTNode*> skill3Branch;

    skill3Branch.push_back(
        new ConditionNode([&context]() -> bool {
            return (static_cast<float>(context.currentHP) / context.maxHP <= 0.2f)
                && (!context.usedSkill3);
            })
    );

    skill3Branch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {

            if (context.eCurState != game::BOSS_STATE_SKILL3)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_SKILL3, 0);

                context.eCurState = game::BOSS_STATE_SKILL3;
            }

            float initTime = context.skill3AnimTime;
            if (context.phaseTimer <= 0)
                context.phaseTimer = initTime;

            if (context.phaseTimer > 0)
            {
                context.phaseTimer -= context.deltaTime;
                return NodeStatus::RUNNING;
            }

            context.usedSkill3 = true;
            return NodeStatus::SUCCESS;
            })
    );

    BTNode* skill3Sequence = new SequenceNode(skill3Branch);
    // ==========================================================================  

    // ==========================================================================  
    // [SKILL2 Branch 시작] : 스킬2 발동 브랜치  
    // = 보스의 HP가 전체의 50% 이하이며 스킬2을 사용하지 않은 경우 처리  
    // = 액션: skill2AnimTime 동안 스킬2 애니메이션을 실행하고,  
    // =         애니메이션 시작 시에만 패킷 전송하며,  
    // =         애니메이션 종료 시 usedSkill2를 true로 설정함  
    // ==========================================================================  
    std::vector<BTNode*> skill2Branch;

    skill2Branch.push_back(
        new ConditionNode([&context]() -> bool {
            return (static_cast<float>(context.currentHP) / context.maxHP <= 0.5f)
                && (!context.usedSkill2);
            })
    );

    skill2Branch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {

            if (context.eCurState != game::BOSS_STATE_SKILL2)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_SKILL2, 0);

                context.eCurState = game::BOSS_STATE_SKILL2;
            }

            float initTime = context.skill2AnimTime;
            if (context.phaseTimer <= 0)
                context.phaseTimer = initTime;

            if (context.phaseTimer > 0)
            {
                context.phaseTimer -= context.deltaTime;
                return NodeStatus::RUNNING;
            }

            context.usedSkill2 = true;
            return NodeStatus::SUCCESS;
            })
    );

    BTNode* skill2Sequence = new SequenceNode(skill2Branch);
    // ==========================================================================  

    // ==========================================================================  
    // [SKILL1 Branch 시작] : 스킬1 발동 브랜치  
    // = 보스의 HP가 전체의 80% 이하이며 스킬1을 사용하지 않은 경우 처리  
    // = 액션: skill1AnimTime 동안 스킬1 애니메이션을 실행하고,  
    // =         애니메이션 시작 시에만 패킷 전송하며,  
    // =         애니메이션 종료 시 usedSkill1을 true로 설정함  
    // ==========================================================================  
    std::vector<BTNode*> skill1Branch;

    skill1Branch.push_back(
        new ConditionNode([&context]() -> bool {
            return (static_cast<float>(context.currentHP) / context.maxHP <= 0.8f)
                && (!context.usedSkill1);
            })
    );

    skill1Branch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {

            if (context.eCurState != game::BOSS_STATE_SKILL1)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_SKILL1, 0);

                context.eCurState = game::BOSS_STATE_SKILL1;
            }

            float initTime = context.skill1AnimTime;
            if (context.phaseTimer <= 0)
                context.phaseTimer = initTime;

            if (context.phaseTimer > 0)
            {
                context.phaseTimer -= context.deltaTime;
                return NodeStatus::RUNNING;
            }

            context.usedSkill1 = true;
            return NodeStatus::SUCCESS;
            })
    );

    BTNode* skill1Sequence = new SequenceNode(skill1Branch);
    // ==========================================================================  

    // ==========================================================================  
    // [ATTACK Branch 시작] : 공격 브랜치  
    // = 조건: 타겟 플레이어가 존재하며, 보스와의 거리가 공격 범위 이하일 경우  
    // = 액션: attackAnimTime 동안 공격 애니메이션을 실행하며,  
    //         타겟 플레이어의 위치(targetMovementPos)를 전송(패킷 전송은 애니메이션 시작 시에만 수행)  
    // ==========================================================================  
    std::vector<BTNode*> attackBranch;

    attackBranch.push_back(
        new ConditionNode([&context]() -> bool {
            // 타겟 플레이어가 존재하고, 해당 플레이어와의 거리가 공격 범위내에 존재할 경우 
            return context.hasTargetPlayer && (context.playerDistance <= context.attackRange);
            })
    );

    attackBranch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {

            if (context.eCurState != game::BOSS_STATE_ATTACK)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_ATTACK, context.moveSpeed);

                context.eCurState = game::BOSS_STATE_ATTACK;

                context.phaseTimer = context.attackAnimTime;
                std::cout << "공격 실행 " << context.phaseTimer << "초 만큼 공격 애니메이션 실행 진행\n";
            }

            if (context.phaseTimer > 0)
            {
                context.phaseTimer -= context.deltaTime;
                std::cout << "공격 애니메이션 실행중. 남은 지속 시간 : " << context.phaseTimer << "\n";
                return NodeStatus::RUNNING;
            }

            return NodeStatus::SUCCESS;
            })
    );

    BTNode* attackSequence = new SequenceNode(attackBranch);
    // ==========================================================================  

    // ==========================================================================  
    // [CHASE Branch 시작] : 추적 브랜치  
    // = 조건: 타겟 플레이어가 존재하지만, 보스와의 거리가 공격 범위보다 클 경우  
    // = 액션: 보스가 플레이어를 향해 이동하며, playerDistance를 감소시키고,  
    //         이동 속도(context.moveSpeed)를 전송 (패킷 전송은 애니메이션 시작 시에만 수행, bossState: BOSS_STATE_CHASE)  
    // ==========================================================================  
    std::vector<BTNode*> chaseBranch;

    chaseBranch.push_back(
        new ConditionNode([&context]() -> bool {
            // 타겟 플레이어가 존재하고, 해당 플레이어와의 거리가 감지 범위내에 존재할 경우 
            return context.hasTargetPlayer && (context.playerDistance <= context.detectionRange);
            })
    );

    chaseBranch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {

            if (context.eCurState != game::BOSS_STATE_CHASE)
            {
                std::cout << "타겟 플레이어 감지됨, Chase 모드 작동\n";

                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_CHASE, 0);

                context.eCurState = game::BOSS_STATE_CHASE;
            }
            else
            {
                std::cout << "Chase 중... 새로운 위치를 갱신해서 쫒아가는 중\n";

                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_CHASE, 0);
            }

            // --- 이동 로직 추가 부분 ---
            // 1) 목표 벡터 계산
            Position targetPos = context.targetPos;
            float dx = targetPos.posX - context.currentPos.posX;
            float dy = targetPos.posY - context.currentPos.posY;
            float dz = targetPos.posZ - context.currentPos.posZ;

            // 2) 거리 계산
            float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
            context.playerDistance = distance;  // 거리 업데이트

            // 3) 공격 범위에 들어오면 완료
            if (distance <= context.attackRange)
            {
                std::cout << "Chase 완료. 공격 범위 내로 플레이어 들어옴\n";
                return NodeStatus::SUCCESS;
            }

            // 4) 이동
            float moveDistance = context.moveSpeed;
            if (moveDistance >= distance)
            {
                // 목표에 바로 도착
                context.currentPos = targetPos;
            }
            else
            {
                // 방향 정규화 후 이동
                float inv = 1.0f / distance;
                context.currentPos.posX += dx * inv * moveDistance;
                context.currentPos.posY += dy * inv * moveDistance;
                context.currentPos.posZ += dz * inv * moveDistance;
            }

            return NodeStatus::RUNNING;
            })
    );

    BTNode* chaseSequence = new SequenceNode(chaseBranch);
    // ==========================================================================  

    // ==========================================================================  
    // [WALK Branch 시작] : 이동 브랜치  
    // = 조건: 타겟 플레이어가 없고, idleTime이 0 이하일 경우
    // = 액션: 새로운 목표 위치로 이동했다고 가정하고, idleTime을 idleResetTime으로 재설정하며 패킷 전송 (bossState: BOSS_STATE_WALK)  
    // ==========================================================================  
    std::vector<BTNode*> walkBranch;

    walkBranch.push_back(
        new ConditionNode([&context]() -> bool {
            return (!context.hasTargetPlayer) && (context.idleTime <= 0);
            })
    );

    walkBranch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {

            // 타겟 플레이어가 존재한다면
            if (context.hasTargetPlayer)
            {
                std::cout << "WALK 중간에 타겟 플레이어 발견\n";
                return NodeStatus::FAILURE;
            }

            if (context.eCurState != game::BOSS_STATE_WALK)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_WALK, 0);

                context.eCurState = game::BOSS_STATE_WALK;
            }

            // 현재 위치와 목표 위치 사이의 방향 벡터 계산
            float dx = context.targetPos.posX - context.currentPos.posX;
            float dy = context.targetPos.posY - context.currentPos.posY;
            float dz = context.targetPos.posZ - context.currentPos.posZ;

            // 거리 계산 (Euclidean Distance)
            float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

            // 이미 도착했거나, 매우 가까운 경우 이동하지 않음
            if (distance < 0.001f) {
                // 도착 처리
                // 목표 위치에 도착했으니 잠시 idle 상태로 전환하기 위해 context.idleTime을 context.idleResetTime로 설정
                context.idleTime = context.idleResetTime;
                
                std::cout << "목표 위치 도착\n";

                return NodeStatus::SUCCESS;
            }

            // 방향 벡터 정규화
            float dirX = dx / distance;
            float dirY = dy / distance;
            float dirZ = dz / distance;

            // 프레임 주기를 고려한 이동 거리
            float moveDistance = context.moveSpeed;  // 현재 25프레임에 1회 실행하는 중

            // 만약 목표까지의 거리가 moveDistance보다 작다면, 목표 지점에 도달
            if (moveDistance >= distance) {
                context.currentPos = context.targetPos;

                std::cout << "현재 위치 : " << context.currentPos.posX << ", " << context.currentPos.posY << ", " << context.currentPos.posZ << "근처 도달함\n";
            }
            else {
                // 방향 * 이동 거리 만큼 이동
                context.currentPos.posX += dirX * moveDistance;
                context.currentPos.posY += dirY * moveDistance;
                context.currentPos.posZ += dirZ * moveDistance;

                std::cout << "현재 위치 : " << context.currentPos.posX << ", " << context.currentPos.posY << ", " << context.currentPos.posZ << "\n";
            }

            return NodeStatus::RUNNING;
            })
    );

    BTNode* walkSequence = new SequenceNode(walkBranch);
    // ==========================================================================  

    // ==========================================================================  
    // [IDLE Action 시작] : 대기 브랜치 (Fallback)  
    // = 조건: idleTime이 0 이상이면 Idle 상태 유지하며 idleTime을 감소시킴  
    // = idleTime이 0 이하가 되면 SUCCESS 반환하여 새로운 목표 위치 설정 기회 제공 (bossState: BOSS_STATE_IDLE)  
    // ==========================================================================  
    BTNode* idleAction = new ActionNode([&context, bossPos]() -> NodeStatus {
        if (context.idleTime > 0)
        {
            // Idle 애니메이션이 시작될 때만 패킷 전송 (idleTime이 idleResetTime과 같을 때)
            if (context.eCurState != game::BOSS_STATE_IDLE)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_IDLE, 0);

                context.eCurState = game::BOSS_STATE_IDLE;
            }

            context.idleTime -= context.deltaTime;

            return NodeStatus::RUNNING;
        }
        // 시간이 다 지났으므로 랜덤한 새로운 위치를 목표로 움직이도록 한다.
        else
        {
            float randomMoveX = RandomRange(3.f, 4.f);
            float randomMoveZ = RandomRange(3.f, 4.f);

            context.targetPos.posX += randomMoveX;
            context.targetPos.posZ += randomMoveZ;

            std::cout << "새로운 목표 위치 갱신\n";
        }

        return NodeStatus::SUCCESS;
        });
    // ==========================================================================  

    // ==========================================================================  
    // [Root Selector 시작] : 전체 Behavior Tree의 루트 노드  
    // = 우선순위: DIE → SKILL3 → SKILL2 → SKILL1 → ATTACK → CHASE → WALK → IDLE  
    // ==========================================================================  
    std::vector<BTNode*> rootChildren;

    rootChildren.push_back(dieSequence);
    rootChildren.push_back(skill3Sequence);
    rootChildren.push_back(skill2Sequence);
    rootChildren.push_back(skill1Sequence);
    rootChildren.push_back(attackSequence);
    rootChildren.push_back(chaseSequence);
    rootChildren.push_back(walkSequence);
    rootChildren.push_back(idleAction);
    // ==========================================================================  

    return new SelectorNode(rootChildren);
}
