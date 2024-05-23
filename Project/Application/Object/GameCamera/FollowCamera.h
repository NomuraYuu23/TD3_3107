#pragma once
#include "../../../Engine/Camera/BaseCamera.h"
#include "../GameUtility/TimerLib.h"
#include "../ObjectList.h"

class FollowCamera : public BaseCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(float elapsedTime = 0.0f) override;

public:
	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void ImGuiDraw();

	void SetPlayer(Player* player) {
		// プレイヤーのポインタ取得
		player_ = player;
		// 追従対象のポインタ設定
		targetTransform_ = &player_->worldtransform_;
	}

	/// <summary>
	/// カメラの引きを調整する関数
	/// </summary>
	void ScalingUpDown();

private:
	// プレイヤー
	Player* player_ = nullptr;
	// 対象のトランスフォーム
	WorldTransform* targetTransform_ = nullptr;

	// カメラのオフセット
	Vector3 defaultOffset_ = {};

	float defaultFovY_ = 0;
	float nowFovY_ = 0;

	TimerLib correctTimer_;

};
