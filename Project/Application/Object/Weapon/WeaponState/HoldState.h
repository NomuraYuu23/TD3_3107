#pragma once
#include "IWeaponState.h"

class HoldState : public IWeaponState
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override {};
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override {};
	/// <summary>
	/// ImGUi
	/// </summary>
	void ImGuiUpdate() override {};

};

