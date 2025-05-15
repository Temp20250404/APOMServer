#pragma once

#include "pch.h"
#include "AIContext.h"
#include "BehaviourTree.h"

class CPlayer;

using BTBuilder = std::function<BTNode* (AIContext&)>;

class AIEntity {
public:
    AIEntity(const AIContext& aiContext, BTBuilder builder);
    ~AIEntity();

    // �� ƽ���� Behavior Tree ������Ʈ�� �ùķ��̼ǿ� ������ ó���� ����
    void Update();

    bool IsAlive() const {
        return m_context.bDeath;
    }

    int GetID() const { return m_id; }

    const AIContext& GetContext(void) const {
        return m_context;
    }

public:
    int m_id;
    AIContext m_context;

    static UINT32 g_id;
};