#pragma once
#include <numbers>
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
		jumping, // 落下中
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

private: // メンバ変数

	// プレイヤー本体
	Player* player_ = nullptr;

	// アニメーション本体
	Animation anim_;

};
