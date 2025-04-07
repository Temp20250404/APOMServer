#pragma once

#include <iostream>
#include <chrono>
#include <vector>
#include <new>
#include <Windows.h>

#define GUARD_VALUE 0xaaaabbbbccccdddd

// Node ����ü ����

#ifdef _DEBUG
#pragma pack(1)
#endif // _DEBUG

template<typename T>
struct Node
{
    // x64 ȯ�濡�� ����ɰ��� ����ؼ� UINT64 ���� ���
#ifdef _DEBUG

    UINT64 BUFFER_GUARD_FRONT;
    T data;
    UINT64 BUFFER_GUARD_END;
    ULONG_PTR POOL_INSTANCE_VALUE;
    Node* next;

#endif // _DEBUG
#ifndef _DEBUG

    T data;
    Node* next;

#endif // !_DEBUG
};

#ifdef _DEBUG
#endif // _DEBUG

// MemoryPool Ŭ���� ����
template<typename T, bool bPlacementNew>
class MemoryPool
{
public:
    // ������
    MemoryPool(UINT32 sizeInitialize = 0);

    // �Ҹ���
    virtual ~MemoryPool(void);

    // Ǯ�� �ִ� ��ü�� �Ѱ��ְų� ���� �Ҵ��� �ѱ�
    T* Alloc(void);

    // ��ü�� Ǯ�� ��ȯ
    bool Free(T* ptr);

    // �޸� Ǯ�� ��뷮 Ȯ��
    void PrintUsage() const;

private:
    Node<T>* m_freeNode;
    UINT32 m_countPool;         // Ǯ ��ü�� �Ҵ�� ��ü ��
    UINT32 m_allocatedCount;    // ���� Ǯ���� �Ҵ�� ��ü ��, 0����� Ǯ���� ���ǰ� �ִ� ��ü�� ���� ���̴�.
};

template<typename T, bool bPlacementNew>
inline MemoryPool<T, bPlacementNew>::MemoryPool(UINT32 sizeInitialize)
{
    m_freeNode = nullptr;
    m_countPool = 0;
    m_allocatedCount = 0;

    // �ʱ� �޸� ���� �غ�
    if (sizeInitialize > 0)
    {
        Node<T>* newNode;

        // ���ο� ��ü �Ҵ�
        newNode = (Node<T>*)malloc(sizeof(Node<T>));

#ifdef _DEBUG
        newNode->BUFFER_GUARD_FRONT = GUARD_VALUE;
        newNode->BUFFER_GUARD_END = GUARD_VALUE;

        newNode->POOL_INSTANCE_VALUE = reinterpret_cast<ULONG_PTR>(this);
#endif // _DEBUG

        // placement New �ɼ� ���ο� ������� ������ ������ ȣ��
        new (&(newNode->data)) T();     //new (reinterpret_cast<char*>(newNode) + offsetof(Node<T>, data)) T();

        // ó���� m_freeNode�� ��������Ƿ� newNode�� ����
        m_freeNode = newNode;


        for (UINT32 i = 1; i < sizeInitialize; i++)
        {
            // ���ο� ��ü �Ҵ�
            newNode = (Node<T>*)malloc(sizeof(Node<T>));

#ifdef _DEBUG
            newNode->BUFFER_GUARD_FRONT = GUARD_VALUE;
            newNode->BUFFER_GUARD_END = GUARD_VALUE;

            newNode->POOL_INSTANCE_VALUE = reinterpret_cast<ULONG_PTR>(this);
#endif // _DEBUG

            // placement New �ɼ� ���ο� ������� ������ ������ ȣ��
            new (&(newNode->data)) T();     //new (reinterpret_cast<char*>(newNode) + offsetof(Node<T>, data)) T();

            // m_freeNode�� ���� ���� ���� -> ��ġ stack�� ����ϵ��� ���
            newNode->next = m_freeNode;
            m_freeNode = newNode;
        }

        // Ǯ���� �����ϴ� ������Ʈ ���� �ʱ�ȭ
        m_countPool = sizeInitialize;
    }
}

template<typename T, bool bPlacementNew>
inline MemoryPool<T, bPlacementNew>::~MemoryPool(void)
{
    // ��� ��� ����
    Node<T>* deleteNode = m_freeNode;
    Node<T>* nextNode;
    while (deleteNode != nullptr)
    {
        nextNode = deleteNode->next;

        // bPlacementNew �ɼ��� ���� �ִ��� ������ Alloc���� �����ڸ� ȣ�����ش�.
        // �׷��� �޸� Ǯ �Ҹ�� bPlacementNew �ɼ��� �����ִٸ� �Ҹ��ڸ� ȣ��������Ѵ�. 
        // bPlacementNew �ɼ��� ������� ���� �� �����ڰ� ȣ��ȴٸ� �����Ҵ��� ���ο��� �� �� �ֱ� ������ �Ҹ��ڸ� ȣ���ؾ� �����Ҵ��� ���� ������ �� �ֱ� �����̴�.
        if constexpr (!bPlacementNew)
        {
            deleteNode->data.~T();
        }

        delete deleteNode;
        deleteNode = nextNode;
        m_countPool--;
    }

    // ���� �Ҵ� ������ ���� �Ϸ���� �ʾҴٸ�
    if (m_countPool != 0)
    {
        // ��... ������� ���߿� �����ڱ�.
    }
}

template<typename T, bool bPlacementNew>
inline T* MemoryPool<T, bPlacementNew>::Alloc(void)
{
    Node<T>* returnNode;

    // m_freeNode�� nullptr�� �ƴ϶�� ���� Ǯ�� ��ü�� �����Ѵٴ� �ǹ��̹Ƿ� �ϳ� �̾Ƽ� �Ѱ���
    if (m_freeNode != nullptr)
    {
        returnNode = m_freeNode;            // top
        m_freeNode = m_freeNode->next;      // pop

        // placement New �ɼ��� �����ִٸ� ������ ȣ��
        if constexpr (bPlacementNew)
        {
            //new (reinterpret_cast<char*>(returnNode) + offsetof(Node<T>, data)) T();
            new (&(returnNode->data)) T();
        }

        // �Ҵ�� ��ü �� ����
        m_allocatedCount++;

        // ��ü�� TŸ�� ������ ��ȯ
        return &returnNode->data;
    }

    // m_freeNode�� nullptr�̶�� Ǯ�� ��ü�� �������� �ʴ´ٴ� �ǹ��̹Ƿ� ���ο� ��ü �Ҵ�

    // �� ��� �Ҵ�
    Node<T>* newNode = (Node<T>*)malloc(sizeof(Node<T>));

#ifdef _DEBUG
    // ������ ����. ���� ���� Ȯ���ϰ�, ��ȯ�Ǵ� Ǯ�� ������ �ùٸ��� Ȯ���ϱ� ���� ���
    newNode->BUFFER_GUARD_FRONT = GUARD_VALUE;
    newNode->BUFFER_GUARD_END = GUARD_VALUE;

    newNode->POOL_INSTANCE_VALUE = reinterpret_cast<ULONG_PTR>(this);
#endif // _DEBUG

    newNode->next = nullptr;    // ��Ȯ�� m_freeNode�� �����ص� �ȴ�. �ٵ� �� ��ü�� nullptr�̴� ���� ���� nullptr ����.

    // bPlacementNew �ɼǰ� ������ ������ �����ڸ� ȣ������.
    new (&(newNode->data)) T();

    // Ǯ ������ 1 ����
    m_countPool++;

    // �Ҵ�� ��ü �� ����
    m_allocatedCount++;

    // ��ü�� TŸ�� ������ ��ȯ
    return reinterpret_cast<T*>(reinterpret_cast<char*>(newNode) + offsetof(Node<T>, data));
}

template<typename T, bool bPlacementNew>
inline bool MemoryPool<T, bPlacementNew>::Free(T* ptr)
{
#ifdef _DEBUG
    // ��ȯ�ϴ� ���� �������� �ʴ´ٸ�
    if (ptr == nullptr)
    {
        // ����
        return false;
    }
#endif // _DEBUG

    // offsetof(Node<T>, data)) => Node ����ü�� data ������ Node ����ü���� �ּ� ���̰��� ���
    // ���⼱ debug ����϶� ���尡 �����Ƿ� 4, release�� ��� 0���� ó��
    Node<T>* pNode = reinterpret_cast<Node<T>*>(reinterpret_cast<char*>(ptr) - offsetof(Node<T>, data));

#ifdef _DEBUG 
    // ���� ����, ��� �÷ο� ����
    if (
        //(pNode->BUFFER_GUARD_FRONT != GUARD_VALUE) ||
        //(pNode->BUFFER_GUARD_END != GUARD_VALUE)
        pNode->BUFFER_GUARD_FRONT != pNode->BUFFER_GUARD_END
        )
    {
        // ���� �ٸ��ٸ� false ��ȯ
        return false;
    }

    //  Ǯ ��ȯ�� �ùٸ��� �˻�
    if (pNode->POOL_INSTANCE_VALUE != reinterpret_cast<ULONG_PTR>(this))
    {
        // �� �������� ��� ����... ���߿� ����.


        // ���� �ٸ��ٸ� false ��ȯ
        return false;
    }
#endif // _DEBUG

    // ���� placement new �ɼ��� ���� �ִٸ�, ������ placement new�� ���� ���̹Ƿ� �ش� ��ü�� �Ҹ��ڸ� �������� �ҷ���
    // ���� �ִٸ� �޸�Ǯ�� �Ҹ��ڿ��� �ϰ��� ó������ ����
    if constexpr (bPlacementNew)
    {
        pNode->data.~T();
    }

    // ���� ����Ʈ ���ÿ� ������ �ְ�
    pNode->next = m_freeNode;
    m_freeNode = pNode;

    // ���Ǿ� ���� �ִ� ��ü �� ����
    m_allocatedCount--;

    // ��ȯ ����
    return true;
}

template<typename T, bool bPlacementNew>
inline void MemoryPool<T, bPlacementNew>::PrintUsage() const
{
    std::cout << "�޸� Ǯ Ÿ�� : " << typeid(T).name() << ":\n";
    std::cout << "�Ҵ�� �� ������Ʈ ���� : " << m_countPool << "\n";
    std::cout << "���� ������� ������Ʈ ���� : " << m_allocatedCount << "\n\n";
}