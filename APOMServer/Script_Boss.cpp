#include "Script_Boss.h"

#include "BehaviourTree.h"
#include "AIContext.h"

#include "MakePacket.h"

#include "Room.h"

#include "MathUtils.h"

/*
Behavior Tree ������:
------------------------
Root (Selector)
���� [Sequence] Death Branch
��       ���� [Condition] ������ ��� �����ΰ�? (context.bDeath == true)
��       ���� [Action] ��� ó��: ��� �ִϸ��̼� ���� �� �����ϰ�, ��Ŷ �۽�
���� [Sequence] Phase4 Branch (����ȭ)
��       ���� [Condition] ���� ü���� �ִ� ü���� 50% �����ΰ�? (context.currentHP / context.maxHP <= phase4HPPercent)
��       ���� [Action] ����ȭ �ߵ�: ���ݷ°� �̵��ӵ� ���� �� ��Ŷ �۽�
���� [Sequence] Phase3 Branch (���� ������)
��       ���� [Condition] ���� ü���� �ִ� ü���� 80% �����ΰ�? (context.currentHP / context.maxHP <= phase3HPPercent)
��       ���� [DynamicAction] ������ �ִϸ��̼� ���� �� ��Ŷ �۽�
��                    - ���� Ÿ���� ������ų� �ߴ� ������ �����Ǹ� �ִϸ��̼� ���� ����
���� [Sequence] Phase2 Branch (����/�Ϲ� ����)
��       ���� [Condition] �÷��̾���� �Ÿ��� phase2Distance �����ΰ�? (���� �Ÿ� <= phase2Distance)
��       ���� [DynamicAction] ���� �� ���� ����:
��                    - ���� ��Ÿ����� �ָ� �÷��̾� ������ �����ϰ� ��Ŷ �۽� (RUNNING)
��                    - ���� ��Ÿ� ���� ���� �����ϰ� ��Ŷ �۽� (SUCCESS)
��                    - �߰��� Ÿ���� ������� �ִϸ��̼� �ߴ�
���� [Action] Phase1 Branch (���, Idle)
        ���� [Action] �� ���� ��Ȳ(Ÿ�� ���� ��)�� �� Idle ���� �����ϰ� ��Ŷ �۽�
*/

BTNode* CreateBossBT(AIContext& context)
{
    // ---------------------------
    // 1. Death Branch
    //    - ����: ������ ��� �������� Ȯ�� (context.bDeath)
    //    - �׼�: ��� �ִϸ��̼� ���� �� ���� ����, ���� ��Ŷ ����
    // ---------------------------
    std::vector<BTNode*> deathBranch;

    deathBranch.push_back(new ConditionNode([&context]() -> bool {
        // ������ ��� �����̸� true ��ȯ
        return context.bDeath;
        }));

    deathBranch.push_back(new ActionNode([&context]() -> NodeStatus {
        // ��� ó�� ����: Ÿ���� ���� ��� Ÿ�ٰ��� �Ÿ� ���
        float distance = 0.f;
        UINT32 targetID = 0;

        if (context.pTargetPlayer)
        {
            float px, py, pz;
            // Ÿ�� �÷��̾��� ��ġ�� ������
            context.pTargetPlayer->getPosition(px, py, pz);

            // ������ Ÿ�� �÷��̾� ������ ��Ŭ���� �Ÿ� ���
            distance = CalculateDistance(context.posX, context.posY, context.posZ, px, py, pz);

            targetID = context.pTargetPlayer->GetId();
        }

        // ������ ���� ��ġ�� Position ����ü�� ����
        Position currentPos = { context.posX, context.posY, context.posZ };

        // ��Ŷ ���� �Լ� ȣ��: ���� ��� ���� ����
        SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_DEATH,
            static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
            currentPos, targetID, distance);

        return NodeStatus::SUCCESS;
        }));

    BTNode* deathSequence = new SequenceNode(deathBranch);



    // ---------------------------
    // 2. Phase4 Branch (����ȭ / Berserk)
    //    - ����: ���� ü���� �ִ� ü���� 50% �������� Ȯ��
    //    - �׼�: ����ȭ �ߵ� (���ݷ� �� �̵��ӵ� ����) �� ��Ŷ ����
    // ---------------------------
    std::vector<BTNode*> phase4Branch;

    phase4Branch.push_back(new ConditionNode([&context]() -> bool {
        float hpPercent = context.currentHP / context.maxHP;
        return (hpPercent <= context.phase4HPPercent);  // ��: 0.5 �����̸� ����ȭ
        }));

    phase4Branch.push_back(new ActionNode([&context]() -> NodeStatus {
        // ����ȭ �ߵ�: ���ݷ°� �̵��ӵ��� 1.2��� ����
        context.baseDamage *= 1.2f;
        context.moveSpeed *= 1.2f;

        float distance = 0.f;
        UINT32 targetID = 0;

        // Ÿ�� �÷��̾ ���� ��� �Ÿ��� ���
        if (context.pTargetPlayer)
        {
            float px, py, pz;
            context.pTargetPlayer->getPosition(px, py, pz);
            distance = CalculateDistance(context.posX, context.posY, context.posZ, px, py, pz);
            targetID = context.pTargetPlayer->GetId();
        }

        Position currentPos = { context.posX, context.posY, context.posZ };

        // ����ȭ ���� ��Ŷ ����
        SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_BERSERK,
            static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
            currentPos, targetID, distance);

        return NodeStatus::SUCCESS;
        }));

    BTNode* phase4Sequence = new SequenceNode(phase4Branch);



    // ---------------------------
    // 3. Phase3 Branch (���� ������ / Reconfiguration)
    //    - ����: ���� ü���� �ִ� ü���� 80% �������� Ȯ��
    //    - �׼�: ������ �ִϸ��̼� ���� �� (�ִϸ��̼��� �Ϸ�� ������ RUNNING ���� ����)
    //            �߰��� Ÿ���� ���ų� �ߴ� ������ �߻��ϸ� �ִϸ��̼� �ߴ��ϰ� SUCCESS ��ȯ
    //    - DynamicActionNode ����Ͽ� �� Tick���� ���� üũ
    // ---------------------------
    std::vector<BTNode*> phase3Branch;

    phase3Branch.push_back(new ConditionNode([&context]() -> bool {
        float hpPercent = context.currentHP / context.maxHP;
        return (hpPercent <= context.phase3HPPercent);  // ��: 0.8 �����̸� ������ ����
        }));

    phase3Branch.push_back(new DynamicActionNode(
        // �ߴ� ����: Ÿ�� �÷��̾ ������ (�Ǵ� �ٸ� �ߴ� ���� �߰� ����)
        [&context]() -> bool {
            return (context.pTargetPlayer == nullptr);
        },

        // primaryActionFunc: ������ �ִϸ��̼� ���� (�ִϸ��̼��� ���� ���̸� ��� RUNNING)
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

            // ������ ���� ��Ŷ ����
            SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_RECONFIGURATION,
                static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
                currentPos, targetID, distance);

            // �ִϸ��̼��� �������̹Ƿ� ��� RUNNING ��ȯ
            return NodeStatus::RUNNING;
        },

        // alternativeActionFunc: �ߴ� ������ �����Ǹ� �ִϸ��̼� �ߴ��ϰ� SUCCESS ��ȯ
        [&context]() -> NodeStatus {
            return NodeStatus::SUCCESS;
        }
    ));

    BTNode* phase3Sequence = new SequenceNode(phase3Branch);



    // ---------------------------
    // 4. Phase2 Branch (����/�Ϲ� ���� / Attack)
    //    - ����: Ÿ�� �÷��̾ �����ϸ�, ������ �÷��̾� ���� �Ÿ��� phase2Distance �������� Ȯ��
    //    - �׼�: DynamicActionNode�� ����Ͽ�,
    //              - ���� ��Ÿ�(distance) �񱳿� ���� ���� �Ǵ� ���� ����
    //              - ���� ��Ÿ����� �ָ� �÷��̾� ������ �����ϰ� ��Ŷ ���� (RUNNING)
    //              - ���� ��Ÿ� ���� ���� ���� �����ϰ� ��Ŷ ���� (SUCCESS)
    //              - �߰��� Ÿ���� ������� �ִϸ��̼� �ߴ��ϰ� SUCCESS ��ȯ
    // ---------------------------
    std::vector<BTNode*> phase2Branch;

    phase2Branch.push_back(new ConditionNode([&context]() -> bool {
        // Ÿ�� �÷��̾ ������ ���� ������
        if (!context.pTargetPlayer)
            return false;

        float px, py, pz;
        context.pTargetPlayer->getPosition(px, py, pz);

        // ������ Ÿ�� �÷��̾� �� �Ÿ� ���
        float distance = CalculateDistance(context.posX, context.posY, context.posZ, px, py, pz);

        return (distance <= context.phase2Distance);  // ��: 20m �����̸� ���� ����
        }));

    phase2Branch.push_back(new DynamicActionNode(
        // �ߴ� ����: Ÿ�� �÷��̾ ������ (�Ǵ� �ٸ� �ߴ� ���� �߰� ����)
        [&context]() -> bool {
            return (context.pTargetPlayer == nullptr);
        },

        // primaryActionFunc: Ÿ���� �����ϸ�, ���� ��Ÿ�(distance) �񱳿� ���� ���� �Ǵ� ���� ����
        [&context]() -> NodeStatus {
            float px, py, pz;
            context.pTargetPlayer->getPosition(px, py, pz);

            // ������ Ÿ�� ������ �Ÿ� ���
            float distance = CalculateDistance(context.posX, context.posY, context.posZ, px, py, pz);

            UINT32 targetID = context.pTargetPlayer->GetId();

            if (distance > context.attackRange)
            {
                // ���� ��Ÿ����� �ָ� �÷��̾�� ���� (�̵�) ó��
                float dx = px - context.posX;
                float dy = py - context.posY;
                float dz = pz - context.posZ;

                float length = std::sqrt(dx * dx + dy * dy + dz * dz);

                if (length > 0.f)
                {
                    // ������ ��ġ�� moveSpeed�� ���� ���� (�÷��̾� �������� �̵�)
                    context.posX += (dx / length) * context.moveSpeed;
                    context.posY += (dy / length) * context.moveSpeed;
                    context.posZ += (dz / length) * context.moveSpeed;
                }

                Position currentPos = { context.posX, context.posY, context.posZ };

                // ���� �� ���� ��Ŷ ���� (ATTACK ����)
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_ATTACK,
                    static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
                    currentPos, targetID, distance);

                return NodeStatus::RUNNING;
            }
            else
            {
                // ���� ��Ÿ� ���� ������ ���� ���� ����
                Position currentPos = { context.posX, context.posY, context.posZ };

                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_ATTACK,
                    static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
                    currentPos, targetID, distance);

                return NodeStatus::SUCCESS;
            }
        },

        // alternativeActionFunc: Ÿ���� ������ �ٷ� SUCCESS ó��
        [&context]() -> NodeStatus {
            return NodeStatus::SUCCESS;
        }
    ));

    BTNode* phase2Sequence = new SequenceNode(phase2Branch);



    // ---------------------------
    // 5. Phase1 Branch (Idle)
    //    - �� ���� ��Ȳ(Ÿ�� ���� ��)�� �� Idle ���� �����ϸ�, ��Ŷ ����
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

        // Idle ���� ��Ŷ ����
        SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID, game::BOSS_PHASE_IDLE,
            static_cast<UINT32>(context.currentHP), static_cast<UINT32>(context.maxHP),
            currentPos, targetID, distance);

        return NodeStatus::SUCCESS;
        });



    // ---------------------------
    // 6. Root Selector ����
    //    - �켱���� (�������� ��): Death > Phase4 > Phase3 > Phase2 > Phase1
    // ---------------------------
    std::vector<BTNode*> rootChildren;

    rootChildren.push_back(deathSequence);
    rootChildren.push_back(phase4Sequence);
    rootChildren.push_back(phase3Sequence);
    rootChildren.push_back(phase2Sequence);
    rootChildren.push_back(phase1Action);



    // ���������� Root Selector ��带 �����Ͽ� ��ȯ
    return new SelectorNode(rootChildren);
}
