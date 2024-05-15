#pragma once
#include "../Weapon.h"
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
	void SetScreenPosition(const Vector2& position,const BaseCamera& camera) {
		screenPositionRatio_ = position;
	}
private:
	Weapon* weapon_ = nullptr;
	Vector2 screenPositionRatio_ = {};

	std::unique_ptr<ShockWaveManager> shockWaveManager_;
};
