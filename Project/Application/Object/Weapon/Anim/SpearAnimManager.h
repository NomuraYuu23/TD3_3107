#pragma once
#include "../../../Engine/Animation/Animation.h"

// クラスの前方宣言
class Weapon;

/// <summary>
/// 槍アニメーション用マネージャー
/// </summary>
class SpearAnimManager
{
public: // サブクラス

	/// <summary>
	/// 槍アニメーション
	/// </summary>
	enum SpearAnim {
		SpearIdle,		// 待機
		SpearAim,		// エイム開始
		SpearAiming,    // エイム中
		SpearThrow,     // 槍投げ
		SpearBounce,    // 槍のバウンド
		SpearAnimCount, // アニメ数
	};

	/// <summary>
	/// リングアニメーション
	/// </summary>
	enum RingAnim {
		RingIdle,		// 待機
		RingShot,		// 投擲時
		RingAnimCount,	// アニメ数
	};

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="weapon">武器/param>
	void Init(Weapon* weapon);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 槍アニメーション再生関数
	/// </summary>
	/// <param name="animNum">再生するアニメーション番号</param>
	/// <param name="isLoop">アニメーションをループさせるか</param>
	/// <param name="isFinish">アニメーションを終了させるか</param>
	void PlaySpearAnimation(int32_t animNum, bool isLoop = false, bool isFinish = true);

	/// <summary>
	/// リングアニメーション再生関数
	/// </summary>
	/// <param name="animNum">再生するアニメーション番号</param>
	/// <param name="isLoop">アニメーションをループさせるか</param>
	/// <param name="isFinish">アニメーションを終了させるか</param>
	void PlayRingAnimation(int32_t animNum, bool isLoop = false, bool isFinish = true);

	/// <summary>
	/// リングアニメーションのセットアップ関数
	/// </summary>
	void SetUpRingAnim();

public: // アクセッサ等

	/// <summary>
	/// 槍アニメーションゲッター
	/// </summary>
	/// <returns>槍アニメーション</returns>
	Animation& GetSpearAnim() { return spearAnim_; }
	/// <summary>
	/// リングアニメーションゲッター
	/// </summary>
	/// <returns>リングアニメーション</returns>
	Animation& GetRingAnim() { return ringAnim_; }

private: // プライベートなメンバ関数

	/// <summary>
	/// 全槍アニメーション停止関数
	/// </summary>
	void StopSpearAnimationAll();

	/// <summary>
	/// 全リングアニメーション停止関数
	/// </summary>
	void StopRingAnimationAll();

private: // メンバ変数

	// 武器本体
	Weapon*	weapon_ = nullptr;

	// アニメーション本体
	Animation spearAnim_; // 槍
	Animation ringAnim_;  // リング

	// リングアニメーションのセットアップ済みフラグ
	bool isSetUpRingAnim_ = false;

};
