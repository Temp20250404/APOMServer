#pragma once

#include "pch.h"
#include "AIContext.h"
#include "BehaviourTree.h"

using BTBuilder = std::function<BTNode* (AIContext&)>;

class AIEntity {
public:
    AIEntity(const AIContext& aiContext, BTBuilder builder)
    {
        m_id = g_id++;
        m_context = aiContext;
        // ���޹��� builder �Լ��� ����Ͽ� �ൿ Ʈ�� ����
        m_behaviorTree = builder(m_context);
    }

    ~AIEntity() {
        delete m_behaviorTree;
    }

    // �� ƽ���� Behavior Tree ������Ʈ�� �ùķ��̼ǿ� ������ ó���� ����
    void Update() {
        NodeStatus status = m_behaviorTree->Tick();
        if (status != NodeStatus::RUNNING)
            m_behaviorTree->Initialize();
    }

    bool IsAlive() const {
        return (m_context.currentHP > 0);
    }

    int GetID() const { return m_id; }

private:
    int m_id;
    AIContext m_context;
    BTNode* m_behaviorTree;

    static UINT32 g_id;
};