#pragma once
#include "../../../Engine/Math/Vector3.h"

/// <summary>
/// 前方宣言
/// </summary>
class Enemy;

/// <summary>
/// プレイヤーを探すシステム
/// </summary>
class DetectionSystem 
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="enemy"></param>
	/// <param name="player"></param>
	void Initialize(Enemy* enemy);

	/// <summary>
	/// 追尾判断
	/// </summary>
	/// <param name="target"></param>
	/// <returns></returns>
	bool ShouldChase(const Vector3& target);

	/// <summary>
	/// 追尾するかフラグ
	/// </summary>
	/// <returns></returns>
	bool IsChase() { return isChase_; }

private:
	// エネミー本体
	Enemy* enemy_;

	bool isChase_ = false;
};