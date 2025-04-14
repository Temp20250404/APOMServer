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

    // 매 틱마다 Behavior Tree 업데이트와 시뮬레이션용 데미지 처리를 진행
    void Update();

    bool IsAlive() const {
        return (m_context.currentHP > 0);
    }

    int GetID() const { return m_id; }

    void UpdateTarget();

private:
    int m_id;
    AIContext m_context;
    BTNode* m_behaviorTree;

    static UINT32 g_id;
};