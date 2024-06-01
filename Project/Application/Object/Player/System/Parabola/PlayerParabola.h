#pragma once
#include <cstdint>
#include "../../../../../Engine/3D/DrawLine.h"

class Player;

class PlayerParabola
{

public:

	void Initialize();

	void Update(
		const Vector3& playerPosition,
		const Vector3& direct, Player* player);

	void Reset();

	void DrawMap(DrawLine* drawLine);

public: // アクセッサ等

	Vector3 GetStartPos() { return startPositions_.front(); }
	Vector3 GetEndPos() { return endPositions_[numLine_ - 1]; }

private:

	// 線の最大数
	static const uint32_t kNumLineMax_ = 32;

	// 時間倍率
	static const uint32_t kTimeMagnification = 5;

	// 線の数
	uint32_t numLine_;

	// 線の開始位置
	std::array<Vector3, kNumLineMax_> startPositions_;

	// 線の終端位置
	std::array<Vector3, kNumLineMax_> endPositions_;

	//槍の速度
	float speed_;

};

