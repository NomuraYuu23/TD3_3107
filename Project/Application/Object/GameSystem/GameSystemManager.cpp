#include "GameSystemManager.h"
#include "../ObjectList.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

uint32_t GameSystemManager::sNowStageNum = 0;
float GameSystemManager::sGameSpeed = 1.0f;

void GameSystemManager::Initialize(Model* goalModel, Player* player)
{
	assert(goalModel);
	// ゴールなどのオブジェクト
	goalModel_ = goalModel;

	// プレイヤー
	player_ = player;

	GenarateGoal({ 40.0f,5.0f,0 });
	deathHeight_ = GlobalVariables::GetInstance()->GetFloatValue("Common", "DeathHeight");
}

void GameSystemManager::Update()
{
	goal_->Update();

	// 落下の死亡処理
	if (player_->worldtransform_.GetWorldPosition().y < deathHeight_) {
		player_->SetIsDead(true);
	}

	GameOverProcess();

}

void GameSystemManager::CollisionRegister(Collision2DManager* collisionManager)
{

	collisionManager->ListRegister(&goal_->boxCollider_);
	//&goal_
}

void GameSystemManager::GameOverProcess()
{
	if (player_->IsDead() || goal_->IsGoal()) {
		goal_->SetIsGoal(false);
		player_->Reset();
	}
}

void GameSystemManager::ImGuiDraw()
{
	ImGui::Begin("GameSystem");

	ImGui::DragFloat("sGameSpeed", &sGameSpeed);

	goal_->ImGuiDraw();


	ImGui::End();
}

void GameSystemManager::Draw(BaseCamera& camera)
{
	goal_->Draw(camera);
}

void GameSystemManager::GenarateGoal(const Vector3& position)
{
	goal_ = std::make_unique<GoalObject>();
	goal_->Initialize(goalModel_);
	goal_->SetPosition(position);
}
