#include "GameTime.h"

GameTime::GameTime()
	: m_StartTime(),
	m_CurrentTime(),
	m_LastTime(),
	m_Frequency(0.0f),
	m_TotalTime(0.0f),
	m_ElapsedTime(0.0f),
	m_FpsTime(0.0f),
	m_FrameCount(0),
	m_FrameRate(0)
{
	Reset();
}

void GameTime::Reset()
{
	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency))
	{
		DEBUG_ERROR("QueryPerformanceFrequency() failed.");
	}
	m_Frequency = (float)frequency.QuadPart;

	QueryPerformanceCounter(&m_StartTime);
	m_CurrentTime = m_StartTime;
	m_LastTime = m_CurrentTime;

	m_FpsTime = 0.0f;
	m_FrameCount = 0;
	m_FrameRate = 0;
}

void GameTime::UpdateGameTime()
{
	QueryPerformanceCounter(&m_CurrentTime);

	m_TotalTime = (m_CurrentTime.QuadPart - m_StartTime.QuadPart) / m_Frequency;
	m_ElapsedTime = (m_CurrentTime.QuadPart - m_LastTime.QuadPart) / m_Frequency;
	m_LastTime = m_CurrentTime;

	if (m_TotalTime - m_FpsTime >= 1.0f)
	{
		m_FpsTime = m_TotalTime;
		m_FrameRate = m_FrameCount;
		m_FrameCount = 0;
	}

	m_FrameCount++;
}
