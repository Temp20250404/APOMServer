
#pragma once

#define RINGBUFFER_SIZE 2000

#include <iostream>
#include <string>

class CRingBuffer {
public:
    // ������: ���� ũ�⸦ �����ϰ� ���۸� �ʱ�ȭ�մϴ�.
    explicit CRingBuffer(int iBufferSize = RINGBUFFER_SIZE)
        : m_iCapacity(iBufferSize), m_iReadPos(0), m_iWritePos(0), m_iSize(0) {
        m_pBuffer = new char[m_iCapacity];
    }

    // �Ҹ���: ���� �޸𸮷� �Ҵ��� ���۸� �����մϴ�.
    ~CRingBuffer() {
        delete[] m_pBuffer;
    }

    // �ε����� �� ������ ��ȿ ���� ���� �����ִ� �Լ�
    int GetWrappedIndex(int index) const {
        return index % m_iCapacity;
    }

    // ���� ũ�⸦ �����ϴ� �Լ�
    void Resize(int newSize) {
        if (newSize <= 0) // ��ȿ���� ���� ũ��� �Լ� ����
            return;

        // �� ���� ũ�Ⱑ ���� ������ ũ�⺸�� ������ ���� �߻�
        if (m_iSize > newSize) {
            DebugBreak();
            //throw std::runtime_error("Resize error: New buffer size is too small to hold existing data.");
        }

        char* newBuffer = new char[newSize]; // �� ���� ����

        // ���� �����Ͱ� ������ �� ���۷� ����
        if (m_iSize > 0) {
            // �����Ͱ� ���ӵ� ���
            if (m_iReadPos < m_iWritePos) {
                memcpy(newBuffer, m_pBuffer + m_iReadPos, m_iSize);
            }
            // �����Ͱ� �������� ���
            else {
                int firstPartSize = m_iCapacity - m_iReadPos;
                memcpy_s(newBuffer, firstPartSize, m_pBuffer + m_iReadPos, firstPartSize);
                memcpy_s(newBuffer + firstPartSize, m_iWritePos, m_pBuffer, m_iWritePos);
            }
        }

        delete[] m_pBuffer; // ���� ���� ����

        m_pBuffer = newBuffer; // �� ���� ����
        m_iCapacity = newSize; // ���� �뷮 ������Ʈ

        m_iReadPos = 0; // ���ο� front ����
        m_iWritePos = m_iSize; // ���ο� rear ����
    }

    // ������ �� �뷮�� ��ȯ�մϴ�.
    int GetBufferSize(void) {
        return m_iCapacity;
    }

    // ���� ��� ���� ������ ũ�⸦ ��ȯ�մϴ�.
    int GetUseSize(void) {
        return m_iSize;
    }

    // ���� ��� ������ ������ ũ�⸦ ��ȯ�մϴ�.
    int GetFreeSize(void) {
        return m_iCapacity - m_iSize;
    }

    // �����͸� ���ۿ� �߰��ϴ� �Լ�
    int Enqueue(const char* chpData, int iSize) {
        if (GetFreeSize() < iSize) {
            iSize = GetFreeSize(); // ���� �뷮��ŭ�� ������ �߰�
        }

        int firstPart = std::min(iSize, m_iCapacity - m_iWritePos);
        int secondPart = iSize - firstPart;

        memcpy(m_pBuffer + m_iWritePos, chpData, firstPart);
        memcpy(m_pBuffer, chpData + firstPart, secondPart);

        m_iWritePos = GetWrappedIndex(m_iWritePos + iSize); // ���ο� rear ��ġ ����
        m_iSize += iSize; // ���� ��� ���� ���� ũ�� ������Ʈ

        return iSize;
    }

    // ���ۿ��� �����͸� �����ϴ� �Լ�
    int Dequeue(char* chpDest, int iSize) {
        if (m_iSize < iSize) {
            iSize = m_iSize; // ���� ��� ���� �뷮��ŭ�� ������ ����
        }

        int firstPart = std::min(iSize, m_iCapacity - m_iReadPos);
        int secondPart = iSize - firstPart;

        memcpy(chpDest, m_pBuffer + m_iReadPos, firstPart);
        memcpy(chpDest + firstPart, m_pBuffer, secondPart);

        m_iReadPos = GetWrappedIndex(m_iReadPos + iSize); // ���ο� front ��ġ ����
        m_iSize -= iSize; // ���� ��� ���� ���� ũ�� ������Ʈ

        if (m_iSize == 0)
        {
            m_iReadPos = 0;
            m_iWritePos = 0;
        }

        return iSize;
    }

    // �����͸� �������� �ʰ� ������ �պκ��� Ȯ���ϴ� �Լ�
    int Peek(char* chpDest, int iSize) {
        if (m_iSize < iSize) {
            iSize = m_iSize; // ���� ��� ���� �뷮��ŭ�� ������ Ȯ��
        }

        int firstPart = std::min(iSize, m_iCapacity - m_iReadPos);
        int secondPart = iSize - firstPart;

        memcpy_s(chpDest, firstPart, m_pBuffer + m_iReadPos, firstPart);
        memcpy_s(chpDest + firstPart, secondPart, m_pBuffer, secondPart);

        return iSize;
    }

    // ���۸� ���� �Լ�
    void ClearBuffer(void) {
        m_iReadPos = 0;
        m_iWritePos = 0;
        m_iSize = 0;
    }

    // ���� ���� ��ġ���� ���� �߰��� �� �ִ� ũ�⸦ ��ȯ�մϴ�.
    int DirectEnqueueSize(void) {
        if (m_iReadPos > m_iWritePos)
            return m_iReadPos - m_iWritePos;
        else
            return m_iCapacity - m_iWritePos;
    }

    // ���� �б� ��ġ���� ���� ������ �� �ִ� ũ�⸦ ��ȯ�մϴ�.
    int DirectDequeueSize(void) {
        if (m_iReadPos < m_iWritePos)
            return m_iWritePos - m_iReadPos;
        else
            return m_iCapacity - m_iReadPos;
    }

    // ���� ��ġ�� �̵���Ű�� �Լ� (�����͸� �߰�)
    inline int MoveRear(int iSize) {
        int moveSize = std::min(iSize, GetFreeSize());
        m_iWritePos = GetWrappedIndex(m_iWritePos + moveSize);
        m_iSize += moveSize;
        return moveSize;
    }

    // �б� ��ġ�� �̵���Ű�� �Լ� (�����͸� ����)
    inline int MoveFront(int iSize) {
        int moveSize = std::min(iSize, GetUseSize());
        m_iReadPos = GetWrappedIndex(m_iReadPos + moveSize);
        m_iSize -= moveSize;
        return moveSize;
    }

    // ������ front �κ��� �����͸� ��ȯ�մϴ�.
    char* GetFrontBufferPtr(void) {
        return m_pBuffer + m_iReadPos;
    }

    // ������ rear �κ��� �����͸� ��ȯ�մϴ�.
    char* GetRearBufferPtr(void) {
        return m_pBuffer + m_iWritePos;
    }

private:
    char* m_pBuffer;    // �������� ������ ���� �迭
    int m_iCapacity;    // ������ ��ü �뷮
    int m_iReadPos;     // ���� �б� ��ġ (front)
    int m_iWritePos;    // ���� ���� ��ġ (rear)
    int m_iSize;        // ���� ��� ���� �������� ũ��
};
