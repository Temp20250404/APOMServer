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
├─ [Sequence] Death Branch
│       ├─ [Condition] 보스가 사망 상태인가? (context.bDeath == true)
│       └─ [Action] 사망 처리: 사망 애니메이션 실행 후 삭제하고, 패킷 송신
├─ [Sequence] Phase4 Branch (광폭화)
│       ├─ [Condition] 보스 체력이 최대 체력의 50% 이하인가? (context.currentHP / context.maxHP <= phase4HPPercent)
│       └─ [Action] 광폭화 발동: 공격력과 이동속도 증가 후 패킷 송신
├─ [Sequence] Phase3 Branch (패턴 재정비)
│       ├─ [Condition] 보스 체력이 최대 체력의 80% 이하인가? (context.currentHP / context.maxHP <= phase3HPPercent)
│       └─ [DynamicAction] 재정비 애니메이션 실행 및 패킷 송신
│                    - 만약 타겟이 사라지거나 중단 조건이 충족되면 애니메이션 실행 중지
├─ [Sequence] Phase2 Branch (습격/일반 공격)
│       ├─ [Condition] 플레이어와의 거리가 phase2Distance 이하인가? (실제 거리 <= phase2Distance)
│       └─ [DynamicAction] 접근 및 공격 실행:
│                    - 공격 사거리보다 멀면 플레이어 쪽으로 접근하고 패킷 송신 (RUNNING)
│                    - 공격 사거리 내면 공격 실행하고 패킷 송신 (SUCCESS)
│                    - 중간에 타겟이 사라지면 애니메이션 중단
└─ [Action] Phase1 Branch (대기, Idle)
        └─ [Action] 그 외의 상황(타겟 없음 등)일 때 Idle 상태 유지하고 패킷 송신
*/

BTNode* CreateBossBT(AIContext& context)
{
    // ---------------------------
    // 1. Death Branch
    //    - 조건: 보스가 사망 상태인지 확인 (context.bDeath)
    //    - 액션: 사망 애니메이션 실행 후 보스 삭제, 관련 패킷 전송
    // ---------------------------
    std::vector<BTNode*> deathBranch;

    deathBranch.push_back(new ConditionNode([&context]() -> bool {
        // 보스가 사망 상태이면 true 반환
        return context.bDeath;
        }));

    deathBranch.push_back(new ActionNode([&context]() -> NodeStatus {
        // 사망 처리 실행: 타겟이 있을 경우 타겟과의 거리 계산
        float distance = 0.f;
        UINT32 targetID = 0;

        if (context.pTargetPlayer)
        {
            float px, py, pz;
            // 타겟 플레이어의 위치를 가져옴
            context.pTargetPlayer->getPosition(px, py, pz);

            // 보스와 타겟 플레이어 사이의 유클리드 거리 계산
            distance = CalculateDistance(context.posX, context.posY, context.posZ, px, py, pz);

            targetID = context.pTargetPlayer->GetId();
        }

        // 보스의 현재 위치를 Position 구조체로 생성
        Position currentPos = { context.posX, context.posY, context.posZ };

        // 패킷 전송 함수 호출: 보스 사망 상태 전송
        SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_DEATH,
            static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
            currentPos, targetID, distance);

        return NodeStatus::SUCCESS;
        }));

    BTNode* deathSequence = new SequenceNode(deathBranch);



    // ---------------------------
    // 2. Phase4 Branch (광폭화 / Berserk)
    //    - 조건: 보스 체력이 최대 체력의 50% 이하인지 확인
    //    - 액션: 광폭화 발동 (공격력 및 이동속도 증가) 후 패킷 전송
    // ---------------------------
    std::vector<BTNode*> phase4Branch;

    phase4Branch.push_back(new ConditionNode([&context]() -> bool {
        float hpPercent = context.currentHP / context.maxHP;
        return (hpPercent <= context.phase4HPPercent);  // 예: 0.5 이하이면 광폭화
        }));

    phase4Branch.push_back(new ActionNode([&context]() -> NodeStatus {
        // 광폭화 발동: 공격력과 이동속도를 1.2배로 증가
        context.baseDamage *= 1.2f;
        context.moveSpeed *= 1.2f;

        float distance = 0.f;
        UINT32 targetID = 0;

        // 타겟 플레이어가 있을 경우 거리를 계산
        if (context.pTargetPlayer)
        {
            float px, py, pz;
            context.pTargetPlayer->getPosition(px, py, pz);
            distance = CalculateDistance(context.posX, context.posY, context.posZ, px, py, pz);
            targetID = context.pTargetPlayer->GetId();
        }

        Position currentPos = { context.posX, context.posY, context.posZ };

        // 광폭화 상태 패킷 전송
        SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_BERSERK,
            static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
            currentPos, targetID, distance);

        return NodeStatus::SUCCESS;
        }));

    BTNode* phase4Sequence = new SequenceNode(phase4Branch);



    // ---------------------------
    // 3. Phase3 Branch (패턴 재정비 / Reconfiguration)
    //    - 조건: 보스 체력이 최대 체력의 80% 이하인지 확인
    //    - 액션: 재정비 애니메이션 실행 중 (애니메이션이 완료될 때까지 RUNNING 상태 유지)
    //            중간에 타겟이 없거나 중단 조건이 발생하면 애니메이션 중단하고 SUCCESS 반환
    //    - DynamicActionNode 사용하여 매 Tick마다 조건 체크
    // ---------------------------
    std::vector<BTNode*> phase3Branch;

    phase3Branch.push_back(new ConditionNode([&context]() -> bool {
        float hpPercent = context.currentHP / context.maxHP;
        return (hpPercent <= context.phase3HPPercent);  // 예: 0.8 이하이면 재정비 실행
        }));

    phase3Branch.push_back(new DynamicActionNode(
        // 중단 조건: 타겟 플레이어가 없으면 (또는 다른 중단 조건 추가 가능)
        [&context]() -> bool {
            return (context.pTargetPlayer == nullptr);
        },

        // primaryActionFunc: 재정비 애니메이션 실행 (애니메이션이 진행 중이면 계속 RUNNING)
        [&context]() -> NodeStatus {
            float distance = 0.f;
            UINT32 targetID = 0;

            if (context.pTargetPlayer)
            {
                float px, py, pz;
                context.pTargetPlayer->getPosition(px, py, pz);
                distance = CalculateDistance(context.posX, context.posY, context.posZ, px, py, pz);
                targetID = context.pTargetPlayer->GetId();
            }

            Position currentPos = { context.posX, context.posY, context.posZ };

            // 재정비 상태 패킷 전송
            SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_RECONFIGURATION,
                static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
                currentPos, targetID, distance);

            // 애니메이션이 진행중이므로 계속 RUNNING 반환
            return NodeStatus::RUNNING;
        },

        // alternativeActionFunc: 중단 조건이 충족되면 애니메이션 중단하고 SUCCESS 반환
        [&context]() -> NodeStatus {
            return NodeStatus::SUCCESS;
        }
    ));

    BTNode* phase3Sequence = new SequenceNode(phase3Branch);



    // ---------------------------
    // 4. Phase2 Branch (습격/일반 공격 / Attack)
    //    - 조건: 타겟 플레이어가 존재하며, 보스와 플레이어 간의 거리가 phase2Distance 이하인지 확인
    //    - 액션: DynamicActionNode를 사용하여,
    //              - 공격 사거리(distance) 비교에 따라 접근 또는 공격 실행
    //              - 공격 사거리보다 멀면 플레이어 쪽으로 접근하고 패킷 전송 (RUNNING)
    //              - 공격 사거리 내면 실제 공격 실행하고 패킷 전송 (SUCCESS)
    //              - 중간에 타겟이 사라지면 애니메이션 중단하고 SUCCESS 반환
    // ---------------------------
    std::vector<BTNode*> phase2Branch;

    phase2Branch.push_back(new ConditionNode([&context]() -> bool {
        // 타겟 플레이어가 없으면 조건 불충족
        if (!context.pTargetPlayer)
            return false;

        float px, py, pz;
        context.pTargetPlayer->getPosition(px, py, pz);

        // 보스와 타겟 플레이어 간 거리 계산
        float distance = CalculateDistance(context.posX, context.posY, context.posZ, px, py, pz);

        return (distance <= context.phase2Distance);  // 예: 20m 이하이면 조건 충족
        }));

    phase2Branch.push_back(new DynamicActionNode(
        // 중단 조건: 타겟 플레이어가 없으면 (또는 다른 중단 조건 추가 가능)
        [&context]() -> bool {
            return (context.pTargetPlayer == nullptr);
        },

        // primaryActionFunc: 타겟이 존재하면, 공격 사거리(distance) 비교에 따라 접근 또는 공격 실행
        [&context]() -> NodeStatus {
            float px, py, pz;
            context.pTargetPlayer->getPosition(px, py, pz);

            // 보스와 타겟 사이의 거리 계산
            float distance = CalculateDistance(context.posX, context.posY, context.posZ, px, py, pz);

            UINT32 targetID = context.pTargetPlayer->GetId();

            if (distance > context.attackRange)
            {
                // 공격 사거리보다 멀면 플레이어에게 접근 (이동) 처리
                float dx = px - context.posX;
                float dy = py - context.posY;
                float dz = pz - context.posZ;

                float length = std::sqrt(dx * dx + dy * dy + dz * dz);

                if (length > 0.f)
                {
                    // 보스의 위치를 moveSpeed에 따라 갱신 (플레이어 방향으로 이동)
                    context.posX += (dx / length) * context.moveSpeed;
                    context.posY += (dy / length) * context.moveSpeed;
                    context.posZ += (dz / length) * context.moveSpeed;
                }

                Position currentPos = { context.posX, context.posY, context.posZ };

                // 접근 중 상태 패킷 전송 (ATTACK 상태)
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_ATTACK,
                    static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
                    currentPos, targetID, distance);

                return NodeStatus::RUNNING;
            }
            else
            {
                // 공격 사거리 내에 들어오면 실제 공격 실행
                Position currentPos = { context.posX, context.posY, context.posZ };

                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_ATTACK,
                    static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
                    currentPos, targetID, distance);

                return NodeStatus::SUCCESS;
            }
        },

        // alternativeActionFunc: 타겟이 없으면 바로 SUCCESS 처리
        [&context]() -> NodeStatus {
            return NodeStatus::SUCCESS;
        }
    ));

    BTNode* phase2Sequence = new SequenceNode(phase2Branch);



    // ---------------------------
    // 5. Phase1 Branch (Idle)
    //    - 그 외의 상황(타겟 없음 등)일 때 Idle 상태 유지하며, 패킷 전송
    // ---------------------------
    BTNode* phase1Action = new ActionNode([&context]() -> NodeStatus {
        float distance = 0.f;
        UINT32 targetID = 0;

        if (context.pTargetPlayer)
        {
            float px, py, pz;
            context.pTargetPlayer->getPosition(px, py, pz);
            distance = CalculateDistance(context.posX, context.posY, context.posZ, px, py, pz);
            targetID = context.pTargetPlayer->GetId();
        }

        Position currentPos = { context.posX, context.posY, context.posZ };

        // Idle 상태 패킷 전송
        SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_IDLE,
            static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
            currentPos, targetID, distance);

        return NodeStatus::SUCCESS;
        });



    // ---------------------------
    // 6. Root Selector 구성
    //    - 우선순위 (상위부터 평가): Death > Phase4 > Phase3 > Phase2 > Phase1
    // ---------------------------
    std::vector<BTNode*> rootChildren;

    rootChildren.push_back(deathSequence);
    rootChildren.push_back(phase4Sequence);
    rootChildren.push_back(phase3Sequence);
    rootChildren.push_back(phase2Sequence);
    rootChildren.push_back(phase1Action);



    // 최종적으로 Root Selector 노드를 생성하여 반환
    return new SelectorNode(rootChildren);
}
