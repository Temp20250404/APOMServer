#include "Script_Boss.h"

#include "BehaviourTree.h"
#include "AIContext.h"

// Behavior Tree ������:
// Root (Selector)
// ���� [Sequence] Death Branch
// ��       ���� [Condition] ������ ��� �����ΰ�? (bDeath == true)
// ��       ���� [Action] ��� ó�� (��� �ִϸ��̼� ���� �� ����)
// ���� [Sequence] Phase4 Branch (����ȭ)
// ��       ���� [Condition] ü���� 50% �����ΰ�? (currentHP / maxHP <= 0.5)
// ��       ���� [Action] ����ȭ �ߵ� (���ݷ�/�̵��ӵ� ����, ������ ��ų ����)
// ���� [Sequence] Phase3 Branch (���� ������)
// ��       ���� [Condition] ü���� 80% �����ΰ�? (currentHP / maxHP <= 0.8)
// ��       ���� [Action] ���� ������ (��� �ڼ�, ȸ�� ��� �� ����)
// ���� [Sequence] Phase2 Branch (���� / �Ϲ� ����)
// ��       ���� [Condition] �÷��̾���� �Ÿ��� 20m �����ΰ�? (distanceToPlayer <= phase2Distance)
// ��       ���� [Action] ���� �� ���� ����
// ��               ���� [Sub-Action] �÷��̾���� �Ÿ��� attackRange���� ũ�� ����
// ��               ���� [Sub-Action] ���� ���� ���� ������ �Ϲ� ���� ����
// ���� [Action] Phase1 (Idle)
//         ���� [Action] ��� ���� ���� (�÷��̾���� �Ÿ��� 20m �̻��� ��)
BTNode* CreateBossBT(AIContext& context)
{
    // -------------------------------------------------------
    // 1) Death Branch
    //    - ����: ������ ��� �����ΰ�? (bDeath == true)
    //    - �׼�: ��� ó�� (��� �ִϸ��̼� ���� �� ����)
    //    - �۽��ؾ� �� ����:
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
            // �۽�: Death ���� ��Ŷ ���� (���� ID, current_phase: DEATH ��)
            // ��: ServerSendPacket(deathPacket);
            return NodeStatus::SUCCESS;
        }
    ));
    BTNode* deathSequence = new SequenceNode(deathBranch);

    // -------------------------------------------------------
    // 2) Phase4 Branch (����ȭ)
    //    - ����: ü���� �ִ� ü���� 50% �����ΰ�? (currentHP / maxHP <= 0.5)
    //    - �׼�: ����ȭ �ߵ� (���ݷ�/�̵��ӵ� ����, ������ ��ų ����)
    //    - �۽��ؾ� �� ����:
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
            // ��: ���� ������Ʈ (���� ����)
            context.baseDamage *= 1.2f;
            context.moveSpeed *= 1.2f;
            // �۽�: BERSERK ���� ��Ŷ ����
            // ��: ServerSendPacket(berserkPacket);
            return NodeStatus::SUCCESS;
        }
    ));
    BTNode* phase4Sequence = new SequenceNode(phase4Branch);

    // -------------------------------------------------------
    // 3) Phase3 Branch (���� ������)
    //    - ����: ü���� �ִ� ü���� 80% �����ΰ�? (currentHP / maxHP <= 0.8)
    //    - �׼�: ���� ������ ���� (��� �ڼ�, ȸ�� ��� ��)
    //    - �۽��ؾ� �� ����:
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
            // �۽�: RECONFIGURATION ���� ��Ŷ ����
            // ��: ServerSendPacket(reconfigPacket);
            return NodeStatus::RUNNING;
        }
    ));
    BTNode* phase3Sequence = new SequenceNode(phase3Branch);

    // -------------------------------------------------------
    // 4) Phase2 Branch (���� / �Ϲ� ����)
    //    - ����: �÷��̾���� �Ÿ��� 20m �����ΰ�? (��: distanceToPlayer <= 20.0f)
    //    - �׼�: ���� �� ���� ����
    //         - ����: (distanceToPlayer > attackRange)�� ���, ���� ��
    //         - ����: (distanceToPlayer <= attackRange)�� ���, ���� ����
    //    - �۽��ؾ� �� ����:
    //         boss_id, current_phase: ATTACK, current_hp, max_hp, distance_to_player,
    //         target_player_id, move_target_position
    // -------------------------------------------------------
    std::vector<BTNode*> phase2Branch;
    phase2Branch.push_back(new ConditionNode(
        [&context]() -> bool {
            const float phase2Distance = 20.0f; // ���� �Ӱ� ��
            return (context.distanceToPlayer <= phase2Distance);
        }
    ));
    phase2Branch.push_back(new ActionNode(
        [&context]() -> NodeStatus {
            if (context.distanceToPlayer > context.attackRange) {
                // �÷��̾�� ���� ��: �Ÿ� ����
                context.distanceToPlayer -= context.moveSpeed;
                // �۽�: ATTACK ����(�̵� ��) ��Ŷ ����
                // ��: ServerSendPacket(attackMovingPacket);
                return NodeStatus::RUNNING;
            }
            else {
                // ���� ���� ���� �����Ͽ� ���� ����
                // �۽�: ATTACK ����(���� ����) ��Ŷ ����
                // ��: ServerSendPacket(attackPacket);
                return NodeStatus::SUCCESS;
            }
        }
    ));
    BTNode* phase2Sequence = new SequenceNode(phase2Branch);

    // -------------------------------------------------------
    // 5) Phase1 Branch (Idle)
    //    - �׼�: ���� ��� ���� ���� (�÷��̾���� �Ÿ��� �� ���)
    //    - �۽��ؾ� �� ����:
    //         boss_id, current_phase: IDLE, current_hp, max_hp, distance_to_player,
    //         target_player_id, move_target_position
    // -------------------------------------------------------
    BTNode* phase1Action = new ActionNode(
        [&context]() -> NodeStatus {
            // �۽�: IDLE ���� ��Ŷ ����
            // ��: ServerSendPacket(idlePacket);
            return NodeStatus::SUCCESS;
        }
    );

    // -------------------------------------------------------
    // Root Selector: �켱���� ������ Branch�� �˻� (Death > Phase4 > Phase3 > Phase2 > Phase1)
    // -------------------------------------------------------
    std::vector<BTNode*> rootChildren;
    rootChildren.push_back(deathSequence);
    rootChildren.push_back(phase4Sequence);
    rootChildren.push_back(phase3Sequence);
    rootChildren.push_back(phase2Sequence);
    rootChildren.push_back(phase1Action);

    return new SelectorNode(rootChildren);
}
