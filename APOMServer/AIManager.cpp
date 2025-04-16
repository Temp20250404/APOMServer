#include "AIManager.h"

CAIManager::CAIManager() noexcept
{
}

CAIManager::~CAIManager() {
    for (AIEntity* entity : m_entities) {
        delete entity;
    }

    m_entities.clear();
    m_idToIndex.clear();
}

// ���ο� AI�� �߰��ϰ�, id�� �ε����� ���� �ε����� ���
void CAIManager::AddEntity(AIEntity* entity) {
    UINT32 id = entity->GetID();

    m_entities.push_back(entity);
    m_idToIndex[id] = m_entities.size() - 1;
}

// id�� ���� AIEntity�� ������ ã�� �Լ�
AIEntity* CAIManager::FindEntityByID(UINT32 id) {
    auto it = m_idToIndex.find(id);
    if (it != m_idToIndex.end()) {
        size_t index = it->second;
        return m_entities[index];
    }

    return nullptr;
}

// ��� AI�� ������Ʈ�ϰ�, ���� AI�� swap-and-pop ������� ����Ʈ���� ����
void CAIManager::UpdateAll() {
    for (size_t i = 0; i < m_entities.size(); ) {
        AIEntity* ai = m_entities[i];
        ai->Update();

        if (!ai->IsAlive()) {
            std::cout << "[AIManager] Removing dead AI (ID: " << ai->GetID() << ")\n";

            UINT32 deadId = ai->GetID();
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
