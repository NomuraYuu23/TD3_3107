#pragma once
#include "IWeaponState.h"
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

	Vector3 velocity_;
	float speedValue_;

};

