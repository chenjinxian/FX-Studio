
#include "SpatialSort.h"

SpatialSort::SpatialSort()
	: m_PlaneNormal(0.8523f, 0.34321f, 0.5736f)
{
	m_PlaneNormal.Normalize();
}

SpatialSort::SpatialSort(const std::vector<Vector3> positions)
	: m_PlaneNormal(0.8523f, 0.34321f, 0.5736f)
{
	m_PlaneNormal.Normalize();
	Fill(positions);
}

SpatialSort::~SpatialSort()
{

}

void SpatialSort::Fill(const std::vector<Vector3> positions, bool pFinalize /*= true*/)
{
	m_Positions.clear();
	Append(positions, pFinalize);
}

void SpatialSort::Append(const std::vector<Vector3> positions, bool pFinalize /*= true*/)
{
	const size_t initial = m_Positions.size();
	m_Positions.reserve(initial + (pFinalize ? positions.size() : positions.size() * 2));

	for (uint32_t i = 0; i < positions.size(); i++)
	{
		const Vector3& vec = positions[i];
		float distance = vec.Dot(m_PlaneNormal);
		m_Positions.push_back(Entry(i + initial, vec, distance));
	}

	if (pFinalize)
	{
		Finalize();
	}
}

void SpatialSort::Finalize()
{
	std::sort(m_Positions.begin(), m_Positions.end());
}

void SpatialSort::FindPositions(const Vector3& position, float radius, std::vector<uint32_t>& results) const
{
	const float dist = position.Dot(m_PlaneNormal);
	const float minDist = dist - radius, maxDist = dist + radius;

	results.clear();

	if (m_Positions.empty() || maxDist < m_Positions.front().Distance || minDist > m_Positions.front().Distance)
		return;

	uint32_t index = (uint32_t)m_Positions.size() / 2;
	uint32_t binaryStepSize = (uint32_t)m_Positions.size() / 4;
	while (binaryStepSize > 1)
	{
		if (m_Positions[index].Distance < minDist)
			index += binaryStepSize;
		else
			index -= binaryStepSize;

		binaryStepSize /= 2;
	}

	while (index > 0 && m_Positions[index].Distance > minDist)
	{
		index--;
	}
	while (index < (m_Positions.size() - 1) && m_Positions[index].Distance < minDist)
	{
		index++;
	}

	const float squared = radius * radius;
	std::vector<Entry>::const_iterator it = m_Positions.begin() + index;
	while (it->Distance < maxDist)
	{
		if ((it->Position - position).LengthSquared() < squared)
		{
			results.push_back(it->Index);
		}
		++it;
		if (it == m_Positions.end())
		{
			break;
		}
	}
}

static int32_t ToBinary(const float& pValue)
{
	static_assert(sizeof(int32_t) >= sizeof(float), "sizeof(int32_t) >= sizeof(float)");

	static_assert(sizeof(int32_t) == sizeof(float), "sizeof(int32_t) == sizeof(float)");
	const int32_t binValue = reinterpret_cast<int32_t const &>(pValue);

	if ((-42 == (~42 + 1)) && (binValue & 0x80000000))
		return int32_t(1 << (CHAR_BIT * sizeof(int32_t) - 1)) - binValue;
	else if ((-42 == ~42) && (binValue & 0x80000000))
		return int32_t(-0) - binValue;
	else if ((-42 == (42 | (-0))) && (binValue & 0x80000000)) // -0 = 1000... binary
		return binValue;
	else
		return binValue;
}

void SpatialSort::FindIdenticalPositions(const Vector3& position, std::vector<uint32_t>& results) const
{
	static const int32_t toleranceInULPs = 4;
	static const int32_t distanceToleranceInULPs = toleranceInULPs + 1;
	static const int32_t distance3DToleranceInULPs = distanceToleranceInULPs + 1;

	const int32_t minDistBinary = ToBinary(position.Dot(m_PlaneNormal)) - distanceToleranceInULPs;
	const int32_t maxDistBinary = minDistBinary + 2 * distanceToleranceInULPs;

	results.clear();

	uint32_t index = (uint32_t)m_Positions.size() / 2;
	uint32_t binaryStepSize = (uint32_t)m_Positions.size() / 4;
	while (binaryStepSize > 1)
	{
		if (minDistBinary > ToBinary(m_Positions[index].Distance))
			index += binaryStepSize;
		else
			index -= binaryStepSize;

		binaryStepSize /= 2;
	}

	while (index > 0 && minDistBinary < ToBinary(m_Positions[index].Distance))
		index--;
	while (index < (m_Positions.size() - 1) && minDistBinary > ToBinary(m_Positions[index].Distance))
		index++;

	std::vector<Entry>::const_iterator it = m_Positions.begin() + index;
	while (ToBinary(it->Distance) < maxDistBinary)
	{
		if (distance3DToleranceInULPs >= ToBinary((it->Position - position).LengthSquared()))
			results.push_back(it->Index);
		++it;
		if (it == m_Positions.end())
			break;
	}
}

uint32_t SpatialSort::GenerateMappingTable(std::vector<uint32_t>& fill, float radius) const
{
	fill.resize(m_Positions.size(), UINT_MAX);
	
	uint32_t t = 0;
	const float squared = radius * radius;

	for (size_t i = 0; i < m_Positions.size();)
	{
		float maxDist = m_Positions[i].Position.Dot(m_PlaneNormal) + radius;

		fill[m_Positions[i].Index] = t;
		const Vector3& oldPos = m_Positions[i].Position;
		i++;

		for (; i < fill.size() && m_Positions[i].Distance < maxDist && (m_Positions[i].Position - oldPos).LengthSquared() < squared; i++)
		{
			fill[m_Positions[i].Index] = t;
		}

		t++;
	}

	for (auto index : fill)
	{
		DEBUG_ASSERT(index < m_Positions.size());
	}

	return t;
}
