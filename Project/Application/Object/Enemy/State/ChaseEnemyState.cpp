#include "ChaseEnemyState.h"
#include "../../ObjectList.h"

void ChaseEnemyState::Initialize()
{
	enemy_->SetState(this);
	enemy_->MatrixUpdate();

}

void ChaseEnemyState::Update()
{
	// プレイヤー
	Vector2 player = { enemy_->GetPlayer()->worldtransform_.GetWorldPosition().x,enemy_->GetPlayer()->worldtransform_.GetWorldPosition().y };
	Vector2 enemy = { enemy_->GetWorldPosition().x,enemy_->GetWorldPosition().y };
	// レイの取得
	enemy_->detectionSystem_.sensorRay_.SetDirect(player, enemy);
	// 更新
	enemy_->detectionSystem_.Update();

}
