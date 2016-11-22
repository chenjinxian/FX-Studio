#pragma once
#include "../TinyEngineBase.h"

class GameTime : boost::noncopyable
{
public:
	GameTime();

	void Reset();
	void UpdateGameTime();

	float GetTotalTime() const { return m_TotalTime; }
	float GetElapsedTime() const { return m_ElapsedTime; }
	uint32_t GetFrameRate() const { return m_FrameRate; }

private:
	LARGE_INTEGER m_StartTime;
	LARGE_INTEGER m_CurrentTime;
	LARGE_INTEGER m_LastTime;
	float m_Frequency;

	float m_TotalTime;
	float m_ElapsedTime;

	float m_FpsTime;
	uint32_t m_FrameCount;
	uint32_t m_FrameRate;
};
