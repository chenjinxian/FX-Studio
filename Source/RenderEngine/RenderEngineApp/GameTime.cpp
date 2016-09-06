#include "GameTime.h"

GameTime::GameTime()
	: m_TotalGameTime(0.0), m_ElapsedGameTime(0.0)
{
}

double GameTime::TotalGameTime() const
{
	return m_TotalGameTime;
}

void GameTime::SetTotalGameTime(double totalGameTime)
{
	m_TotalGameTime = totalGameTime;
}

double GameTime::ElapsedGameTime() const
{
	return m_ElapsedGameTime;
}

void GameTime::SetElapsedGameTime(double elapsedGameTime)
{
	m_ElapsedGameTime = elapsedGameTime;
}
