#pragma once
#include "IWeaponState.h"
#include "../../../Engine/Math/Vector3.h"

class ReturnState : public IWeaponState
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

	/// <summary>
	/// イージング（タイマー）更新
	/// </summary>
	void EaseUpdate();

private:
	// 座標
	Vector3 startPosition_ = {};
	// Lerpの補間レート
	float lerp_t = 0;

};

