#pragma once

class CSession;
class CPacket;

#include "CircularQueue.h"

#include "NetIOManager.h"

// �⺻ CObject Ŭ���� ����
class CObject {
public:
    explicit CObject(float _x = 0, float _y = 0, float _z = 0) noexcept;
    virtual ~CObject() = default;

    // �޸� Ǯ���� ����� �뵵
    virtual void Init(void);

    // ������Ʈ �Լ� (���� �������� ������Ʈ ���¸� �����ϴ� �� ���)
    virtual void Update(void);
    virtual void LateUpdate(void);

    // ������ ���� �Լ�
    virtual void Move(void);


public:
    // ��ġ ���� �� ��������
    constexpr void SetPosition(float _x, float _y, float _z) {
        m_x = _x;
        m_y = _y;
        m_z = _z;
    }

    constexpr void getPosition(float& _x, float& _y, float& _z) const {
        _x = m_x;
        _y = m_y;
        _z = m_z;
    }

public:
    inline bool isDead(void) { return m_bDead; }
    
private:
    void CheckTimeout(void);    // �߱������� Timeout�� Ȯ���ϱ� ���� �ҷ��ִ� �Լ�
    void SetCurTimeout(void);   // Timeout ���� �����ϴ� �Լ�. ��Ŷ ó���� �� ���� ȣ���ؼ� Timeout ����
    friend bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket);
    friend void CNetIOManager::netProc_Recv(CSession* pSession);

public:
    CSession* m_pSession;   // ������Ʈ�� ����� ����
    UINT32 m_ID;            // ID

public:
    float m_x, m_y, m_z;   // ���� ��ġ(3����)
    bool m_bDead;           // �׾����� ����

private:
    // Timeout ����
    UINT32 m_lastTimeoutCheckTime; // timegettime ���� ��

private:
    static UINT32 g_ID; // ID
    static UINT32 m_maxLastTimeoutCheckTime; // timegettime ���� ��
};