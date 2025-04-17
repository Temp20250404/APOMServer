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
���� [Sequence] DIE Branch
��       ���� [Condition] if currentHP <= 0
��       ���� [Action] SC_BOSS_PHASE ȣ��. ��� �ִϸ��̼� ��� �ð����� RUNNING ���� ����. ���� bDeath�� true�� ����

���� [Sequence] SKILL3 Branch
��       ���� [Condition] if currentHP / maxHP <= 0.2  // ��ü ü���� 20% ������ ���
��       ���� [Action] SC_BOSS_PHASE ȣ��. 1ȸ�� ȣ��

���� [Sequence] SKILL2 Branch
��       ���� [Condition] if currentHP / maxHP <= 0.5  // ��ü ü���� 50% ������ ���
��       ���� [Action] SC_BOSS_PHASE ȣ��. 1ȸ�� ȣ��

���� [Sequence] SKILL1 Branch
��       ���� [Condition] if currentHP / maxHP <= 0.8  // ��ü ü���� 80% ������ ���
��       ���� [Action] SC_BOSS_PHASE ȣ��. 1ȸ�� ȣ��

���� [Sequence] ATTACK Branch
��       ���� [Condition] if �÷��̾ ���� �Ÿ� ���� ���� ���
��       ���� [Action] ����. ���� �ִϸ��̼� ����ð� ������ RUNNING ���� ����

���� [Sequence] CHASE Branch
��       ���� [Condition] if Ÿ�� �÷��̾ ������ ���, ���� ���� �Ÿ����� �Ÿ��� �� ���
��       ���� [Action] �̵��ӵ��� �°�, �ش� �÷��̾� ��ġ�� �̵�

���� [Sequence] WALK Branch
��       ���� [Condition] if Ÿ�� �÷��̾ ���� ���, IDLE TIME�� 0 ������ ���
��       ���� [Action] ��ǥ ��ġ�� �̵�. ��ǥ ��ġ�� �̵��ϸ� IDLE TIME = 3�� ����

���� [Action] IDLE
��       ���� [Condition] IDLE TIME�� 0 �̻��� ���
        ���� [Action] IDLE TIME�� 0�� �Ǹ� ���ο� ��ǥ ��ġ�� ����
*/

BTNode* CreateBossBT(AIContext& context)
{
    // ==========================================================================  
    // [����: updateTarget �˻�]  
    // = �� ������ Ÿ�� ������Ʈ�� ������ ó���ǹǷ�,  
    // = �ܼ��� pTargetPlayer�� nullptr�� �ƴ����� �˻��Ͽ� Ÿ���� ���� ���θ� �Ǵ���  
    // ==========================================================================  
    auto hasValidTarget = [&context]() -> bool {
        return (context.pTargetPlayer != nullptr);
        };
    // ==========================================================================  

    // ==========================================================================  
    // ������ ���� ��ġ�� Position �������� ����  
    // = ������ ��ġ�� context.posX, posY, posZ���� ������  
    // = ���� ���� �ܰ迡�� ������ ��ġ(bossPos)�� ��Ŷ ���� ���ڷ� �����  
    // ==========================================================================  
    Position bossPos = context.currentPos;
    // ==========================================================================  

    // ==========================================================================  
    // [DIE Branch ����] : ��� �귣ġ  
    // = ������ ����ϴ� ��츦 ó���ϴ� �б�  
    // = ����: currentHP�� 0�̸� ������ ����ߴٰ� �Ǵ�  
    // = �׼�: ��� �ִϸ��̼�(dieAnimTime) ���� phaseTimer�� ���� �ִϸ��̼� ����� �����ϸ�,  
    // =         ���� �ִϸ��̼� ���� �� ��Ŷ ����(SC_BOSS_PHASE_FOR_AROUND ȣ��)  
    // =         �ִϸ��̼� ���� �� bDeath�� true�� �����ϰ�, ���� ���¸� ��Ŷ���� ������  
    // ==========================================================================  
    std::vector<BTNode*> dieBranch;

    // ���� ���: currentHP�� 0�̸� SUCCESS, �׷��� ������ FAILURE ��ȯ
    dieBranch.push_back(
        new ConditionNode([&context]() -> bool {
            return context.currentHP == 0;
            })
    );

    // �׼� ���: ��� �ִϸ��̼� ���� �� ��Ŷ ����  
    dieBranch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {

            if (context.eCurState != game::BOSS_STATE_DIE)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_DIE, 0);

                context.eCurState = game::BOSS_STATE_DIE;
            }

            float initTime = context.dieAnimTime;   // �ִϸ��̼� ���� �ð�(��)

            // phaseTimer�� �ʱ�ȭ���� �ʾҴٸ� �ʱ�ȭ
            if (context.phaseTimer <= 0)
                context.phaseTimer = initTime;

            // �ִϸ��̼� ���� ���̸� phaseTimer ���� ���ҽ�Ű�� RUNNING ���� ��ȯ
            if (context.phaseTimer > 0)
            {
                context.phaseTimer -= context.deltaTime;
                return NodeStatus::RUNNING;
            }

            // �ִϸ��̼� ������ �Ϸ�� ���, ������ ��� ���·� ��ȯ
            context.bDeath = true;

            return NodeStatus::SUCCESS;
            })
    );

    BTNode* dieSequence = new SequenceNode(dieBranch);
    // ==========================================================================  

    // ==========================================================================  
    // [SKILL3 Branch ����] : ��ų3 �ߵ� �귣ġ  
    // = ������ HP�� ��ü�� 20% �����̸� ��ų3�� ������� ���� ��� ó��  
    // = �׼�: skill3AnimTime ���� ��ų3 �ִϸ��̼��� �����ϰ�,  
    // =         �ִϸ��̼� ���� �ÿ��� ��Ŷ �����ϸ�,  
    // =         �ִϸ��̼��� ������ usedSkill3�� true�� ������  
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
    // [SKILL2 Branch ����] : ��ų2 �ߵ� �귣ġ  
    // = ������ HP�� ��ü�� 50% �����̸� ��ų2�� ������� ���� ��� ó��  
    // = �׼�: skill2AnimTime ���� ��ų2 �ִϸ��̼��� �����ϰ�,  
    // =         �ִϸ��̼� ���� �ÿ��� ��Ŷ �����ϸ�,  
    // =         �ִϸ��̼� ���� �� usedSkill2�� true�� ������  
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
    // [SKILL1 Branch ����] : ��ų1 �ߵ� �귣ġ  
    // = ������ HP�� ��ü�� 80% �����̸� ��ų1�� ������� ���� ��� ó��  
    // = �׼�: skill1AnimTime ���� ��ų1 �ִϸ��̼��� �����ϰ�,  
    // =         �ִϸ��̼� ���� �ÿ��� ��Ŷ �����ϸ�,  
    // =         �ִϸ��̼� ���� �� usedSkill1�� true�� ������  
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
    // [ATTACK Branch ����] : ���� �귣ġ  
    // = ����: Ÿ�� �÷��̾ �����ϸ�, �������� �Ÿ��� ���� ���� ������ ���  
    // = �׼�: attackAnimTime ���� ���� �ִϸ��̼��� �����ϸ�,  
    //         Ÿ�� �÷��̾��� ��ġ(targetMovementPos)�� ����(��Ŷ ������ �ִϸ��̼� ���� �ÿ��� ����)  
    // ==========================================================================  
    std::vector<BTNode*> attackBranch;

    attackBranch.push_back(
        new ConditionNode([&context]() -> bool {
            // Ÿ�� �÷��̾ �����ϰ�, �ش� �÷��̾���� �Ÿ��� ���� �������� ������ ��� 
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
                std::cout << "���� ���� " << context.phaseTimer << "�� ��ŭ ���� �ִϸ��̼� ���� ����\n";
            }

            if (context.phaseTimer > 0)
            {
                context.phaseTimer -= context.deltaTime;
                std::cout << "���� �ִϸ��̼� ������. ���� ���� �ð� : " << context.phaseTimer << "\n";
                return NodeStatus::RUNNING;
            }

            return NodeStatus::SUCCESS;
            })
    );

    BTNode* attackSequence = new SequenceNode(attackBranch);
    // ==========================================================================  

    // ==========================================================================  
    // [CHASE Branch ����] : ���� �귣ġ  
    // = ����: Ÿ�� �÷��̾ ����������, �������� �Ÿ��� ���� �������� Ŭ ���  
    // = �׼�: ������ �÷��̾ ���� �̵��ϸ�, playerDistance�� ���ҽ�Ű��,  
    //         �̵� �ӵ�(context.moveSpeed)�� ���� (��Ŷ ������ �ִϸ��̼� ���� �ÿ��� ����, bossState: BOSS_STATE_CHASE)  
    // ==========================================================================  
    std::vector<BTNode*> chaseBranch;

    chaseBranch.push_back(
        new ConditionNode([&context]() -> bool {
            // Ÿ�� �÷��̾ �����ϰ�, �ش� �÷��̾���� �Ÿ��� ���� �������� ������ ��� 
            return context.hasTargetPlayer && (context.playerDistance <= context.detectionRange);
            })
    );

    chaseBranch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {

            if (context.eCurState != game::BOSS_STATE_CHASE)
            {
                std::cout << "Ÿ�� �÷��̾� ������, Chase ��� �۵�\n";

                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_CHASE, 0);

                context.eCurState = game::BOSS_STATE_CHASE;
            }
            else
            {
                std::cout << "Chase ��... ���ο� ��ġ�� �����ؼ� �i�ư��� ��\n";

                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_CHASE, 0);
            }

            // --- �̵� ���� �߰� �κ� ---
            // 1) ��ǥ ���� ���
            Position targetPos = context.targetPos;
            float dx = targetPos.posX - context.currentPos.posX;
            float dy = targetPos.posY - context.currentPos.posY;
            float dz = targetPos.posZ - context.currentPos.posZ;

            // 2) �Ÿ� ���
            float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
            context.playerDistance = distance;  // �Ÿ� ������Ʈ

            // 3) ���� ������ ������ �Ϸ�
            if (distance <= context.attackRange)
            {
                std::cout << "Chase �Ϸ�. ���� ���� ���� �÷��̾� ����\n";
                return NodeStatus::SUCCESS;
            }

            // 4) �̵�
            float moveDistance = context.moveSpeed;
            if (moveDistance >= distance)
            {
                // ��ǥ�� �ٷ� ����
                context.currentPos = targetPos;
            }
            else
            {
                // ���� ����ȭ �� �̵�
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
    // [WALK Branch ����] : �̵� �귣ġ  
    // = ����: Ÿ�� �÷��̾ ����, idleTime�� 0 ������ ���
    // = �׼�: ���ο� ��ǥ ��ġ�� �̵��ߴٰ� �����ϰ�, idleTime�� idleResetTime���� �缳���ϸ� ��Ŷ ���� (bossState: BOSS_STATE_WALK)  
    // ==========================================================================  
    std::vector<BTNode*> walkBranch;

    walkBranch.push_back(
        new ConditionNode([&context]() -> bool {
            return (!context.hasTargetPlayer) && (context.idleTime <= 0);
            })
    );

    walkBranch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {

            // Ÿ�� �÷��̾ �����Ѵٸ�
            if (context.hasTargetPlayer)
            {
                std::cout << "WALK �߰��� Ÿ�� �÷��̾� �߰�\n";
                return NodeStatus::FAILURE;
            }

            if (context.eCurState != game::BOSS_STATE_WALK)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    context.targetPos, context.currentPos, game::BOSS_STATE_WALK, 0);

                context.eCurState = game::BOSS_STATE_WALK;
            }

            // ���� ��ġ�� ��ǥ ��ġ ������ ���� ���� ���
            float dx = context.targetPos.posX - context.currentPos.posX;
            float dy = context.targetPos.posY - context.currentPos.posY;
            float dz = context.targetPos.posZ - context.currentPos.posZ;

            // �Ÿ� ��� (Euclidean Distance)
            float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

            // �̹� �����߰ų�, �ſ� ����� ��� �̵����� ����
            if (distance < 0.001f) {
                // ���� ó��
                // ��ǥ ��ġ�� ���������� ��� idle ���·� ��ȯ�ϱ� ���� context.idleTime�� context.idleResetTime�� ����
                context.idleTime = context.idleResetTime;
                
                std::cout << "��ǥ ��ġ ����\n";

                return NodeStatus::SUCCESS;
            }

            // ���� ���� ����ȭ
            float dirX = dx / distance;
            float dirY = dy / distance;
            float dirZ = dz / distance;

            // ������ �ֱ⸦ ����� �̵� �Ÿ�
            float moveDistance = context.moveSpeed;  // ���� 25�����ӿ� 1ȸ �����ϴ� ��

            // ���� ��ǥ������ �Ÿ��� moveDistance���� �۴ٸ�, ��ǥ ������ ����
            if (moveDistance >= distance) {
                context.currentPos = context.targetPos;

                std::cout << "���� ��ġ : " << context.currentPos.posX << ", " << context.currentPos.posY << ", " << context.currentPos.posZ << "��ó ������\n";
            }
            else {
                // ���� * �̵� �Ÿ� ��ŭ �̵�
                context.currentPos.posX += dirX * moveDistance;
                context.currentPos.posY += dirY * moveDistance;
                context.currentPos.posZ += dirZ * moveDistance;

                std::cout << "���� ��ġ : " << context.currentPos.posX << ", " << context.currentPos.posY << ", " << context.currentPos.posZ << "\n";
            }

            return NodeStatus::RUNNING;
            })
    );

    BTNode* walkSequence = new SequenceNode(walkBranch);
    // ==========================================================================  

    // ==========================================================================  
    // [IDLE Action ����] : ��� �귣ġ (Fallback)  
    // = ����: idleTime�� 0 �̻��̸� Idle ���� �����ϸ� idleTime�� ���ҽ�Ŵ  
    // = idleTime�� 0 ���ϰ� �Ǹ� SUCCESS ��ȯ�Ͽ� ���ο� ��ǥ ��ġ ���� ��ȸ ���� (bossState: BOSS_STATE_IDLE)  
    // ==========================================================================  
    BTNode* idleAction = new ActionNode([&context, bossPos]() -> NodeStatus {
        if (context.idleTime > 0)
        {
            // Idle �ִϸ��̼��� ���۵� ���� ��Ŷ ���� (idleTime�� idleResetTime�� ���� ��)
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
        // �ð��� �� �������Ƿ� ������ ���ο� ��ġ�� ��ǥ�� �����̵��� �Ѵ�.
        else
        {
            float randomMoveX = RandomRange(3.f, 4.f);
            float randomMoveZ = RandomRange(3.f, 4.f);

            context.targetPos.posX += randomMoveX;
            context.targetPos.posZ += randomMoveZ;

            std::cout << "���ο� ��ǥ ��ġ ����\n";
        }

        return NodeStatus::SUCCESS;
        });
    // ==========================================================================  

    // ==========================================================================  
    // [Root Selector ����] : ��ü Behavior Tree�� ��Ʈ ���  
    // = �켱����: DIE �� SKILL3 �� SKILL2 �� SKILL1 �� ATTACK �� CHASE �� WALK �� IDLE  
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
