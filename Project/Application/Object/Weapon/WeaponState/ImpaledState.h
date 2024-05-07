#pragma once
#include "IWeaponState.h"
#include "../System/AutoComeBack.h"

class ImpaledState : public IWeaponState
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
	// 自動的に戻ってくるシステム
	//AutoComeBack autoComeBack_;

	int easeTime_ = 0;


};

