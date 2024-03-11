#pragma once
#include "IActionState.h"

class GroundState : public IActionState
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
	// 移動
	float moveSpeed_;

	bool isFall_ = false;

};

