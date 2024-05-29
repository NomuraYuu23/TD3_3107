#pragma once
#include "IWeaponState.h"
#include "../../GameUtility/TimerLib.h"
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/Math/Vector2.h"

class FreeFallState : public IWeaponState
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
	void ImGuiUpdate() override {};

private:
	// 回転速度
	float rotateSpinSpeed_ = 0.0f;
	// 特有の重力レシオ
	float gravityRate_ = 0.0f;
};
