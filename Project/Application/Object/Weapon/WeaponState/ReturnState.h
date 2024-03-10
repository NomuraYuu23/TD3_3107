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
	/// 開始設定
	/// </summary>
	/// <param name="frame"></param>
	void StartEasing(int frame) override;
	/// <summary>
	/// イージング（タイマー）更新
	/// </summary>
	void EaseUpdate() override;

private:

	Vector3 startPosition_ = {};
	//Vector3 endPosition_ = {};

};

