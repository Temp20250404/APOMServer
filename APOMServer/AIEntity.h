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
        // 전달받은 builder 함수를 사용하여 행동 트리 생성
        m_behaviorTree = builder(m_context);
    }

    ~AIEntity() {
        delete m_behaviorTree;
    }

    // 매 틱마다 Behavior Tree 업데이트와 시뮬레이션용 데미지 처리를 진행
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