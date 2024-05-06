#pragma once
#include "../../../Engine/Collider2D/Circle.h"
#include "../../../Enemy/EnemyManager.h"

class Player;

class CorrectSystem
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Initialize(Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="enemyManager">エネミーマネージャー</param>
	void Update(EnemyManager* enemyManager);

private:

	// 確認する範囲（半径）
	const float kInitLengthMin_ = 30.0f;

	// 回転幅
	const float kRotationWidth_ = 0.15f;

	// プレイヤーポインタ
	Player* player_;

	Vector3 NearEnemyLockOn(EnemyManager* enemyManager);

	Vector2 prevLeftStick_ = {};

	bool isInNearArea_ = false;
};
