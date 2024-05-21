#include "IBossState.h"
#include "../IBoss.h"
#include "../../../IObject.h"
#include "../../../GameSystem/GameSystemManager.h"

void IBossState::PreInitialize(IBoss* boss)
{
	// 対象設定
	boss_ = boss;

}

void IBossState::Update()
{
	// 座標更新
	boss_->worldtransform_.transform_.translate += boss_->velocity_ * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
}
