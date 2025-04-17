#pragma once

#include "ProtoStruct.h"
#include "Protobuf/Protocol.pb.h"

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
    float detectionRange;   // �÷��̾� ���� ����. ���� ���� ���� ���� �÷��̾ �ִٸ� �ش� �÷��̾ Ÿ����


    // �߰� ����: �� �� Ÿ�� �÷��̾� ����
    CRoom* ptargetRoom = nullptr;      // ���� AI�� ���� �� (�� ������ ���� �� ���� �÷��̾� ��� �� ����)
    CPlayer* pTargetPlayer = nullptr;    // ���� AI�� Ÿ������ �÷��̾��� ����


    // ��ġ ����
    Position currentPos{ 0.f, 0.f, 0.f };   // ���� AI(����)�� ��ġ (���� ��ǥ)
    Position targetPos{ 0.f, 0.f, 0.f };    // ������ �̵� ��ǥ ��ġ

    // ���� ����
    game::BOSS_STATE eCurState = game::BOSS_STATE::BOSS_STATE_IDLE;
};
