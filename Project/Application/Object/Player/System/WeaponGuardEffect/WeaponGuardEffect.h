#pragma once
#include "../../../GameUtility/TimerLib.h"
#include "../../../GameUtility/MathUtility.h"
#include "../../../Engine/PostEffect/ShockWaveManager.h"

class Player;

class WeaponGuardEffect
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
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
	void SetScreenPosition(const BaseCamera& camera);

	void ImGuiDraw();

	void StartShockWave(float frame);

	bool IsActive() { return effectTimer_.IsActive(); }

	void SetCameraAddPosition(const Vector3 cameraAddPosition) { cameraAddPosition_ = cameraAddPosition; }

private:
	// 親
	Player* player_ = nullptr;
	// スクリーンの割合座標
	Vector2 screenPositionRatio_ = {};

	// 衝撃波マネ
	std::unique_ptr<ShockWaveManager> shockWaveManager_;

	// エフェクトの時間管理
	TimerLib effectTimer_;

	// 位置
	Vector3 position_ = { 0.0f,0.0f,0.0f };

	// カメラのずれ
	Vector3 cameraAddPosition_ = { 0.0f,0.0f,0.0f };

};

