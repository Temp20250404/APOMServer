#pragma once

class CRoom;
class CPlayer;

// �𸮾󿡼� blackboard�� �Ҹ��� AI ������ ��Ʈ���ϴ� ����ü
struct AIContext {
    // ���� ID
    UINT32 ID = -1;  // ���� �Ǵ� AI�� ���� �ĺ���

    // HP ����
    UINT32 currentHP;    // ���� HP
    UINT32 maxHP;        // �ִ� HP
    bool bDeath;         // ��� ����

    // ��ų ��� ����
    bool usedSkill1;     // ��ų1 ��� ����
    bool usedSkill2;     // ��ų2 ��� ����
    bool usedSkill3;     // ��ų3 ��� ����

    // �ִϸ��̼� �ð� (��)
    float attackAnimTime;   // ���� �ִϸ��̼� ��� �ð�
    float dieAnimTime;      // ��� �ִϸ��̼� ��� �ð�
    float skill1AnimTime;   // ��ų1 �ִϸ��̼� ��� �ð�
    float skill2AnimTime;   // ��ų2 �ִϸ��̼� ��� �ð�
    float skill3AnimTime;   // ��ų3 �ִϸ��̼� ��� �ð�
    float phaseTimer;       // ���� ���� ���� �ִϸ��̼� Ÿ�̸�

    // Ÿ�� ����
    bool hasTargetPlayer;   // Ÿ�� �÷��̾� ���� ����
    float playerDistance;   // Ÿ�� �÷��̾���� �Ÿ�

    // Idle ����
    float idleTime;         // ��� ����(Idle) �ð�

    // ��� (�� Tick �� �ð� ��)
    float attackRange;      // ���� ����
    float moveSpeed;        // �̵� �ӵ� (�� Tick �� �����ϴ� �Ÿ�)
    float idleResetTime;    // �̵� �� Idle �ð� �缳�� ��
    float deltaTime;        // �� Tick �� �ð� (��)

    // �߰� ����: �� �� Ÿ�� �÷��̾� ����
    CRoom* ptargetRoom = nullptr;      // ���� AI�� ���� �� (�� ������ ���� �� ���� �÷��̾� ��� �� ����)
    CPlayer* pTargetPlayer = nullptr;    // ���� AI�� Ÿ������ �÷��̾��� ����

    // ���� AI(����)�� ��ġ (���� ��ǥ)
    float posX = 0.f;  // X ��ǥ
    float posY = 0.f;  // Y ��ǥ
    float posZ = 0.f;  // Z ��ǥ
};
