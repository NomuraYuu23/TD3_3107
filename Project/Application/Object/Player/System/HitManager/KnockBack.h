#pragma once
#include "../../../GameUtility/TimerLib.h"
#include "../../../Engine/Math/Vector3.h"

class Player;

class KnockBack
{
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="direction"></param>
	void CreateKnockBack(const Vector3& direction);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 無敵中か
	/// </summary>
	/// <returns></returns>
	bool AcceptActive() { return AcceptTimer_.IsActive(); }

	/// <summary>
	/// 着地するタイミングでリセット
	/// </summary>
	/// <returns></returns>
	bool IsHit() { return isHit_; }

	void SetIsHit(bool isHit) { isHit_ = isHit; }

private:
	// プレイヤー
	Player* player_ = nullptr;
	// 受け付けた際の大麻ー
	TimerLib AcceptTimer_;

	// とりあえずパワ～～～
	Vector3 knockBackPower_ = {};
	Vector3 moveDirection_ = {};

	bool isHit_ = false;

};