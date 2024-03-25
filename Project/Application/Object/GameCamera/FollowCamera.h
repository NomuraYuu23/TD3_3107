#pragma once
#include "../../../Engine/Camera/BaseCamera.h"
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
	void Update() override;

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

private:
	// プレイヤー
	Player* player_ = nullptr;
	// 対象のトランスフォーム
	WorldTransform* targetTransform_ = nullptr;

	// カメラのオフセット
	Vector3 defaultOffset_ = {};

};
