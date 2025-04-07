#pragma once

#include "Singleton.h"
#include <list>
class CSession;

const size_t MAX_MULTICAST_SESSIONS = 100;

//==========================================================================================================================================
// Broadcast
//==========================================================================================================================================
void BroadcastData(CSession* excludeSession, PACKET_HEADER* pPacket, UINT32 dataSize);
void BroadcastData(CSession* excludeSession, CPacket* pPacket, UINT32 dataSize);
void BroadcastPacket(CSession* excludeSession, PACKET_HEADER* pHeader, CPacket* pPacket);

//==========================================================================================================================================
// Unicast
//==========================================================================================================================================
void UnicastData(CSession* includeSession, PACKET_HEADER* pPacket, UINT32 dataSize);
void UnicastData(CSession* includeSession, CPacket* pPacket, UINT32 dataSize);
void UnicastPacket(CSession* includeSession, PACKET_HEADER* pHeader, CPacket* pPacket);

//==========================================================================================================================================
// ���� / �Ҹ�
//==========================================================================================================================================
void NotifyClientDisconnected(CSession* disconnectedCSession);
CSession* createSession(SOCKET ClientSocket, SOCKADDR_IN ClientAddr);

extern std::unordered_map<SOCKET, CSession*> g_SessionHashMap; // ������ ������ ���ǵ鿡 ���� ����

typedef void(*DisconnectCallback)(CSession* pSession);

class CSessionManager : public SingletonBase<CSessionManager> {
private:
    friend class SingletonBase<CSessionManager>;

public:
    explicit CSessionManager() noexcept;
    ~CSessionManager() noexcept;

    // ���� �����ڿ� ���� �����ڸ� �����Ͽ� ���� ����
    CSessionManager(const CSessionManager&) = delete;
    CSessionManager& operator=(const CSessionManager&) = delete;

public:
    void Update(void);

public:
    friend void BroadcastData(CSession* excludeSession, PACKET_HEADER* pPacket, UINT32 dataSize);
    friend void BroadcastData(CSession* excludeSession, CPacket* pPacket, UINT32 dataSize);
    friend void BroadcastPacket(CSession* excludeSession, PACKET_HEADER* pHeader, CPacket* pPacket);

    friend void UnicastData(CSession* includeSession, PACKET_HEADER* pPacket, UINT32 dataSize);
    friend void UnicastData(CSession* includeSession, CPacket* pPacket, UINT32 dataSize);
    friend void UnicastPacket(CSession* includeSession, PACKET_HEADER* pHeader, CPacket* pPacket);

public:
    static void RegisterDisconnectCallback(DisconnectCallback callback) { m_callbackDisconnect = callback; }

private:
    static DisconnectCallback m_callbackDisconnect;
};
