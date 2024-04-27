#pragma once
#include "../../../Engine/Math/Vector2.h"

class RayContactRecord
{
public:
	void AddHistory(uint32_t number, Vector2 position);

	bool HistoricalComparison(uint32_t number);

};
