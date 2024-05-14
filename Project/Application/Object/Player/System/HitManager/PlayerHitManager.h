#pragma once
#include "../../../GameUtility/TimerLib.h"
#include <stdint.h>
#include "../../../Engine/Math/Vector2.h"

class Player;
/// <summary>
/// プレイヤーのHp管理クラスなので武器を持っていない際の衝突にしか対応しない
/// </summary>
class PlayerHitManager 
{
private:
	struct PlayerHealth {
		// 現在体力
		uint32_t current;
		// 最大体力
		uint32_t max;

		// 削る量
		uint32_t decreValue_;

	};
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player);
	/// <summary>
	/// 無敵タイマー処理とかに使う更新
	/// </summary>
	void Update();
	/// <summary>
	/// HPが削れる処理
	/// </summary>
	void OnHit();
	/// <summary>
	/// 引数で削るよう
	/// </summary>
	/// <param name="decrement"></param>
	void OnHit(uint32_t decrement);

	/// <summary>
	/// ImGUi
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// 無敵中か
	/// </summary>
	/// <returns></returns>
	bool InvisibleActive() { return invisibleTimer_.IsActive(); }

private:
	// 親
	Player* player_ = nullptr;
	// 無敵時間
	TimerLib invisibleTimer_;
	// 無敵フレーム数
	float invMaxFrame_;
	// HP
	PlayerHealth hp_;

	TimerLib hitEffectTimer_;
public:
	struct Effect {
		float time;
		Vector2 rShift, gShift, bShift;
		bool isStop;
		float nowFrame;
	};
	Effect hitEffect_;

	Effect defaultData_;
};
