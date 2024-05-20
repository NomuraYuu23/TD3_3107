#pragma once
#include "../../IObject.h"
#include "../Goal/GoalObject.h"

class GameManager
{
public:
	void Initialize(Model* goalModel);

private:
	void GenarateGoal(const Vector3& position);

private:
	// ゴール
	std::unique_ptr<GoalObject> goal_;

	Model* goalModel_ = nullptr;
	Model* checkPointMode_ = nullptr;

};