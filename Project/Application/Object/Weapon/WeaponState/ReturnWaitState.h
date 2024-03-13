#pragma once
#include "IWeaponState.h"
#include "../../GameUtility/TimerLib.h"
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/Math/Vector2.h"

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

	TimerLib pressTimer_;

	Vector3 rotateVector_ = {};

	Vector2 highestPoint_ = {};
};