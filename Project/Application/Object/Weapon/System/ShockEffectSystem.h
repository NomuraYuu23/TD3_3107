#pragma once
#include "../Weapon.h"
#include "../../GameUtility/TimerLib.h"
#include "../../GameUtility/MathUtility.h"
#include "../../../Engine/PostEffect/ShockWaveManager.h"

class ShockEffectSystem 
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="weapon"></param>
	void Initialize(Weapon* weapon);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 衝撃波マネージャー取得
	/// </summary>
	/// <returns></returns>
	ShockWaveManager* GetShockWaveManager() { return shockWaveManager_.get(); }
	/// <summary>
	/// スクリーン内のどの位置かを設定
	/// </summary>
	/// <param name="position"></param>
	void SetScreenPosition(const BaseCamera& camera) {
		screenPosition_ = MathUtility::WorldToScreen(weapon_->worldtransform_.GetWorldPosition(), &static_cast<BaseCamera>(camera));
		screenPositionRatio_ = MathUtility::ScreenPositionRatio(weapon_->worldtransform_.GetWorldPosition(), &static_cast<BaseCamera>(camera));
	}

	void ImGuiDraw();

private:
	// 親
	Weapon* weapon_ = nullptr;
	// スクリーンの割合座標
	Vector2 screenPositionRatio_ = {};
	Vector2 screenPosition_ = {};

	// 衝撃波マネ
	std::unique_ptr<ShockWaveManager> shockWaveManager_;

	// エフェクトの時間管理
	TimerLib effectTimer_;

};
