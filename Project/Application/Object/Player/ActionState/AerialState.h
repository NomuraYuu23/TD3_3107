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

};

