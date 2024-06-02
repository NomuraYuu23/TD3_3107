#include "LockStop.h"
#include "../../ObjectList.h"

void LockStop::Initialize(Enemy* enemy)
{
	enemy_ = enemy;
}

void LockStop::Update()
{

	if (!acceptTimer_.IsActive() && enemy_->GetMoveStop()) {
		float correctFrame = 5.0f;
		acceptTimer_.Start(correctFrame);
	}

	acceptTimer_.Update();

	// 止めとくかどうか
	if (acceptTimer_.IsActive()) {
		enemy_->SetIsMoveStop(true);
	}
	else {
		enemy_->SetIsMoveStop(false);
	}


}
