#pragma once
#include "../../../../../Engine/Math/Vector2.h"

class Player;
class BaseCamera;

class SlowEffect
{

public: // サブクラス

	/// <summary>
	/// 状態一覧
	/// </summary>
	enum StateIndex {
		kStateIndexSlowNow = 0, // スロー中
		kStateIndexSlowEnd = 1, // スロー終了
		kStateIndexOfCount, // 数える用。使わない
	};

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Initalize(Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();
	
private: // 関数

	/// <summary>
	/// セットアップ
	/// </summary>
	void SetUp();

	/// <summary>
	/// スロー中更新
	/// </summary>
	void SlowNowUpdate();

	/// <summary>
	/// スロー終了更新
	/// </summary>
	void SlowEndUpdate();

public: // アクセッサ

	Vector2 GetCenter() { return center_; }

	Vector2 GetSize() { return size_; }

	float GetColorT() { return colorT_; }

	bool GetRunning() { return running_; }

	void SetCamera(BaseCamera* camera) { camera_ = camera; }

private: // 定数 （一旦変数）

	// 最大の大きさ
	static Vector2 kMaxSize;

	// 最大の大きくなる速度
	static float kMaxSizeVelocity;

	// 最大の色の変更速度
	static float kMaxColorVelocity;

	// 大きくなる加速度
	static float kSizeAcceleration_;

	// 色の変更加速度
	static float kColorAcceleration_;

private: // 変数

	// センター
	Vector2 center_ = {0.0f,0.0f};

	// 大きさ
	Vector2 size_ = {1.0f,1.0f};

	// 大きくなる速度
	float sizeVelocity_ = 0.0f;

	// 色の媒介変数
	float colorT_ = 0.0f;

	// 色の変更速度
	float colorVelocity_ = 0.0f;

	// プレイヤー
	Player* player_ = nullptr;

	// カメラ
	BaseCamera* camera_ = nullptr;

	// エフェクトを起動中か
	bool running_ = false;

	// 現在の状態
	StateIndex state_ = kStateIndexSlowEnd;

	// 前フレームの状態
	StateIndex preState_ = kStateIndexSlowEnd;

};

