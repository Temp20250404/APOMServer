#pragma once

#include "pch.h"

// 노드 실행 상태 열거형
enum class NodeStatus { RUNNING, SUCCESS, FAILURE };

// BT 노드 기본 클래스
class BTNode {
public:
    virtual ~BTNode() {}
    virtual NodeStatus Tick() = 0;
    virtual void Initialize() {}
};

// 시퀀스 노드: 자식 노드를 순차적으로 실행
class SequenceNode : public BTNode {
public:
    SequenceNode(const std::vector<BTNode*>& children)
        : m_children(children), m_currentChildIndex(0) {}

    virtual ~SequenceNode() {
        for (BTNode* child : m_children) {
            delete child;
        }
        m_children.clear();
    }

    virtual void Initialize() override {
        m_currentChildIndex = 0;
        for (BTNode* child : m_children) {
            child->Initialize();
        }
    }

    virtual NodeStatus Tick() override {
        while (m_currentChildIndex < m_children.size()) {
            NodeStatus status = m_children[m_currentChildIndex]->Tick();
            if (status == NodeStatus::RUNNING)
                return NodeStatus::RUNNING;
            else if (status == NodeStatus::FAILURE)
                return NodeStatus::FAILURE;
            m_currentChildIndex++;
        }
        return NodeStatus::SUCCESS;
    }

private:
    std::vector<BTNode*> m_children;
    size_t m_currentChildIndex;
};

// 설렉터 노드: 자식 노드를 차례대로 실행하여, 하나라도 성공하면 성공을 반환
class SelectorNode : public BTNode {
public:
    SelectorNode(const std::vector<BTNode*>& children)
        : m_children(children), m_currentChildIndex(0) {}

    virtual ~SelectorNode() {
        for (BTNode* child : m_children) {
            delete child;
        }
        m_children.clear();
    }

    virtual void Initialize() override {
        m_currentChildIndex = 0;
        for (BTNode* child : m_children) {
            child->Initialize();
        }
    }

    virtual NodeStatus Tick() override {
        while (m_currentChildIndex < m_children.size()) {
            NodeStatus status = m_children[m_currentChildIndex]->Tick();
            if (status == NodeStatus::RUNNING)
                return NodeStatus::RUNNING;
            else if (status == NodeStatus::SUCCESS)
                return NodeStatus::SUCCESS;
            m_currentChildIndex++;
        }
        return NodeStatus::FAILURE;
    }

private:
    std::vector<BTNode*> m_children;
    size_t m_currentChildIndex;
};

// 조건 노드: 조건 함수의 결과에 따라 성공 또는 실패를 반환
class ConditionNode : public BTNode {
public:
    explicit ConditionNode(std::function<bool()> conditionFunc)
        : m_conditionFunc(conditionFunc) {}

    virtual NodeStatus Tick() override {
        bool result = m_conditionFunc();
        return result ? NodeStatus::SUCCESS : NodeStatus::FAILURE;
    }
private:
    std::function<bool()> m_conditionFunc;
};

// 행동 노드: 실제 행동을 담당하는 노드
class ActionNode : public BTNode {
public:
    explicit ActionNode(std::function<NodeStatus()> actionFunc)
        : m_actionFunc(actionFunc) {}

    virtual NodeStatus Tick() override {
        return m_actionFunc();
    }
private:
    std::function<NodeStatus()> m_actionFunc;
};

// 동적 액션 노드: 매 Tick마다 조건을 확인하여 기본 액션과 대체 액션 중 하나를 실행
// 이 노드를 사용하면 액션 실행 도중 조건에 따라 즉시 다른 행동으로 전환할 수 있음
class DynamicActionNode : public BTNode {
public:
    // @param conditionFunc: 조건 확인 함수 (true이면 대체 액션 실행)
    // @param primaryActionFunc: 기본 동작 액션 함수
    // @param alternativeActionFunc: 조건 충족 시 실행될 대체 액션 함수
    DynamicActionNode(std::function<bool()> conditionFunc,
        std::function<NodeStatus()> primaryActionFunc,
        std::function<NodeStatus()> alternativeActionFunc)
        : m_conditionFunc(conditionFunc),
        m_primaryActionFunc(primaryActionFunc),
        m_alternativeActionFunc(alternativeActionFunc) {}

    virtual NodeStatus Tick() override {
        // 매 Tick마다 조건을 확인하여, 조건이 충족되면 대체 액션을 실행
        if (m_conditionFunc()) {
            return m_alternativeActionFunc();
        }
        return m_primaryActionFunc();
    }

private:
    std::function<bool()> m_conditionFunc;
    std::function<NodeStatus()> m_primaryActionFunc;
    std::function<NodeStatus()> m_alternativeActionFunc;
};
