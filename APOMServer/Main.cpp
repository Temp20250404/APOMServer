

//===================================================
// pch
//===================================================
#include "pch.h"

//===================================================
// 매니저 class
//===================================================
#include "WinSockManager.h" // 윈도우 소켓
#include "SessionManager.h"
#include "TimerManager.h"
#include "NetIOManager.h"
#include "ObjectManager.h"
#include "RoomManager.h"

//===================================================
// 오브젝트 관련
//===================================================
#include "Player.h"
#include "Session.h"
#include "Packet.h"

//===================================================
// 패킷 처리 관련
//===================================================
#include "PacketProc.h"

//===================================================
// 콘텐츠 관련
//===================================================
#include "Content.h"

//===================================================
// 키 입력
//===================================================
#include <conio.h>

//===================================================
// AI 관련
//===================================================
#include "AIManager.h"
#include "Script_Boss.h"

//===================================================
// 네트워크 관련
//===================================================
#include "Session.h"

//===================================================
// 스레드 관련
//===================================================
#include <process.h>

//===================================================
// 메모리 관련
//===================================================
#include "MemoryPoolManager.h"

//===================================================
// 로그 관련
//===================================================
#include "LogManager.h"

//===================================================
// 모니터링 관련
//===================================================
#include "SystemMonitor.h"

//===================================================
// protobuf 관련
//===================================================
#ifdef _DEBUG
#pragma comment(lib, "Protobuf/Debug/libprotobufd.lib")
#else
#pragma comment(lib, "Protobuf/Release/libprotobuf.lib")
#endif 

#include "Protobuf/Protocol.pb.h"

//===================================================
// dump 관련
//===================================================
CCrashDump dump;






//========================================
bool g_bShutdown = false;

void Update(void);
void ServerControl(void);
void Monitor(void);
//========================================
constexpr int TICK_PER_FRAME = 40;
constexpr int FRAME_PER_SECONDS = (1000) / TICK_PER_FRAME;

BOOL g_bShutDown = FALSE;
extern int g_iSyncCount;
int g_iDisconCount = 0;
int g_iDisConByRBFool = 0;
int g_iDisConByTimeOut = 0;

unsigned int g_iOldFrameTick;
unsigned int g_iFpsCheck;
unsigned int g_iTime;
unsigned int g_iFPS;
unsigned int g_iNetworkLoop;
unsigned int g_iFirst;

ULONGLONG g_fpsCheck;
//========================================


CObjectManager& objectManager = CObjectManager::GetInstance();
CSessionManager& sessionManager = CSessionManager::GetInstance();
LogManager& logManager = LogManager::GetInstance();
AIManager& aiManager = AIManager::GetInstance();


unsigned int WINAPI MonitorThread(void* pArg);

int main()
{
    // 랜덤 시드 설정
    srand(static_cast<unsigned int>(time(nullptr)));

    // 모니터 스레드 생성
    HANDLE hMonitorThread;   // 모니터 스레드 핸들값
    DWORD dwThreadID;
    hMonitorThread = (HANDLE)_beginthreadex(NULL, 0, MonitorThread, NULL, 0, (unsigned int*)&dwThreadID);

    //=====================================================================================================================================
    // listen 소켓 준비
    //=====================================================================================================================================
    CWinSockManager& winSockManager = CWinSockManager::GetInstance();

    UINT8 options = 0;
    options |= OPTION_NONBLOCKING;

    winSockManager.StartServer(PROTOCOL_TYPE::TCP_IP, dfNETWORK_PORT, options, INADDR_ANY, SOMAXCONN_HINT(65535));

    //=====================================================================================================================================
    // recv 한 프로토콜을 처리하는 함수 등록
    //=====================================================================================================================================
    CNetIOManager& netIOManager = CNetIOManager::GetInstance();
    netIOManager.RegisterPacketProcCallback(PacketProc);        // 패킷을 처리하는 함수 등록
    netIOManager.RegisterAcceptCreateProcCallback(CreateAcceptObject);    // accept 시 세션과 연결되는 객체를 만드는 CObject* 객체를 반환하는 함수를 등록
    netIOManager.RegisterAcceptAfterCreateCallback(LoginAcceptObject);    // accept 시 세션과 연결되는 객체를 만드는 CObject* 객체를 반환하는 함수를 등록

    //=====================================================================================================================================
    // 세션이 종료될 때 호출될 함수 등록
    //=====================================================================================================================================
    CSessionManager::RegisterDisconnectCallback(DisconnectSessionProc);

    //=====================================================================================================================================
    // 서버 시간 설정
    //=====================================================================================================================================
    CTimerManager& timerManager = CTimerManager::GetInstance();

    timerManager.InitTimer(25);


    //=====================================================================================================================================
    // 룸 매니저 초기화
    //=====================================================================================================================================
    CRoomManager& roomManager = CRoomManager::GetInstance();
    roomManager.InitRooms();



    //=====================================================================================================================================
    // AI 정보 추가, 내용이 많아질 예정이니 나중에 다른곳에서 하도록 수정 예정
    //=====================================================================================================================================

    BTBuilder builder = CreateBossBT;

    // 초기 AIContext 설정
    AIContext context;
    context.maxHP = 100;             // 초기 체력
    context.currentHP = context.maxHP;
    context.attackRange = 5.0f;
    context.moveSpeed = 1.0f;
    context.detectionRange = 15.0f;

    AIEntity* pAIEntity;
    for (int i = 0; i < 10; ++i)
    {
        // context에 방 정보 삽입
        CRoom* pRoom = roomManager.GetRoomById(i);
        context.ptargetRoom = pRoom;

        // AIEntity 인스턴스 생성
        pAIEntity = new AIEntity(context, builder);

        // 고유 ID 추가
        context.ID = pAIEntity->GetID();

        // AI 매니저에 AI 객체 등록
        aiManager.AddEntity(pAIEntity);
    }






    //=======================================================
    g_iNetworkLoop = 0;
    g_iFirst = timeGetTime();
    g_iOldFrameTick = g_iFirst;
    g_iTime = g_iOldFrameTick;
    g_iFPS = 0;
    g_iFpsCheck = g_iOldFrameTick;
    //=======================================================






    //=====================================================================================================================================
    // 메인 로직
    //=====================================================================================================================================
    while (!g_bShutdown)
    {
        try
        {
            if (timerManager.CheckFrame())
            {
                // 네트워크 I/O 처리
                netIOManager.netIOProcess();

                // 게임 로직 업데이트
                Update();
            }

            // 키보드 입력을 통해 게임을 제어할 시 사용
            ServerControl();

            // 모니터링 정보를 표시, 저장, 전송하는 경우 사용
            Monitor();
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << "\n";

            g_bShutdown = true;
        }
    }

    // 종료시 현재 서버에 있는 정보 안전하게 DB등에 저장

    // 윈도우 소켓 매니저 정리
    winSockManager.Cleanup();
    // 예외 발생 가능 코드
}

// 메인 로직
void Update(void)
{
    objectManager.Update();
    objectManager.LateUpdate();

    sessionManager.Update();
    aiManager.UpdateAll();

    //logManager.saveLog();
}

bool PressKey(WCHAR checkKey, WCHAR input)
{
    return towlower(checkKey) == towlower(input);
}

// 키보드 입력을 통해 게임을 제어할 시 사용
void ServerControl(void)
{
    //==========================================================
    // L : 컨트롤 Lock     U : 컨트롤 Unlock      Q : 서버 종료
    //==========================================================

    static bool bControlMode = false;

    if (_kbhit())
    {
        WCHAR ControlKey = _getch();

        if (PressKey(ControlKey, L'U'))
        {
            if (!bControlMode)
            {
                std::wcout << L"Control Mode : Press Q - Quit\n";
                std::wcout << L"Control Mode : Press L - Key Lock\n";

                bControlMode = true;
            }
        }

        if (PressKey(ControlKey, L'L'))
        {
            if (bControlMode)
            {
                std::wcout << L"Control Locked. Press U to Unlock Control\n";

                bControlMode = false;
            }
        }

        // 컨트롤 중인 경우 확인
        if (bControlMode)
        {
            if (PressKey(ControlKey, L'Q'))
            {
                g_bShutdown = true;

                std::wcout << L"Server Shutdown\n";
            }
        }
    }
}

// 모니터링 정보를 표시, 저장, 전송하는 경우 사용
void Monitor(void)
{
    InterlockedIncrement(&g_iNetworkLoop);

    g_iTime = timeGetTime();
    if (g_iTime - g_iOldFrameTick >= TICK_PER_FRAME)
    {
        g_iOldFrameTick = g_iTime - ((g_iTime - g_iFirst) % TICK_PER_FRAME);
        InterlockedIncrement(&g_iFPS);
    }
}



// 인자로 받은 서버의 TPS 및 보고 싶은 정보를 1초마다 감시하는 스레드
unsigned int WINAPI MonitorThread(void* pArg)
{
    SystemMonitor monitor;

    const CNetIOManager& netIOManager = CNetIOManager::GetInstance();

    while (true)
    {
        UINT32 globalTime = InterlockedCompareExchange(&g_iTime, 0, 0);
        if (globalTime - g_iFpsCheck >= 1000)
        {
            std::wcout << L"======================================================" << std::endl;

            monitor.PrintMonitoringData();
            std::cout << "\n";

            g_iFpsCheck += 1000;

            UINT32 netLoop = InterlockedExchange(&g_iNetworkLoop, 0);
            UINT32 fps = InterlockedExchange(&g_iFPS, 0);

            std::cout << "FPS & Network Loop Num : " << fps << "\n";
            std::cout << "Main Loop : " << netLoop << "\n";
            std::cout << "SyncCount : " << g_iSyncCount << "\n";

            std::wcout << L"======================================================" << std::endl;

            std::cout << "Session Count" << "\n\n";
            std::cout << "Connect : " << g_SessionHashMap.size() << "\n";
            std::cout << "Accept : " << netIOManager.acceptSessionCnt << "\n";
            std::cout << "Disconnect : " << netIOManager.disconnectSessionCnt << "\n";
        }

        // 1초간 Sleep
        Sleep(1000);
    }

    return 0;
}


