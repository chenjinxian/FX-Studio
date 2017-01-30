#pragma once
#include "../TinyEngineBase.h"

class SpatialSort
{
public:

	SpatialSort();
	SpatialSort(const std::vector<Vector3> positions);
	~SpatialSort();

	void Fill(const std::vector<Vector3> positions, bool pFinalize = true);
	void Append(const std::vector<Vector3> positions, bool pFinalize = true);
	void Finalize();

	void FindPositions(const Vector3& position, float radius, std::vector<uint32_t>& results) const;
	void FindIdenticalPositions(const Vector3& position, std::vector<uint32_t>& results) const;
	uint32_t GenerateMappingTable(std::vector<uint32_t>& fill, float radius) const;

private:
	Vector3 m_PlaneNormal;

	struct Entry
	{
		uint32_t Index;
		Vector3 Position;
		float Distance;

		Entry() {}
		Entry(uint32_t index, const Vector3& position, float distance)
			: Index(index), Position(position), Distance(distance)
		{}

		bool operator < (const Entry& e) const { return Distance < e.Distance; }
	};

	std::vector<Entry> m_Positions;
};
