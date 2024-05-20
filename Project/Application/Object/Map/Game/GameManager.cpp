#include "GameManager.h"

void GameManager::Initialize(Model* goalModel)
{
	assert(goalModel);
	goalModel_ = goalModel;

}

void GameManager::GenarateGoal(const Vector3& position)
{
	goal_ = std::make_unique<GoalObject>();
	goal_->Initialize(goalModel_);
	goal_->SetPosition(position);
}
