
#pragma once

#include "pch.h"
#include "Object.h"
#include "Session.h"
#include "TimerManager.h"

#include "Packet.h"
#include "CircularQueue.h"

#include "LogManager.h"

UINT32 CObject::g_ID = 1;
UINT32 CObject::m_maxLastTimeoutCheckTime = 0;

static CTimerManager& timerManager = CTimerManager::GetInstance();
static LogManager& logManager = LogManager::GetInstance();

CObject::CObject(float _x, float _y, float _z) noexcept
	: m_x(_x), m_y(_y), m_z(_z), m_pSession(nullptr), m_bDead(false)
{
	m_ID = g_ID;
	g_ID++;
}

void CObject::Move()
{
}

void CObject::Init(void)
{
	SetCurTimeout();

	m_bDead = false;
}

void CObject::Update(void)
{
	CheckTimeout();
}

void CObject::LateUpdate(void)
{
}

void CObject::CheckTimeout(void)
{
	UINT32 currSeverTime = timerManager.GetCurrServerTime();

	if (m_maxLastTimeoutCheckTime < (currSeverTime - m_lastTimeoutCheckTime))
	{
		m_maxLastTimeoutCheckTime = (currSeverTime - m_lastTimeoutCheckTime);
	}

	if ((currSeverTime - m_lastTimeoutCheckTime) > dfNETWORK_PACKET_RECV_TIMEOUT)
	{
		m_bDead = true;
	}
}

void CObject::SetCurTimeout(void)
{
	m_lastTimeoutCheckTime = timerManager.GetCurrServerTime();
}
