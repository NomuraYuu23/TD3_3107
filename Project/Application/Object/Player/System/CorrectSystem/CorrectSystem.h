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

	void ImGuiDraw();

private:

	void NearLockOn(EnemyManager* enemyManager);

	// プレイヤーポインタ
	Player* player_;

	Vector3 NearEnemyLockOn(EnemyManager* enemyManager);

	Vector3 StickAimAssist(EnemyManager* enemyManager, const Vector3& stickDirect);

	Vector3 LeftStickAimAssist(EnemyManager* enemyManager, const Vector3& stickDirect);

	Vector2 prevLeftStick_ = {};

	bool isInNearArea_ = false;

	Vector3 targetDirect_ = {};

	OneOfManyObjects* targetPointer_ = nullptr;
};
