

//===================================================
// pch
//===================================================
#include "pch.h"

//===================================================
// �Ŵ��� class
//===================================================
#include "WinSockManager.h" // ������ ����
#include "SessionManager.h"
#include "TimerManager.h"
#include "NetIOManager.h"
#include "ObjectManager.h"
#include "RoomManager.h"
#include "LoginManager.h"

//===================================================
// ������Ʈ ����
//===================================================
#include "Player.h"
#include "Session.h"
#include "Packet.h"

//===================================================
// ��Ŷ ó�� ����
//===================================================
#include "PacketProc.h"

//===================================================
// ������ ����
//===================================================
#include "Content.h"

//===================================================
// Ű �Է�
//===================================================
#include <conio.h>

//===================================================
// AI ����
//===================================================
#include "AIManager.h"
#include "Script_Boss.h"

//===================================================
// ��Ʈ��ũ ����
//===================================================
#include "Session.h"

//===================================================
// ������ ����
//===================================================
#include <process.h>

//===================================================
// �޸� ����
//===================================================
#include "MemoryPoolManager.h"

//===================================================
// �α� ����
//===================================================
#include "LogManager.h"

//===================================================
// ����͸� ����
//===================================================
#include "SystemMonitor.h"

//===================================================
// protobuf ����
//===================================================
#ifdef _DEBUG
#pragma comment(lib, "Protobuf/Debug/libprotobufd.lib")
#else
#pragma comment(lib, "Protobuf/Release/libprotobuf.lib")
#endif 

#include "Protobuf/Protocol.pb.h"

//===================================================
// dump ����
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
CAIManager& aiManager = CAIManager::GetInstance();


unsigned int WINAPI MonitorThread(void* pArg);

int main()
{
    // ���� �õ� ����
    srand(static_cast<unsigned int>(time(nullptr)));

    // ����� ������ ����
    HANDLE hMonitorThread;   // ����� ������ �ڵ鰪
    DWORD dwThreadID;
    hMonitorThread = (HANDLE)_beginthreadex(NULL, 0, MonitorThread, NULL, 0, (unsigned int*)&dwThreadID);

    //=====================================================================================================================================
    // listen ���� �غ�
    //=====================================================================================================================================
    CWinSockManager& winSockManager = CWinSockManager::GetInstance();

    UINT8 options = 0;
    options |= OPTION_NONBLOCKING;

    winSockManager.StartServer(PROTOCOL_TYPE::TCP_IP, dfNETWORK_PORT, options, INADDR_ANY, SOMAXCONN_HINT(65535));

    //=====================================================================================================================================
    // recv �� ���������� ó���ϴ� �Լ� ���
    //=====================================================================================================================================
    CNetIOManager& netIOManager = CNetIOManager::GetInstance();
    netIOManager.RegisterPacketProcCallback(PacketProc);        // ��Ŷ�� ó���ϴ� �Լ� ���
    netIOManager.RegisterAcceptCreateProcCallback(CreateAcceptObject);    // accept �� ���ǰ� ����Ǵ� ��ü�� ����� CObject* ��ü�� ��ȯ�ϴ� �Լ��� ���
    netIOManager.RegisterAcceptAfterCreateCallback(LoginAcceptObject);    // accept �� ���ǰ� ����Ǵ� ��ü�� ����� CObject* ��ü�� ��ȯ�ϴ� �Լ��� ���

    //=====================================================================================================================================
    // ������ ����� �� ȣ��� �Լ� ���
    //=====================================================================================================================================
    CSessionManager::RegisterDisconnectCallback(DisconnectSessionProc);

    //=====================================================================================================================================
    // ���� �ð� ����
    //=====================================================================================================================================
    CTimerManager& timerManager = CTimerManager::GetInstance();

    timerManager.InitTimer(25);


    //=====================================================================================================================================
    // �� �Ŵ��� �ʱ�ȭ
    //=====================================================================================================================================
    CRoomManager& roomManager = CRoomManager::GetInstance();
    roomManager.InitRooms();


    //=====================================================================================================================================
    // �α��� ���� �ҷ�����
    //=====================================================================================================================================
    CLoginManager& loginManager = CLoginManager::GetInstance();
    if(!loginManager.LoadFromFile("logins.json"))
    {
        std::cout << "������ �ҷ����� ���߽��ϴ�.\n";
        std::system("pause");
    }

    //=====================================================================================================================================
    // AI ���� �߰�, ������ ������ �����̴� ���߿� �ٸ������� �ϵ��� ���� ����
    //=====================================================================================================================================

    BTBuilder builder = CreateBossBT;

    // AIContext (����)
    AIContext boss{};

    boss.currentHP = 100.0f;
    boss.maxHP = 100.0f;
    boss.bDeath = false;

    boss.usedSkill1 = false;
    boss.usedSkill2 = false;
    boss.usedSkill3 = false;

    boss.attackAnimTime = 1.f;  // ���� �ִϸ��̼�
    boss.dieAnimTime = 2.f;     // ��� �ִϸ��̼�
    boss.skill1AnimTime = 1.0f;  // ��ų1 �ִϸ��̼�
    boss.skill2AnimTime = 1.5f;  // ��ų2 �ִϸ��̼�
    boss.skill3AnimTime = 2.0f;  // ��ų3 �ִϸ��̼�
    boss.phaseTimer = 0;

    boss.hasTargetPlayer = false;
    boss.playerDistance = 0;

    boss.idleTime = 0;

    // ��� ����
    boss.attackRange = 3.0f;
    boss.moveSpeed = 0.4f;          // 1ȸ���� 0.6��ŭ �̵�. ���� 25�����ӿ� 1ȸ�� �۵��ϴ� 1�ʿ� 0.6��ŭ ������
    boss.idleResetTime = 3.0f;
    boss.deltaTime = 1.f;           // ���� ai�� 25�����ӿ� 1���� �۵��ϰ�, ������ 1�ʿ� 25������ �����δ�. �׷��Ƿ� 1 ����
    boss.detectionRange = 15.0f;     // �÷��̾� ���� ����

    // ������ ���� ��ġ
    boss.currentPos.posX = 0.f; boss.currentPos.posY = 0.f; boss.currentPos.posZ = 0.f;

    AIEntity* pAIEntity;
    for (int i = 0; i < 1; ++i)
    {
        // context�� �� ���� ����
        CRoom* pRoom = roomManager.GetRoomById(i);
        boss.ptargetRoom = pRoom;

        // AIEntity �ν��Ͻ� ����
        pAIEntity = new AIEntity(boss, builder);

        // ���� ID �߰�
        boss.ID = pAIEntity->GetID();

        // AI �Ŵ����� AI ��ü ���
        aiManager.AddEntity(pAIEntity);

        // �濡 AI ��ü ���� ���
        pRoom->AddEntity(pAIEntity);
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
    // ���� ����
    //=====================================================================================================================================
    int AIFrameCnt = 0;

    while (!g_bShutdown)
    {
        try
        {
            if (timerManager.CheckFrame())
            {
                // ��Ʈ��ũ I/O ó��
                netIOManager.netIOProcess();

                // ���� ���� ������Ʈ
                Update();

                AIFrameCnt++;
                if (AIFrameCnt >= 5)
                {
                    aiManager.UpdateAll();
                    AIFrameCnt = 0;
                }
            }

            // Ű���� �Է��� ���� ������ ������ �� ���
            ServerControl();

            // ����͸� ������ ǥ��, ����, �����ϴ� ��� ���
            Monitor();
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << "\n";

            g_bShutdown = true;
        }
    }

    // ����� ���� ������ �ִ� ���� �����ϰ� DB� ����

    // �α��� ������ ����
    loginManager.SaveToFile("logins.json");

    // ������ ���� �Ŵ��� ����
    winSockManager.Cleanup();
    // ���� �߻� ���� �ڵ�
}

// ���� ����
void Update(void)
{
    objectManager.Update();
    objectManager.LateUpdate();

    sessionManager.Update();
    //aiManager.UpdateAll();

    //logManager.saveLog();
}

bool PressKey(WCHAR checkKey, WCHAR input)
{
    return towlower(checkKey) == towlower(input);
}

// Ű���� �Է��� ���� ������ ������ �� ���
void ServerControl(void)
{
    //==========================================================
    // L : ��Ʈ�� Lock     U : ��Ʈ�� Unlock      Q : ���� ����
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

        // ��Ʈ�� ���� ��� Ȯ��
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

// ����͸� ������ ǥ��, ����, �����ϴ� ��� ���
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



// ���ڷ� ���� ������ TPS �� ���� ���� ������ 1�ʸ��� �����ϴ� ������
unsigned int WINAPI MonitorThread(void* pArg)
{
    SystemMonitor monitor;

    const CNetIOManager& netIOManager = CNetIOManager::GetInstance();

    while (true)
    {
        UINT32 globalTime = InterlockedCompareExchange(&g_iTime, 0, 0);
        if (globalTime - g_iFpsCheck >= 1000)
        {
        //    std::wcout << L"======================================================" << std::endl;

        //    //monitor.PrintMonitoringData();
        //    std::cout << "\n";

        //    g_iFpsCheck += 1000;

        //    UINT32 netLoop = InterlockedExchange(&g_iNetworkLoop, 0);
        //    UINT32 fps = InterlockedExchange(&g_iFPS, 0);

        //    std::cout << "FPS & Network Loop Num : " << fps << "\n";
        //    std::cout << "Main Loop : " << netLoop << "\n";
        //    std::cout << "SyncCount : " << g_iSyncCount << "\n";

        //    std::wcout << L"======================================================" << std::endl;

        //    std::cout << "Session Count" << "\n\n";
        //    std::cout << "Connect : " << g_SessionHashMap.size() << "\n";
        //    std::cout << "Accept : " << netIOManager.acceptSessionCnt << "\n";
        //    std::cout << "Disconnect : " << netIOManager.disconnectSessionCnt << "\n";
        }

        // 1�ʰ� Sleep
        Sleep(1000);
    }

    return 0;
}


