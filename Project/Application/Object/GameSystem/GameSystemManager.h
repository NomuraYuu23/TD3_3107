#pragma once
#include "../IObject.h"
#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../Map/CheckPoint/CheckPointManager.h"

class GoalObject;
class Player;
class EnemyManager;
/// <summary>
/// チェックポイント管理クラス
/// </summary>
//class CheckPointManager;

/// <summary>
/// ゲームシステム関係をまとめたクラス
/// </summary>
class GameSystemManager
{
public:
	static uint32_t sNowStageNum;
	static float sGameSpeed;

	// ここでプレイヤーのスポーン関数を作成
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="goalModel"></param>
	void Initialize(Model* goalModel, Player* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(BaseCamera& camera);
	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// コライダー登録
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionRegister(Collision2DManager* collisionManager);

	/// <summary>
	/// ゲームオーバー時の処理
	/// </summary>
	void GameOverProcess();


public: // アクセッサ
	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; }

private:
	/// <summary>
	/// ゴール生成関数
	/// </summary>
	/// <param name="position"></param>
	void GenarateGoal(const Vector3& position);

private:
	// ゲームクリア
	bool isGameClear_ = false;
	// ゲームオーバー
	bool isGameOver_ = false;

	// プレイヤーの死ぬ高さ
	float deathHeight_ = 0.0f;

private:
	// ゴール
	std::unique_ptr<GoalObject> goal_;

	std::unique_ptr<CheckPointManager> checkPointManager_;

	// プレイヤー
	Player* player_ = nullptr;
	// 敵の管理クラス
	EnemyManager* enemyManager_ = nullptr;

	Model* goalModel_ = nullptr;
	Model* checkPointModel_ = nullptr;

};