#pragma once

#include <chrono>

class CTimer
{
public:
	CTimer(void);
	~CTimer(void);

public:
    // ��� �ð��� ����ϴ� �Լ�
	void PrintElapsedTime(void);

private:
	std::chrono::steady_clock::time_point processStartTime;
};

