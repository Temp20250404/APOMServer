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
    Position bossPos = { context.posX, context.posY, context.posZ };
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
            float initTime = context.dieAnimTime;   // �ִϸ��̼� ���� �ð�(��)

            // phaseTimer�� �ʱ�ȭ���� �ʾҴٸ� �ʱ�ȭ
            if (context.phaseTimer <= 0)
                context.phaseTimer = initTime;

            // �ִϸ��̼��� �� �����ϴ� ���, �� phaseTimer�� �ʱⰪ�� ���� ��
            if (context.phaseTimer == initTime)
            {
                // ��� ��Ŷ ����: ������ ��� ���¸� Ŭ���̾�Ʈ�� �˸�
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    bossPos, bossPos, game::BOSS_STATE_DIE, 0);
            }

            // �ִϸ��̼� ���� ���̸� phaseTimer ���� ���ҽ�Ű�� RUNNING ���� ��ȯ
            if (context.phaseTimer > 0)
            {
                context.phaseTimer -= context.deltaTime;
                return NodeStatus::RUNNING;
            }

            // �ִϸ��̼� ������ �Ϸ�� ���, ������ ��� ���·� ��ȯ
            context.bDeath = true;
            // ���� ���¸� Ŭ���̾�Ʈ�� ���� (�߰� �������� �ʾƵ� ������ ���� ����ȭ�� ���� �ٽ� �ѹ� ����)
            SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                context.currentHP, context.maxHP,
                bossPos, bossPos, game::BOSS_STATE_DIE, 0);
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
            float initTime = context.skill3AnimTime;
            if (context.phaseTimer <= 0)
                context.phaseTimer = initTime;

            if (context.phaseTimer == initTime)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    bossPos, bossPos, game::BOSS_STATE_SKILL3, 0);
            }

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
            float initTime = context.skill2AnimTime;
            if (context.phaseTimer <= 0)
                context.phaseTimer = initTime;

            if (context.phaseTimer == initTime)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    bossPos, bossPos, game::BOSS_STATE_SKILL2, 0);
            }

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
            float initTime = context.skill1AnimTime;
            if (context.phaseTimer <= 0)
                context.phaseTimer = initTime;

            if (context.phaseTimer == initTime)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    bossPos, bossPos, game::BOSS_STATE_SKILL1, 0);
            }

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
            return context.hasTargetPlayer && (context.playerDistance <= context.attackRange);
            })
    );

    attackBranch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {
            // Ÿ�� �÷��̾� ��ġ�� ���. Ÿ���� ������ ���� ��ġ�� �⺻������ ���
            Position targetPos = bossPos;
            if (context.pTargetPlayer)
            {
                float tx, ty, tz;
                context.pTargetPlayer->getPosition(tx, ty, tz);
                targetPos = { tx, ty, tz };
            }

            float initTime = context.attackAnimTime;
            if (context.phaseTimer <= 0)
                context.phaseTimer = initTime;

            if (context.phaseTimer == initTime)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    targetPos, bossPos, game::BOSS_STATE_ATTACK, 0);
            }

            if (context.phaseTimer > 0)
            {
                context.phaseTimer -= context.deltaTime;
                return NodeStatus::RUNNING;
            }

            SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                context.currentHP, context.maxHP,
                targetPos, bossPos, game::BOSS_STATE_ATTACK, 0);
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
            return context.hasTargetPlayer && (context.playerDistance > context.attackRange);
            })
    );

    chaseBranch.push_back(
        new ActionNode([&context, bossPos]() -> NodeStatus {
            Position targetPos = bossPos;
            if (context.pTargetPlayer)
            {
                float tx, ty, tz;
                context.pTargetPlayer->getPosition(tx, ty, tz);
                targetPos = { tx, ty, tz };
            }

            // ������ Ÿ���� ���� �����ϸ鼭, �÷��̾���� �Ÿ��� ���ҽ�Ŵ
            context.playerDistance -= context.moveSpeed;

            float initSpeed = context.moveSpeed;
            // ���� �ִϸ��̼��� ���� �������� ��Ŷ ����
            if (context.phaseTimer <= 0)
                context.phaseTimer = context.attackAnimTime;  // �ӽ÷� attackAnimTime ���

            if (context.phaseTimer == context.attackAnimTime)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    targetPos, bossPos, game::BOSS_STATE_CHASE, initSpeed);
            }

            if (context.playerDistance <= context.attackRange)
                return NodeStatus::SUCCESS;

            if (context.phaseTimer > 0)
            {
                context.phaseTimer -= context.deltaTime;
                return NodeStatus::RUNNING;
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
            context.idleTime = context.idleResetTime;
            SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                context.currentHP, context.maxHP,
                bossPos, bossPos, game::BOSS_STATE_WALK, 0);
            return NodeStatus::SUCCESS;
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
            if (context.idleTime == context.idleResetTime)
            {
                SC_BOSS_PHASE_FOR_AROUND(nullptr, context.ptargetRoom, context.ID,
                    context.currentHP, context.maxHP,
                    bossPos, bossPos, game::BOSS_STATE_IDLE, 0);
            }
            context.idleTime -= context.deltaTime;
            return NodeStatus::RUNNING;
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
