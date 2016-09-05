#pragma once
#include "../RenderEngineBase.h"

class GameTime;

class GameClock : public boost::noncopyable
{
public:
	GameClock();

	const LARGE_INTEGER& StartTime() const;
	const LARGE_INTEGER& CurrentTime() const;
	const LARGE_INTEGER& LastTime() const;

	void Reset();
	double GetFrequency() const;
	void GetTime(LARGE_INTEGER& time) const;
	void UpdateGameTime(GameTime& gameTime);

private:
	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mCurrentTime;
	LARGE_INTEGER mLastTime;
	double mFrequency;
};
