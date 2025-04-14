#pragma once

#include "pch.h"
#include "Singleton.h"
#include "AIEntity.h"

// ���߿� UpdateAll�� ��Ƽ������� �� ����. �̰� �׽�Ʈ�ϸ鼭 [ ������ ����, AI ���� ]�� �������� ���� ������ ���� ���� ã�ƾ���.
class AIManager : public SingletonBase<AIManager> {
public:
    ~AIManager() {
        for (AIEntity* entity : m_entities) {
            delete entity;
        }
        m_entities.clear();
        m_idToIndex.clear();
    }

    // ���ο� AI�� �߰��ϰ�, id�� �ε����� ���� �ε����� ���
    void AddEntity(AIEntity* entity) {
        int id = entity->GetID();
        m_entities.push_back(entity);
        m_idToIndex[id] = m_entities.size() - 1;
    }

    // id�� ���� AIEntity�� ������ ã�� �Լ�
    AIEntity* FindEntityByID(int id) {
        auto it = m_idToIndex.find(id);
        if (it != m_idToIndex.end()) {
            size_t index = it->second;
            return m_entities[index];
        }
        return nullptr;
    }

    // ��� AI�� ������Ʈ�ϰ�, ���� AI�� swap-and-pop ������� ����Ʈ���� ����
    void UpdateAll() {
        for (size_t i = 0; i < m_entities.size(); ) {
            AIEntity* ai = m_entities[i];
            ai->Update();
            if (!ai->IsAlive()) {
                std::cout << "[AIManager] Removing dead AI (ID: " << ai->GetID() << ")\n";
                int deadId = ai->GetID();
                delete ai;

                size_t lastIndex = m_entities.size() - 1;
                if (i != lastIndex) {
                    // ������ ��Ҹ� ���� ��ҿ� ��ȯ
                    AIEntity* swappedEntity = m_entities.back();
                    m_entities[i] = swappedEntity;
                    // ��ȯ�� ����� index�� ������Ʈ
                    m_idToIndex[swappedEntity->GetID()] = i;
                }
                // ������ ��Ҹ� �����ϰ�, ���� �ε��������� ����
                m_entities.pop_back();
                m_idToIndex.erase(deadId);
                // ��ȯ�� ��Ұ� i��°�� �������Ƿ�, i�� ������Ű�� ����
            }
            else {
                ++i;
            }
        }
    }

private:
    std::vector<AIEntity*> m_entities;
    std::unordered_map<int, size_t> m_idToIndex;
};
