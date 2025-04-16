#pragma once

#include "pch.h"
#include "Singleton.h"
#include "AIEntity.h"

// 나중에 UpdateAll을 멀티스레드로 뺄 예정. 이건 테스트하면서 [ 스레드 갯수, AI 갯수 ]를 조건으로 가장 성능이 좋은 값을 찾아야함.
class CAIManager : public SingletonBase<CAIManager> {
    friend class SingletonBase<CAIManager>;

public:
    explicit CAIManager() noexcept;
    ~CAIManager() noexcept;

    // 복사 생성자와 대입 연산자를 삭제하여 복사 방지
    CAIManager(const CAIManager&) = delete;
    CAIManager& operator=(const CAIManager&) = delete;

public:
    // 새로운 AI를 추가하고, id와 인덱스를 보조 인덱스에 등록
    void AddEntity(AIEntity* entity);

    // id를 통해 AIEntity를 빠르게 찾는 함수
    AIEntity* FindEntityByID(UINT32 id);

    // 모든 AI를 업데이트하고, 죽은 AI는 swap-and-pop 기법으로 리스트에서 제거
    void UpdateAll();

private:
    std::vector<AIEntity*> m_entities;
    std::unordered_map<UINT32, size_t> m_idToIndex;
};