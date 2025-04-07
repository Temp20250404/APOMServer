#pragma once

#include <Pdh.h>
#include <PdhMsg.h>
#include <strsafe.h>

#pragma comment(lib, "Pdh.lib")

#include "cpuUsage.h"
#include "Timer.h"

#define df_PDH_ETHERNET_MAX 8
//--------------------------------------------------------------
// �̴��� �ϳ��� ���� Send,Recv PDH ���� ����.
//--------------------------------------------------------------
struct st_ETHERNET
{
    bool _bUse;
    WCHAR _szName[128];
    PDH_HCOUNTER _pdh_Counter_Network_RecvBytes;
    PDH_HCOUNTER _pdh_Counter_Network_SendBytes;

    PDH_HQUERY _sendQuery;
    PDH_HQUERY _recvQuery;
};

class SystemMonitor {
public:
    explicit SystemMonitor(void);
    ~SystemMonitor(void);

public:
    double GetCounterValue(PDH_HCOUNTER counter);
    bool InitNetworkTrafficBytes(void);
    bool MeasureNetworkTrafficBytes(void);
    void PrintMonitoringData(void);

private:
    PDH_HQUERY queryHandle;            // PDH ���� �ڵ�

    // CPU ��ü ����                   // �� 2���� �׸��� ���� ����Ϳ��� �ڵ�� �����ϸ� ��Ȯ�� ���� ������ �����Ƿ� �ٸ� ���(CPU_UsageChekcing ������Ʈ)���� ����
    // ���μ��� CPU ����

    PDH_HCOUNTER privateBytesCounter;  // ���μ��� ���� �Ҵ� �޸�
    PDH_HCOUNTER poolNonPagedCounter;  // ���μ��� �������� �޸�
    PDH_HCOUNTER availableMemory;      // ��� ���� �޸�
    PDH_HCOUNTER memoryPoolNonPaged;   // ���������� �޸�

    PDH_HCOUNTER TCPv4Send;            // TCPv4���� ���۵� ���׸�Ʈ ����
    PDH_HCOUNTER TCPv4Retransmitted;   // TCPv4���� �����۵� ���׸�Ʈ ����


    double maxRetransmitted;    // �����۵� ���׸�Ʈ ���� �ִ� / �ּ� / ��� ��
    double minRetransmitted;
    double AvgRetransmitted;    // ����� ������ 7/8, �߰��� �� 1/8�� ���. 

    UINT64 sumSendSegments;     // ���� ���۵� ���� ���� ���׸�Ʈ ������ byte ũ�� ����. �뷫������, ū ���� ����Ǳ⿡ UINT64�� ���
    UINT64 sumSendBytes;




    PDH_HQUERY _pdh_Query;

    st_ETHERNET _EthernetStruct[df_PDH_ETHERNET_MAX];   // ��ī�� �� PDH ����
    double _pdh_value_Network_RecvBytes;                // �� Recv Bytes ��� �̴����� Recv ��ġ �ջ�
    double _pdh_value_Network_SendBytes;                // �� Send Bytes ��� �̴����� Send ��ġ �ջ�

    // CPU ��ü ���� �� ���μ��� CPU ���� ����
    CCpuUsage CPUTime; // CPUTime(hProcess)

    char padding[200];

    CTimer processTimer;
};