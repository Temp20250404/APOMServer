#pragma once

// �𸮾󿡼� blackboard�� �Ҹ��� AI ������ ��Ʈ���ϴ� ����ü
struct AIContext {
    // ü�� ����
    float currentHP = 100.0f;    // ���� ü��
    float maxHP = 100.0f;        // �ִ� ü��

    bool bDeath = false;         // ü���� 0 ���ϰ� �Ǹ� true

    // �÷��̾���� �Ÿ�
    float distanceToPlayer = 30.0f; // �ʱⰪ�� 30m�� ����

    // �̵�/���� ����
    float moveSpeed = 1.0f;      // ���� �̵� �ӵ�
    float baseDamage = 10.0f;    // �⺻ ���ݷ�
    float attackRange = 5.0f;    // ���� �Ÿ�

    // ������ ��ȯ ����
    float phase2Distance = 20.0f;  // ������2�� ��ȯ�ϴ� �Ÿ�(20m ����)
    float phase3HPPercent = 0.8f;  // ü���� 80% ���ϰ� �Ǹ� ������3
    float phase4HPPercent = 0.5f;  // ü���� 50% ���ϰ� �Ǹ� ������4

    float detectionRange = 15.0f; // �÷��̾� ���� ����
};