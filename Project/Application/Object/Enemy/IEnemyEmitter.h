#pragma once
#include "../../../Engine/3D/LargeNumberOfObjects.h"

class IEnemyEmitter : public LargeNumberOfObjects
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:
	WorldTransform worldTransform_;
	float distance_ = 0.0f;
	uint32_t maxCount_ = 0u;
};
