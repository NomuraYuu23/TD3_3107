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

	void StartEasing(int frame) override;

	void EaseUpdate() override;
private:

	Vector3 startPosition_ = {};
	//Vector3 endPosition_ = {};

};

