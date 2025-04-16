#pragma once

#include "pch.h"
#include "Singleton.h"
#include "AIEntity.h"

// ���߿� UpdateAll�� ��Ƽ������� �� ����. �̰� �׽�Ʈ�ϸ鼭 [ ������ ����, AI ���� ]�� �������� ���� ������ ���� ���� ã�ƾ���.
class CAIManager : public SingletonBase<CAIManager> {
    friend class SingletonBase<CAIManager>;

public:
    explicit CAIManager() noexcept;
    ~CAIManager() noexcept;

    // ���� �����ڿ� ���� �����ڸ� �����Ͽ� ���� ����
    CAIManager(const CAIManager&) = delete;
    CAIManager& operator=(const CAIManager&) = delete;

public:
    // ���ο� AI�� �߰��ϰ�, id�� �ε����� ���� �ε����� ���
    void AddEntity(AIEntity* entity);

    // id�� ���� AIEntity�� ������ ã�� �Լ�
    AIEntity* FindEntityByID(UINT32 id);

    // ��� AI�� ������Ʈ�ϰ�, ���� AI�� swap-and-pop ������� ����Ʈ���� ����
    void UpdateAll();

private:
    std::vector<AIEntity*> m_entities;
    std::unordered_map<UINT32, size_t> m_idToIndex;
};