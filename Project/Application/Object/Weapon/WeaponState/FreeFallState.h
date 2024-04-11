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
	// 
	Vector3 directPosition_ = {};

	// 方向ベクトルの回転
	Vector3 rotateVector(const Vector3& direct, float theta);

};
