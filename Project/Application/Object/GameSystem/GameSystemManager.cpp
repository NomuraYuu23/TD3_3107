#include "GameSystemManager.h"
#include "../ObjectList.h"
#include "../Enemy/EnemyManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

uint32_t GameSystemManager::sNowStageNum = 0;
float GameSystemManager::sGameSpeed = 1.0f;

void GameSystemManager::Initialize(Model* goalModel, Model* checkPointModel, Player* player, GameAudioManager* gam)
{
	assert(goalModel);
	// ゴールなどのオブジェクト
	goalModel_ = goalModel;
	checkPointModel_ = checkPointModel;
	// プレイヤー
	player_ = player;

	// クリアフラグ
	isGameClear_ = false;
	isGameOver_ = false;

	checkPointManager_ = std::make_unique<CheckPointManager>();
	checkPointManager_->Initialize(player_, checkPointModel_, goalModel_, gam);

	//checkPointManager_->GenerateCheckPoint({ 10,0,0 }, 1);
	//checkPointManager_->GenerateCheckPoint({ 30,0,0 }, 2);
	//checkPointManager_->GenerateCheckPoint({ 50,0,0 }, 3);

	GenarateGoal({ 459.0f,78.0f,0 });
	deathHeight_ = GlobalVariables::GetInstance()->GetFloatValue("Common", "DeathHeight");
}

void GameSystemManager::Update()
{
	// チェックポイント管理更新
	checkPointManager_->Update();
	// ゴールオブジェクト更新
	goal_->Update();

	// 落下の死亡処理
	if (player_->worldtransform_.GetWorldPosition().y < deathHeight_) {
		player_->SetIsDead(true);
	}

	// クリアかゲームオーバーの処理を判断
	CheckGameStatus();

}

void GameSystemManager::CollisionRegister(Collision2DManager* collisionManager)
{
	// ゴールをコライダーに登録
	collisionManager->ListRegister(&goal_->boxCollider_);
	// チェックポイントをコライダーに登録
	checkPointManager_->CollisionRegister(collisionManager);
}

void GameSystemManager::GameOverProcess()
{
	goal_->SetIsGoal(false);
	player_->Reset(checkPointManager_->GetRespawnPosition());
	enemyManager_->LoadEnemyData();

}

void GameSystemManager::UpdateStageInfoOnCheckPoint()
{



}

void GameSystemManager::GameClearProcess()
{
	// クリア演出開始
	gum_->SetISClear(true);

	// プレイヤーを操作不能に
	player_->isGameClear_ = true;

	// クリア演出終了時
	if (gum_->GetClearAppear()) {
		isGameClear_ = true;
	}
}

void GameSystemManager::CheckGameStatus()
{
	// 死亡処理
	if (player_->IsDead()) {
		GameOverProcess();
	}
	// ゴール処理
	else if (goal_->IsGoal()) {
		GameClearProcess();
	}
}

void GameSystemManager::ImGuiDraw()
{
	ImGui::Begin("GameSystem");

	ImGui::DragFloat("sGameSpeed", &sGameSpeed);
	// ゴール
	goal_->ImGuiDraw();
	// チェックポイント
	checkPointManager_->ImGuiDraw();

	ImGui::End();

	checkPointManager_->GetCheckPointEditor()->ImGuiDraw();

}

void GameSystemManager::Draw(BaseCamera& camera)
{
	goal_->Draw(camera);

	checkPointManager_->Draw(camera);
}

void GameSystemManager::GenarateGoal(const Vector3& position)
{
	goal_ = std::make_unique<GoalObject>();
	goal_->Initialize(goalModel_);
	goal_->SetPosition();
}
