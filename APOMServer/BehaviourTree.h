#pragma once

#include "pch.h"

// ��� ���� ���� ������
enum class NodeStatus { RUNNING, SUCCESS, FAILURE };

// BT ��� �⺻ Ŭ����
class BTNode {
public:
    virtual ~BTNode() {}
    virtual NodeStatus Tick() = 0;
    virtual void Initialize() {}
};

// ������ ���: �ڽ� ��带 ���������� ����
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

// ������ ���: �ڽ� ��带 ���ʴ�� �����Ͽ�, �ϳ��� �����ϸ� ������ ��ȯ
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

// ���� ���: ���� �Լ��� ����� ���� ���� �Ǵ� ���и� ��ȯ
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

// �ൿ ���: ���� �ൿ�� ����ϴ� ���
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

// ���� �׼� ���: �� Tick���� ������ Ȯ���Ͽ� �⺻ �׼ǰ� ��ü �׼� �� �ϳ��� ����
// �� ��带 ����ϸ� �׼� ���� ���� ���ǿ� ���� ��� �ٸ� �ൿ���� ��ȯ�� �� ����
class DynamicActionNode : public BTNode {
public:
    // @param conditionFunc: ���� Ȯ�� �Լ� (true�̸� ��ü �׼� ����)
    // @param primaryActionFunc: �⺻ ���� �׼� �Լ�
    // @param alternativeActionFunc: ���� ���� �� ����� ��ü �׼� �Լ�
    DynamicActionNode(std::function<bool()> conditionFunc,
        std::function<NodeStatus()> primaryActionFunc,
        std::function<NodeStatus()> alternativeActionFunc)
        : m_conditionFunc(conditionFunc),
        m_primaryActionFunc(primaryActionFunc),
        m_alternativeActionFunc(alternativeActionFunc) {}

    virtual NodeStatus Tick() override {
        // �� Tick���� ������ Ȯ���Ͽ�, ������ �����Ǹ� ��ü �׼��� ����
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
