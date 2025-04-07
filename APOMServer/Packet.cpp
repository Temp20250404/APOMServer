
#include "pch.h"
#include "Packet.h"
#include <iostream>

CPacket::CPacket(int iBufferSize)
{
	m_iBufferSize = iBufferSize;
	m_iFront = m_iRear = 0;
}

CPacket::~CPacket()
{
	Clear();
}

void CPacket::Clear(void)
{
	if (m_iFront > m_iRear)
		DebugBreak();

	m_iFront = m_iRear = 0;
}

int CPacket::MoveWritePos(int iSize)
{
	m_iRear += iSize;

	return m_iRear;
}

int CPacket::MoveReadPos(int iSize)
{
	m_iFront += iSize;

	return m_iFront;
}

CPacket& CPacket::operator=(CPacket& clSrcPacket)
{
	m_iFront = clSrcPacket.m_iFront;
	m_iRear = clSrcPacket.m_iRear;
	memcpy(m_chpBuffer, clSrcPacket.m_chpBuffer, m_iRear);

	return *this;
}

int CPacket::GetData(char* chpDest, int iSize)
{
	std::memcpy(chpDest, m_chpBuffer + m_iFront, iSize);
	MoveReadPos(iSize);

	return 0;
}

int CPacket::PutData(const char* chpSrc, int iSrcSize)
{
	if ((m_iRear + iSrcSize) > m_iBufferSize)
	{
		// ��Ŷ�� �ִ� ������ ���� �� ���� ���� �� ����.
		DebugBreak();
	}

	std::memcpy(m_chpBuffer + m_iRear, chpSrc, iSrcSize);
	MoveWritePos(iSrcSize);

	return 0;
}
