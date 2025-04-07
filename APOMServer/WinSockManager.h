#pragma once

#include "Singleton.h"

#define OPTION_NONBLOCKING 0x01

enum class PROTOCOL_TYPE
{
    TCP_IP,
    UDP,
    END
};

class CWinSockManager : public SingletonBase<CWinSockManager> {
private:
    friend class SingletonBase<CWinSockManager>;

public:
    explicit CWinSockManager() noexcept;
    ~CWinSockManager() noexcept;
    
    // ���� �����ڿ� ���� �����ڸ� �����Ͽ� ���� ����
    CWinSockManager(const CWinSockManager&) = delete;
    CWinSockManager& operator=(const CWinSockManager&) = delete;

private:
    void InitWinSock() noexcept;
    void CreateListenSocket(PROTOCOL_TYPE type);
    void Bind(UINT16 serverPort, UINT32 serverIP);
    void Listen(UINT32 somaxconn);
    void SetOptions(UINT8 options);

public:
    SOCKET Accept(SOCKADDR_IN& ClientAddr) noexcept;
    void StartServer(PROTOCOL_TYPE type, UINT16 serverPort, UINT8 options = 0, UINT32 serverIP = INADDR_ANY, UINT32 somaxconn = SOMAXCONN_HINT(65535)) noexcept;
    void Cleanup() noexcept;

public:
    static std::string GetIP(const SOCKADDR_IN& ClientAddr) noexcept;
    static std::string GetIP(const IN_ADDR& ClientInAddr) noexcept;
    static UINT16 GetPort(const SOCKADDR_IN& ClientAddr) noexcept;
    static UINT16 GetPort(const UINT16& ClientInAddr) noexcept;
    static bool DomainToIP(WCHAR* szDomain, IN_ADDR* pAddr);

public:
    constexpr SOCKET GetListenSocket(void) { return m_listenSocket; }

private:
    SOCKET m_listenSocket;                // listen ����
};