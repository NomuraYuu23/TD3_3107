#pragma once
#include "SmallBossStateList.h"

class SmallBossMoveState : public IBossState
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;


};