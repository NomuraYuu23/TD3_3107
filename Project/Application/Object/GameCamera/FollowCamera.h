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

	/// <summary>
	/// 主に死亡演出終了後にリセットする関数
	/// </summary>
	void Reset();

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

	void MoveCameraForward();

	/// <summary>
	/// カメラのロック状態ゲッター
	/// </summary>
	/// <param name="isLock">カメラをロックするか</param>
	void SetLockCamera(const bool isLock) { isLockCamera_ = isLock; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetIsEndDeadCameraStaging() { return isEndDeadCameraStaging_; }

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

	// 入力クラス
	Input* input_ = nullptr;

	// カメラ固定
	bool isLockCamera_ = false;

private: // お試し

	// オフセット位置ターゲット
	Vector3 defaultOffsetTarget_;

	// オフセット位置追加分
	Vector3 defaultOffsetAdd_;

	// 補間係数
	float targetT_;

	// 大きさレート
	float scalingRate_ = 0.0f;
	float scalingRateT_ = 0.2f;

private: // 死亡演出時

	// 死亡演出時の秒数
	float currentForcusTime_ = 0.0f;
	float forcusTime_ = 2.0f;

	 // 演出開始時の座標
	Vector3 prevTranslate_;

	// カメラ自体の死亡演出が終了したか
	bool isEndDeadCameraStaging_ = false;
};
