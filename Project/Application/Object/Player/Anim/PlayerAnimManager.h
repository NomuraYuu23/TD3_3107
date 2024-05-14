#pragma once
#include <numbers>
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Animation/Animation.h"

// クラスの前方宣言
class Player;

/// <summary>
/// プレイヤー用アニメーションマネージャー
/// </summary>
class PlayerAnimManager {
public: // サブクラス

	/// <summary>
	/// 再生中アニメーション
	/// </summary>
	enum playingAnim {
		Idle, // 待機
		Run, // 走り
		JumpStart, // ジャンプ開始
		Jumping, // 落下中
		Landing, // 着地
		ThrowStart, // 投げ開始
		ThrowStaying, // 投げ待機
		ThrowSpear, // 槍投げ
		SpearJump, // 槍ジャンプ
	};

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Init(Player* player);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// アニメーション再生関数
	/// </summary>
	/// <param name="animNum">再生するアニメーション番号</param>
	/// <param name="isLoop">アニメーションをループさせるか</param>
	/// <param name="isFinish">アニメーションを終了させるか</param>
	void PlayAnimation(int32_t animNum, bool isLoop = false, bool isFinish =true);

public: // アクセッサ等

	/// <summary>
	/// プレイヤー方向ゲッター
	/// </summary>
	/// <returns>プレイヤー方向</returns>
	bool GetIsRight() { return isRight_; }

private: // プライベートなメンバ関数

	/// <summary>
	/// 全アニメーション停止関数
	/// </summary>
	void StopAnimationAll();

private: // メンバ変数

	// 入力管理クラス
	Input* input_ = nullptr;

	// プレイヤー本体
	Player* player_ = nullptr;

	// アニメーション本体
	Animation anim_;

	// プレイヤーが右を向いているか
	bool isRight_ = true;

};
