#pragma once
#include "../../../Engine/3D/LargeNumberOfObjects.h"

class EnemyEmitter : public LargeNumberOfObjects
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

};
