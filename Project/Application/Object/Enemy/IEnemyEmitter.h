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

	void CreateEnemy(const Vector3& transformPosition, float distance, uint32_t enemyCount);

private:
	// シリアルナンバー
	uint32_t serialNum_ = 0;

	static uint32_t sSerialNumber_;

private:
	WorldTransform worldTransform_;
	float distance_ = 0.0f;
	uint32_t maxCount_ = 0u;
};
