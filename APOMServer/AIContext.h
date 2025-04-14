#pragma once

class CRoom;
class CPlayer;

// �𸮾󿡼� blackboard�� �Ҹ��� AI ������ ��Ʈ���ϴ� ����ü
struct AIContext {
    // ���� ID
    UINT32 ID = -1;

    // ü�� ����
    float currentHP = 100.0f;    // ���� ü��
    float maxHP = 100.0f;        // �ִ� ü��

    bool bDeath = false;         // ü���� 0 ���ϰ� �Ǹ� true

    // �̵�/���� ����
    float moveSpeed = 1.0f;      // ���� �̵� �ӵ�
    float baseDamage = 10.0f;    // �⺻ ���ݷ�
    float attackRange = 5.0f;    // ���� �Ÿ�

    // ������ ��ȯ ����
    float phase2Distance = 20.0f;  // ������2�� ��ȯ�ϴ� �Ÿ�(20m ����)
    float phase3HPPercent = 0.8f;  // ü���� 80% ���ϰ� �Ǹ� ������3
    float phase4HPPercent = 0.5f;  // ü���� 50% ���ϰ� �Ǹ� ������4

    float detectionRange = 15.0f; // �÷��̾� ���� ����

    CRoom* ptargetRoom = nullptr;   // ���� AI�� �����ϴ� �濡 ���� ����
    CPlayer* pTargetPlayer = nullptr;   // ���� AI�� Ÿ������ �÷��̾��� ����

    float posX = 0.f;     // ������ ���� ��ġ
    float posY = 0.f;
    float posZ = 0.f;
};