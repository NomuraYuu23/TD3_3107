#pragma once
#include "../../../Engine/3D/LargeNumberOfObjects.h"

class IEnemyEmitter : public LargeNumberOfObjects
{
private:
	// 共通の番号
	static uint32_t sSerialNumber_;

	// シリアルナンバー
	uint32_t serialNum_ = 0;

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
	// 
	std::string name_;

private:
	WorldTransform worldTransform_;
	float distance_ = 0.0f;
	uint32_t maxCount_ = 0u;
};
