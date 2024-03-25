#pragma once
#include "IActionState.h"

class SpearAerialState : public IActionState
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
	float decreeSpeed_ = 2.0f;

	float gravity_ = 0;

};

