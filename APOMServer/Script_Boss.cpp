#include "Script_Boss.h"

#include "BehaviourTree.h"
#include "AIContext.h"

// Behavior Tree 구성도:
// Root (Selector)
// ├─ [Sequence] Death Branch
// │       ├─ [Condition] 보스가 사망 상태인가? (bDeath == true)
// │       └─ [Action] 사망 처리 (사망 애니메이션 실행 후 삭제)
// ├─ [Sequence] Phase4 Branch (광폭화)
// │       ├─ [Condition] 체력이 50% 이하인가? (currentHP / maxHP <= 0.5)
// │       └─ [Action] 광폭화 발동 (공격력/이동속도 증가, 강력한 스킬 실행)
// ├─ [Sequence] Phase3 Branch (패턴 재정비)
// │       ├─ [Condition] 체력이 80% 이하인가? (currentHP / maxHP <= 0.8)
// │       └─ [Action] 패턴 재정비 (방어 자세, 회복 모션 등 실행)
// ├─ [Sequence] Phase2 Branch (습격 / 일반 공격)
// │       ├─ [Condition] 플레이어와의 거리가 20m 이하인가? (distanceToPlayer <= phase2Distance)
// │       └─ [Action] 접근 및 공격 수행
// │               ├─ [Sub-Action] 플레이어와의 거리가 attackRange보다 크면 접근
// │               └─ [Sub-Action] 공격 범위 내에 들어오면 일반 공격 실행
// └─ [Action] Phase1 (Idle)
//         └─ [Action] 대기 상태 유지 (플레이어와의 거리가 20m 이상일 때)
BTNode* CreateBossBT(AIContext& context)
{
    // -------------------------------------------------------
    // 1) Death Branch
    //    - 조건: 보스가 사망 상태인가? (bDeath == true)
    //    - 액션: 사망 처리 (사망 애니메이션 실행 후 삭제)
    //    - 송신해야 할 정보:
    //         boss_id, current_phase: DEATH, current_hp, max_hp, distance_to_player,
    //         target_player_id, move_target_position
    // -------------------------------------------------------
    std::vector<BTNode*> deathBranch;
    deathBranch.push_back(new ConditionNode(
        [&context]() -> bool {
            return context.bDeath;
        }
    ));
    deathBranch.push_back(new ActionNode(
        [&context]() -> NodeStatus {
            // 송신: Death 상태 패킷 전송 (보스 ID, current_phase: DEATH 등)
            // 예: ServerSendPacket(deathPacket);
            return NodeStatus::SUCCESS;
        }
    ));
    BTNode* deathSequence = new SequenceNode(deathBranch);

    // -------------------------------------------------------
    // 2) Phase4 Branch (광폭화)
    //    - 조건: 체력이 최대 체력의 50% 이하인가? (currentHP / maxHP <= 0.5)
    //    - 액션: 광폭화 발동 (공격력/이동속도 증가, 강력한 스킬 실행)
    //    - 송신해야 할 정보:
    //         boss_id, current_phase: BERSERK, current_hp, max_hp, distance_to_player,
    //         target_player_id, move_target_position
    // -------------------------------------------------------
    std::vector<BTNode*> phase4Branch;
    phase4Branch.push_back(new ConditionNode(
        [&context]() -> bool {
            float hpPercent = context.currentHP / context.maxHP;
            return (hpPercent <= 0.5f);
        }
    ));
    phase4Branch.push_back(new ActionNode(
        [&context]() -> NodeStatus {
            // 예: 상태 업데이트 (버프 적용)
            context.baseDamage *= 1.2f;
            context.moveSpeed *= 1.2f;
            // 송신: BERSERK 상태 패킷 전송
            // 예: ServerSendPacket(berserkPacket);
            return NodeStatus::SUCCESS;
        }
    ));
    BTNode* phase4Sequence = new SequenceNode(phase4Branch);

    // -------------------------------------------------------
    // 3) Phase3 Branch (패턴 재정비)
    //    - 조건: 체력이 최대 체력의 80% 이하인가? (currentHP / maxHP <= 0.8)
    //    - 액션: 패턴 재정비 실행 (방어 자세, 회복 모션 등)
    //    - 송신해야 할 정보:
    //         boss_id, current_phase: RECONFIGURATION, current_hp, max_hp, distance_to_player,
    //         target_player_id, move_target_position
    // -------------------------------------------------------
    std::vector<BTNode*> phase3Branch;
    phase3Branch.push_back(new ConditionNode(
        [&context]() -> bool {
            float hpPercent = context.currentHP / context.maxHP;
            return (hpPercent <= 0.8f);
        }
    ));
    phase3Branch.push_back(new ActionNode(
        [&context]() -> NodeStatus {
            // 송신: RECONFIGURATION 상태 패킷 전송
            // 예: ServerSendPacket(reconfigPacket);
            return NodeStatus::RUNNING;
        }
    ));
    BTNode* phase3Sequence = new SequenceNode(phase3Branch);

    // -------------------------------------------------------
    // 4) Phase2 Branch (습격 / 일반 공격)
    //    - 조건: 플레이어와의 거리가 20m 이하인가? (예: distanceToPlayer <= 20.0f)
    //    - 액션: 접근 및 공격 실행
    //         - 접근: (distanceToPlayer > attackRange)일 경우, 접근 중
    //         - 공격: (distanceToPlayer <= attackRange)일 경우, 공격 실행
    //    - 송신해야 할 정보:
    //         boss_id, current_phase: ATTACK, current_hp, max_hp, distance_to_player,
    //         target_player_id, move_target_position
    // -------------------------------------------------------
    std::vector<BTNode*> phase2Branch;
    phase2Branch.push_back(new ConditionNode(
        [&context]() -> bool {
            const float phase2Distance = 20.0f; // 예시 임계 값
            return (context.distanceToPlayer <= phase2Distance);
        }
    ));
    phase2Branch.push_back(new ActionNode(
        [&context]() -> NodeStatus {
            if (context.distanceToPlayer > context.attackRange) {
                // 플레이어에게 접근 중: 거리 감소
                context.distanceToPlayer -= context.moveSpeed;
                // 송신: ATTACK 상태(이동 중) 패킷 전송
                // 예: ServerSendPacket(attackMovingPacket);
                return NodeStatus::RUNNING;
            }
            else {
                // 공격 범위 내로 도달하여 공격 실행
                // 송신: ATTACK 상태(공격 실행) 패킷 전송
                // 예: ServerSendPacket(attackPacket);
                return NodeStatus::SUCCESS;
            }
        }
    ));
    BTNode* phase2Sequence = new SequenceNode(phase2Branch);

    // -------------------------------------------------------
    // 5) Phase1 Branch (Idle)
    //    - 액션: 보스 대기 상태 유지 (플레이어와의 거리가 멀 경우)
    //    - 송신해야 할 정보:
    //         boss_id, current_phase: IDLE, current_hp, max_hp, distance_to_player,
    //         target_player_id, move_target_position
    // -------------------------------------------------------
    BTNode* phase1Action = new ActionNode(
        [&context]() -> NodeStatus {
            // 송신: IDLE 상태 패킷 전송
            // 예: ServerSendPacket(idlePacket);
            return NodeStatus::SUCCESS;
        }
    );

    // -------------------------------------------------------
    // Root Selector: 우선순위 순으로 Branch를 검사 (Death > Phase4 > Phase3 > Phase2 > Phase1)
    // -------------------------------------------------------
    std::vector<BTNode*> rootChildren;
    rootChildren.push_back(deathSequence);
    rootChildren.push_back(phase4Sequence);
    rootChildren.push_back(phase3Sequence);
    rootChildren.push_back(phase2Sequence);
    rootChildren.push_back(phase1Action);

    return new SelectorNode(rootChildren);
}
