#pragma once
#include "IActionState.h"

class AerialState : public IActionState
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	Vector3 velocity_ = {};

	float decreeSpeed_ = 2.0f;

	float jumpPower_ = 0.0f;

	int testCount = 0;
};

