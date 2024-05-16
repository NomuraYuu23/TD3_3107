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
		SpearIdle,	 // 待機
		SpearAim,	 // エイム開始
		SpearAiming, // エイム中
		SpearThrow,  // 槍投げ
		SpearBounce, // 槍のバウンド
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
	/// アニメーション再生関数
	/// </summary>
	/// <param name="animNum">再生するアニメーション番号</param>
	/// <param name="isLoop">アニメーションをループさせるか</param>
	/// <param name="isFinish">アニメーションを終了させるか</param>
	void PlayAnimation(int32_t animNum, bool isLoop = false, bool isFinish = true);

public: // アクセッサ等

	/// <summary>
	/// アニメーションゲッター
	/// </summary>
	/// <returns></returns>
	Animation& GetAnim() { return anim_; }

private: // プライベートなメンバ関数

	/// <summary>
	/// 全アニメーション停止関数
	/// </summary>
	void StopAnimationAll();

private: // メンバ変数

	// 武器本体
	Weapon*	weapon_ = nullptr;

	// アニメーション本体
	Animation anim_;

};
