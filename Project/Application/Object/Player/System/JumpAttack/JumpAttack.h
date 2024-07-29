#pragma once
#include "../../../GameUtility/TimerLib.h"
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/Math/Vector2.h"

class Player;

class JumpAttack {
public:
	/// <summary>
	/// 初期化（設定
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player) {
		player_ = player;
	}
	/// <summary>
	/// 開始受付
	/// </summary>
	/// <param name="invincibleFrame"></param>
	void StartAccept(float invincibleFrame = 60.0f);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 中断
	/// </summary>
	void Cancel();

	/// <summary>
	/// 起動中か
	/// </summary>
	/// <returns></returns>
	bool IsActive() { return timer_.IsActive(); }

private:
	// 親
	Player* player_ = nullptr;
	// 攻撃中のフラグ
	TimerLib timer_;
};