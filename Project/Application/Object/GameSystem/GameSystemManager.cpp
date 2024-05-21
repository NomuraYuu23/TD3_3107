#include "GameSystemManager.h"
#include "../ObjectList.h"
#include "../../../Engine/2D/ImguiManager.h"

uint32_t GameSystemManager::sNowStageNum = 0;
float GameSystemManager::sGameSpeed = 1.0f;

void GameSystemManager::Initialize(Model* goalModel)
{
	assert(goalModel);
	goalModel_ = goalModel;
	GenarateGoal({ 40.0f,5.0f,0 });
	deathHeight_ = -60.0f;
}

void GameSystemManager::Update(Player* player)
{
	goal_->Update();

	if (player->worldtransform_.GetWorldPosition().y < deathHeight_) {
		player->SetIsDead(true);
	}

	if (player->IsDead()) {
		player->Reset();
	}


}

void GameSystemManager::CollisionRegister(Collision2DManager* collisionManager)
{

	collisionManager->ListRegister(&goal_->boxCollider_);
	//&goal_
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
