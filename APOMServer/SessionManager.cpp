#include "pch.h"
#include "WinSockManager.h"
#include "SessionManager.h"
#include "Packet.h"
#include "ObjectManager.h"

#include "Player.h"

#include "MemoryPoolManager.h"

#include "LogManager.h"

static CObjectManager& objectManager = CObjectManager::GetInstance();
static LogManager& logManager = LogManager::GetInstance();

std::unordered_map<SOCKET, CSession*> g_SessionHashMap; // ������ ������ ���ǵ鿡 ���� ����

DisconnectCallback CSessionManager::m_callbackDisconnect;

CSessionManager::CSessionManager() noexcept
{
}

CSessionManager::~CSessionManager() noexcept
{
}


std::string makeDebugLog1(long sectorPosX, long sectorPosY, unsigned char direction, unsigned short posX, unsigned short posY, unsigned char hp)
{
    std::stringstream ss;
    ss << "[ SECTOR : " << sectorPosX << ", " << sectorPosY << " ] [ direction : " << direction << " ] [ curPos : " << posX << ", " << posY << " ] [ hp : " << hp << " ]";

    return ss.str();
}

void CSessionManager::Update(void)
{
    // ��Ȱ��ȭ�� Ŭ���̾�Ʈ�� ����Ʈ���� ����
    // ���⼭ �����ϴ� ������ ���� �����ӿ� ���� �󿡼� ���ŵ� ���ǵ��� sendQ�� ������� ���� ���ŵǱ� ���ؼ� �̷��� �ۼ�.
    auto it = g_SessionHashMap.begin();
    while (it != g_SessionHashMap.end())
    {
        // ��Ȱ��ȭ �Ǿ��ٸ�
        if (!(*it).second->isAlive)
        {
            // ���������� �־��� �Լ� ȣ��
            m_callbackDisconnect((*it).second);

            // ����
            closesocket((*it).second->sock);

            sessionPool.Free((*it).second); // ���� ����

            it = g_SessionHashMap.erase(it);
        }
        // Ȱ�� ���̶��
        else
        {
            ++it;
        }
    }
}

void BroadcastData(CSession* excludeSession, PACKET_HEADER* pPacketHeader, UINT32 dataSize)
{
    CSession* pSession;
    for (auto& mapIndex : g_SessionHashMap)
    {
        pSession = mapIndex.second;

        // �ش� ������ alive�� �ƴϰų� ������ �����̶�� �Ѿ��
        if (!pSession->isAlive || excludeSession == pSession)
            continue;

        // �޽��� ����, ������ sendQ�� �����͸� ����
        int retVal = pSession->sendQ.Enqueue((const char*)pPacketHeader, dataSize);

        if (retVal != dataSize)
        {
            // �̷� ���� �־ �ȵ����� Ȥ�� �𸣴� �˻�
            // enqueue���� ������ �� ���� �������� ũ�Ⱑ ����á�ٴ� �ǹ��̹Ƿ�, ���⿡ ���Դٴ� ���� resize�� �ذ� ����.
            // �ٸ�, �������� ũ�Ⱑ ���� �� �� �ִ� ��Ȳ �� tcp ������ ����� 0�� ��쿣 resize�� �ذ��� �ȵ� Ȯ���� ������ ���°� ����. 
            // ���� ������ ���� ����� ���� �׽�Ʈ�ϸ鼭 ����

            int error = WSAGetLastError();
            std::cout << "Error : BroadcastData(), sendQ�� ����á��" << error << "\n";
            DebugBreak();

            NotifyClientDisconnected(pSession);
        }
    }
}

// Ŭ���̾�Ʈ���� �����͸� ��ε�ĳ��Ʈ�ϴ� �Լ�
void BroadcastData(CSession* excludeSession, CPacket* pPacket, UINT32 dataSize)
{
    CSession* pSession;
    for (auto& mapIndex : g_SessionHashMap)
    {
        pSession = mapIndex.second;

        // �ش� ������ alive�� �ƴϰų� ������ �����̶�� �Ѿ��
        if (!pSession->isAlive || excludeSession == pSession)
            continue;

        // �޽��� ����, ������ sendQ�� �����͸� ����
        int retVal = pSession->sendQ.Enqueue((const char*)pPacket, dataSize);

        if (retVal != dataSize)
        {
            // �̷� ���� �־ �ȵ����� Ȥ�� �𸣴� �˻�
            // enqueue���� ������ �� ���� �������� ũ�Ⱑ ����á�ٴ� �ǹ��̹Ƿ�, ���⿡ ���Դٴ� ���� resize�� �ذ� ����.
            // �ٸ�, �������� ũ�Ⱑ ���� �� �� �ִ� ��Ȳ �� tcp ������ ����� 0�� ��쿣 resize�� �ذ��� �ȵ� Ȯ���� ������ ���°� ����. 
            // ���� ������ ���� ����� ���� �׽�Ʈ�ϸ鼭 ����

            int error = WSAGetLastError();
            std::cout << "Error : BroadcastData(), sendQ�� ����á��" << error << "\n";
            DebugBreak();

            NotifyClientDisconnected(pSession);
        }
    }
}

void BroadcastPacket(CSession* excludeSession, PACKET_HEADER* pHeader, CPacket* pPacket)
{
    BroadcastData(excludeSession, pHeader, sizeof(PACKET_HEADER));
    BroadcastData(excludeSession, pPacket, pHeader->bySize);
}

// Ŭ���̾�Ʈ ������ ������ ��쿡 ȣ��Ǵ� �Լ�
void NotifyClientDisconnected(CSession* disconnectedCSession)
{
    //// ���� �̹� �׾��ٸ� NotifyClientDisconnected�� ȣ��Ǿ��� �����̹Ƿ� �ߺ��� ����. üũ�� ��.
    //if (disconnectedCSession->isAlive == false)
    //{
    //    DebugBreak();
    //}


    // ��¥ �߸��� ������ ���� ������ ����ٸ� �׵��� ����� �α׸� ���
    // ü���� 0�� ��ٰų� timeout���� ���� ���� ������ϴ� ��Ȳ�� �ƴ϶�� ��� �߸��� ������

    // ���� �ʾҴµ� isAlive�� false�� ����� ���
    if (!disconnectedCSession->pObj->isDead())
    {
        //DebugBreak();

        //logManager.LogError(disconnectedCSession->debugLogQueue.rear);

        //// ���� �αװ� ������ �����µ� ���
        //for (int i = 0; i < 30; ++i)
        //{
        //    logManager.LogError(makeDebugLog1(
        //        std::get<0>(disconnectedCSession->debugLogQueue.queue[i]),
        //        std::get<1>(disconnectedCSession->debugLogQueue.queue[i]),
        //        (unsigned int)std::get<2>(disconnectedCSession->debugLogQueue.queue[i]),
        //        std::get<3>(disconnectedCSession->debugLogQueue.queue[i]),
        //        std::get<4>(disconnectedCSession->debugLogQueue.queue[i]),
        //        (unsigned int)std::get<5>(disconnectedCSession->debugLogQueue.queue[i])
        //    ));
        //}
    }

    disconnectedCSession->isAlive = false;
}

void UnicastData(CSession* includeSession, PACKET_HEADER* pPacket, UINT32 dataSize)
{
    if (!includeSession->isAlive)
        return;

    // ������ sendQ�� �����͸� ����
    int retVal = includeSession->sendQ.Enqueue((const char*)pPacket, dataSize);

    if (retVal != dataSize)
    {
        // �̷� ���� �־ �ȵ����� Ȥ�� �𸣴� �˻�
        // enqueue���� ������ �� ���� �������� ũ�Ⱑ ����á�ٴ� �ǹ��̹Ƿ�, ���⿡ ���Դٴ� ���� resize�� �ذ� ����.
        // �ٸ�, �������� ũ�Ⱑ ���� �� �� �ִ� ��Ȳ �� tcp ������ ����� 0�� ��쿣 resize�� �ذ��� �ȵ� Ȯ���� ������ ���°� ����. 
        // ���� ������ ���� ����� ���� �׽�Ʈ�ϸ鼭 ����

        int error = WSAGetLastError();
        std::cout << "Error : UnicastData(), sendQ�� ����á��" << error << "\n";
        DebugBreak();

        NotifyClientDisconnected(includeSession);
    }
}

// ���ڷ� ���� ���ǵ鿡�� ������ ������ �õ��ϴ� �Լ�
void UnicastData(CSession* includeSession, CPacket* pPacket, UINT32 dataSize)
{
    if (!includeSession->isAlive)
        return;

    // ������ sendQ�� �����͸� ����
    int retVal = includeSession->sendQ.Enqueue((const char*)pPacket->GetBufferPtr(), dataSize);

    if (retVal != dataSize)
    {
        // �̷� ���� �־ �ȵ����� Ȥ�� �𸣴� �˻�
        // enqueue���� ������ �� ���� �������� ũ�Ⱑ ����á�ٴ� �ǹ��̹Ƿ�, ���⿡ ���Դٴ� ���� resize�� �ذ� ����.
        // �ٸ�, �������� ũ�Ⱑ ���� �� �� �ִ� ��Ȳ �� tcp ������ ����� 0�� ��쿣 resize�� �ذ��� �ȵ� Ȯ���� ������ ���°� ����. 
        // ���� ������ ���� ����� ���� �׽�Ʈ�ϸ鼭 ����

        int error = WSAGetLastError();
        std::cout << "Error : UnicastData(), sendQ�� ����á��" << error << "\n";
        DebugBreak();

        NotifyClientDisconnected(includeSession);
    }
}

void UnicastPacket(CSession* includeSession, PACKET_HEADER* pHeader, CPacket* pPacket)
{
    UnicastData(includeSession, pHeader, sizeof(PACKET_HEADER));
    UnicastData(includeSession, pPacket, pHeader->bySize);
}

CSession* createSession(SOCKET ClientSocket, SOCKADDR_IN ClientAddr)
{
    CWinSockManager& winSockManager = CWinSockManager::GetInstance();

    // accept�� �Ϸ�Ǿ��ٸ� ���ǿ� ��� ��, �ش� ���ǿ� ��Ŷ ����
    CSession* Session = sessionPool.Alloc();
    Session->recvQ.ClearBuffer();
    Session->sendQ.ClearBuffer();

    Session->isAlive = true;

    // ���� ���� �߰�
    Session->sock = ClientSocket;

    // IP / PORT ���� �߰�
    memcpy(Session->IP, winSockManager.GetIP(ClientAddr).c_str(), sizeof(Session->IP));
    Session->port = winSockManager.GetPort(ClientAddr);

    // ������ ������ ���� ��ü ������ ���� �ʱ�ȭ
    Session->pObj = nullptr;

    return Session;
}