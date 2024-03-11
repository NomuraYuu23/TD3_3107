#pragma once
#include "IWeaponState.h"
#include "../../GameUtility/TimerLib.h"
#include "../../../Engine/Math/Vector3.h"

class ThrownState : public IWeaponState
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
	/// ImGUi
	/// </summary>
	void ImGuiUpdate() override;

private:
	// 速度ベクトル
	Vector3 velocity_;
	// 速さの値
	float speedValue_;
	// 槍の速さ調整用タイマー
	TimerLib acceleratorTimer_;

};

