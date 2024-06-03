#pragma once
#include "../IObject.h"
#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../Map/CheckPoint/CheckPointManager.h"
#include "../../../Application/AudioManager/GameAudioManager.h"
#include "../../UI/GameUIManager.h"

class GoalObject;
class Player;
class FollowCamera;
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
	void Initialize(Model* goalModel, Model* checkPointModel, Player* player, GameAudioManager* gam);
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

	/// <summary>
	/// 
	/// </summary>
	void UpdateStageInfoOnCheckPoint();

	/// <summary>
	/// クリア時
	/// </summary>
	void GameClearProcess();

	/// <summary>
	/// クリア処理とゲームオーバー処理の判断、関数呼び出しを行う関数
	/// </summary>
	void CheckGameStatus();

public: // アクセッサ
	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; }

	bool GetIsGameClear() { return isGameClear_; }

	/// <summary>
	/// ゲーム画面UIマネージャーセッター
	/// </summary>
	/// <param name="uiManager">ゲーム画面UIマネージャー</param>
	void SetGameUIManager(GameUIManager* uiManager) { gum_ = uiManager; }

	/// <summary>
	/// 追従カメラセッター
	/// </summary>
	/// <param name="camera">追従カメラ</param>
	void SetFollowCamera(FollowCamera* camera) { camera_ = camera; };

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

	// ゲーム画面UIマネージャー
	GameUIManager* gum_ = nullptr;

	// 追従カメラ
	FollowCamera* camera_ = nullptr;

private: // 死亡演出用変数

	// 死亡演出進捗
	enum DeadStagingProgress {
		kDeadStagingSetUp,
		kCameraMove, // カメラをプレイヤーに向けて動かす、集まっているようなパーティクル
		kPaticleBlast, // パーティクル爆散、プレイヤー非表示
		kFadeOut, // 暗転、リスポーン
	};

	// 死亡演出トリガー
	bool isEndDeadStaging_ = false;

	// 死亡演出進捗
	int deadStagingProgress_ = kDeadStagingSetUp;

	// 爆散時の表示時間
	float currentBlastTime_ = 0.0f;
	float blastTime_		= 1.0f;

};