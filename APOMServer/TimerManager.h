#pragma once

#include "Singleton.h"
#include <fstream>	// ���� ������� ���� ���

class CTimerManager : public SingletonBase<CTimerManager>
{
public:
	explicit CTimerManager() noexcept;
	~CTimerManager() noexcept;

	// ���� �����ڿ� ���� �����ڸ� �����Ͽ� ���� ����
	CTimerManager(const CTimerManager&) = delete;
	CTimerManager& operator=(const CTimerManager&) = delete;

public:
	// ��ǥ ������. �ظ��ϸ� 1000�� ����� ������ ��.
	void InitTimer(DWORD _targetFPS);
	bool CheckFrame(void);

public:
	inline DWORD GetCurrServerTime(void) { return m_currentServerTime; }
	
public:
	void MeasureJitter(void);
	inline DWORD GetJitter(void) { return m_jitter; }
	void PrintJitterStats(void);

private:
	// ������ ����
	DWORD m_targetFPS;				// 1�ʴ� ��ǥ ������
	DWORD m_givenFrameTime;			// 1�ʴ� �־����� �ð� -> 1000 / m_targetFPS
	DWORD m_standardServerTime;		// ���� ������ ���۵� �� �ʱ�ȭ�Ǵ� ��
	DWORD m_currentServerTime;		// ���� ������ ���۵� �� �ʱ�ȭ�ǰ�, ���Ŀ� �������� ���� �� ���� m_givenFrameTime ��ŭ ����.
	DWORD m_currentMainLogicTime;	// ���� ������ ���ư� �� ���� ����(timeGetTime)�ϴ� ��

	UINT16 m_FPS;	// ���� ������

private:
	// ���� ���� ����
	DWORD m_prevNetIOTime;			 // ���� ��Ʈ��ũ I/O ó�� �Ϸ� �ð�
	DWORD m_currNetIOTime;			 // ���� ��Ʈ��ũ I/O ó�� �Ϸ� �ð�
	DWORD m_jitter;					 // ���� ��

	// �ʴ� ���� ��� ����
	DWORD m_minJitter;               // 1�� ���� ������ ������ �ּҰ�
	DWORD m_maxJitter;               // 1�� ���� ������ ������ �ִ밪
	DWORD m_totalJitter;             // 1�� ���� ������ ������ ���� (��� ����� ����)
	DWORD m_jitterCount;             // 1�� ���� ������ ������ ���� (��� ����� ����)
};


