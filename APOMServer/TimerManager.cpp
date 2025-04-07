
#include "pch.h"
#include "TimerManager.h"

#include <ctime>    // �ð� ���� ���̺귯��
#include <sstream>  // ��Ʈ�� ��Ʈ�� ���
#include <iomanip>  // �ð� ������ ���� ���

#include "DebugDefines.h"
#include "LogManager.h"

static LogManager& logManager = LogManager::GetInstance();

CTimerManager::CTimerManager() noexcept
{
}

CTimerManager::~CTimerManager() noexcept
{
}

void CTimerManager::InitTimer(DWORD _targetFPS)
{
	timeBeginPeriod(1);

	m_targetFPS = _targetFPS;
	m_givenFrameTime = 1000 / m_targetFPS;
	m_standardServerTime = timeGetTime();
	m_currentServerTime = m_standardServerTime;

	// ���Ϳ�, ���⼭ �ð� ������ �߱⿡ MeasureJitter���� �Ź� m_prevNetIOTime�� 0���� �˻縦 ���ϰ� �ٷ� ��� ����. ó���� Ƣ�°� ����
	m_prevNetIOTime = m_standardServerTime;
}

bool CTimerManager::CheckFrame(void)
{
	// ���� ���� �ð� ������Ʈ
	m_currentMainLogicTime = timeGetTime();

#ifdef MEASURE_GITTER

	// ���� ����
	MeasureJitter();

#endif // MEASURE_GITTER

#ifdef CHECK_FPS

	// ������ ���߱�
	if (m_currentMainLogicTime < (m_currentServerTime + m_givenFrameTime))
		return false;

	m_currentServerTime += m_givenFrameTime;

	m_FPS++;

	// fps ����
	if ((m_currentServerTime - m_standardServerTime) >= 1000)
	{
		std::cout << "Logic Frame"<< m_FPS << "\n"; //" / " << m_currentServerTime - m_standardServerTime << "\n";
		m_standardServerTime += 1000;

		m_FPS = 0;

#ifdef CHECK_GITTER

		// ���� ���
		PrintJitterStats();

#endif // CHECK_GITTER
	}

	return true;

#else

	// ������ ���߱�
	if (m_currentMainLogicTime < (m_currentServerTime + m_givenFrameTime))
		return false;
	

	m_currentServerTime += m_givenFrameTime;

	//// �������� �з��� ��� ���� �ð��� ��ܿ�.
	//while (m_currentMainLogicTime > m_currentServerTime)
	//{
	//	m_currentServerTime += m_givenFrameTime;
	//}

#ifdef CHECK_GITTE

	// 1�ʸ��� ���� ��� ���
	if ((m_currentServerTime - m_standardServerTime) >= 1000) {
		m_standardServerTime += 1000;
		PrintJitterStats();
	}

#endif // CHECK_GITTER

	return true;
	
#endif
}

void CTimerManager::MeasureJitter(void)
{
	m_currNetIOTime = timeGetTime();	// m_currentMainLogicTime�� �����������, ��Ȯ�� ������ ���� ���� �ѹ� �� ����(timeGetTime).

	// ���� ��Ʈ��ũ �����Ӱ� ���� ��Ʈ��ũ �����Ӱ��� ���̸� ��� 
	DWORD delay = m_currNetIOTime - m_prevNetIOTime;

	// m_givenFrameTime���� ���̸� ���
	m_jitter = abs(static_cast<int>(delay) - static_cast<int>(m_givenFrameTime));

	// 1�� ������ ���� ��� ���
	if (m_jitterCount == 0) {
		m_minJitter = m_jitter;
		m_maxJitter = m_jitter;
	}
	else {
		if (m_jitter < m_minJitter) m_minJitter = m_jitter;
		if (m_jitter > m_maxJitter) m_maxJitter = m_jitter;
	}

	m_totalJitter += m_jitter;
	m_jitterCount++;

	m_prevNetIOTime = m_currNetIOTime;
}

void CTimerManager::PrintJitterStats(void)
{
	if (m_jitterCount > 0) {
		DWORD avgJitter = m_totalJitter / m_jitterCount;
		//std::cout << "Jitter - Min: " << m_minJitter << " ms, Max: " << m_maxJitter << " ms, Avg: " << avgJitter << " ms\n"; 
		
		// ���Ͽ� ���
		logManager.LogDebug("Jitter\nMin: ", m_minJitter, " ms\nMax: ", m_maxJitter, " ms\nAvg: ", avgJitter, " ms\n\n");
	}

	// ��� �ʱ�ȭ
	m_minJitter = 0;
	m_maxJitter = 0;
	m_totalJitter = 0;
	m_jitterCount = 0;
}
