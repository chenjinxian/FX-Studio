#pragma once
#include "../TinyEngineBase.h"

class GameTime : boost::noncopyable
{
public:
	GameTime();

	void Reset();
	void UpdateGameTime();

	double GetTotalTime() const { return m_TotalTime; }
	double GetElapsedTime() const { return m_ElapsedTime; }
	uint32_t GetFrameRate() const { return m_FrameRate; }

private:
	LARGE_INTEGER m_StartTime;
	LARGE_INTEGER m_CurrentTime;
	LARGE_INTEGER m_LastTime;
	double m_Frequency;

	double m_TotalTime;
	double m_ElapsedTime;

	double m_FpsTime;
	uint32_t m_FrameCount;
	uint32_t m_FrameRate;
};
