#pragma once

#include "ProtoStruct.h"
#include "Protobuf/Protocol.pb.h"

class CRoom;

// �𸮾󿡼� blackboard�� �Ҹ��� AI ������ ��Ʈ���ϴ� ����ü
struct AIContext {
    // ���� ID
    UINT32 ID = -1;  // ���� �Ǵ� AI�� ���� �ĺ���

    bool bDeath;         // ��� ����

    // �߰� ����: �� �� Ÿ�� �÷��̾� ����
    CRoom* ptargetRoom = nullptr;      // ���� AI�� ���� �� (�� ������ ���� �� ���� �÷��̾� ��� �� ����)
};
