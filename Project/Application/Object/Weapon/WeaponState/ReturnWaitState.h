#pragma once
#include "IWeaponState.h"
#include "../../GameUtility/TimerLib.h"

class ReturnWaitState : public IWeaponState
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
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiUpdate() override;

private:
	// 終了タイマー用
	TimerLib endTimer_;
};