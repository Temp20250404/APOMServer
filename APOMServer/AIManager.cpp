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

// 새로운 AI를 추가하고, id와 인덱스를 보조 인덱스에 등록
void CAIManager::AddEntity(AIEntity* entity) {
    UINT32 id = entity->GetID();

    m_entities.push_back(entity);
    m_idToIndex[id] = m_entities.size() - 1;
}

// id를 통해 AIEntity를 빠르게 찾는 함수
AIEntity* CAIManager::FindEntityByID(UINT32 id) {
    auto it = m_idToIndex.find(id);
    if (it != m_idToIndex.end()) {
        size_t index = it->second;
        return m_entities[index];
    }

    return nullptr;
}

// 모든 AI를 업데이트하고, 죽은 AI는 swap-and-pop 기법으로 리스트에서 제거
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
                // 마지막 요소를 현재 요소와 교환
                AIEntity* swappedEntity = m_entities.back();
                m_entities[i] = swappedEntity;

                // 교환된 요소의 index를 업데이트
                m_idToIndex[swappedEntity->GetID()] = i;
            }

            // 마지막 요소를 제거하고, 보조 인덱스에서도 제거
            m_entities.pop_back();
            m_idToIndex.erase(deadId);

            // 교환된 요소가 i번째에 들어왔으므로, i는 증가시키지 않음
        }
        else {
            ++i;
        }
    }
}
