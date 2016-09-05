#pragma once
#include "../RenderEngineBase.h"

class GameTime
{
public:
	GameTime();

	double TotalGameTime() const;
	void SetTotalGameTime(double totalGameTime);

	double ElapsedGameTime() const;
	void SetElapsedGameTime(double elapsedGameTime);

private:
	double m_TotalGameTime;
	double m_ElapsedGameTime;
};
