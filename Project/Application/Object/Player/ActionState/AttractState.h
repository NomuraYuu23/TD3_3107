#pragma once
#include "IActionState.h"
#include "../../GameUtility/TimerLib.h"

class AttractState : public IActionState
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
	// 引き寄せのタイマー
	TimerLib attractTimer_;
	// 開始座標
	Vector3 startPosition_ = {};
	// 方向ベクトル
	Vector3 jumpDirection_ = {};
};