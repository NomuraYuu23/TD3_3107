#include "ChaseEnemyState.h"
#include "../../ObjectList.h"

void ChaseEnemyState::Initialize()
{
	enemy_->SetState(this);
	enemy_->MatrixUpdate();
	enemy_->usedDirection_ = true;
	sensorRay_.Initialize(enemy_);

}

void ChaseEnemyState::Update()
{
	// 停止中なら早期
	if (enemy_->GetMoveStop()) {
		return;
	}

	// プレイヤー
	Vector2 player = { enemy_->GetPlayer()->worldtransform_.GetWorldPosition().x,enemy_->GetPlayer()->worldtransform_.GetWorldPosition().y };
	Vector2 enemy = { enemy_->GetWorldPosition().x,enemy_->GetWorldPosition().y };
	// 更新
	enemy_->detectionSystem_.Update(sensorRay_.IsCollision());

	// レイの取得
	sensorRay_.SetDirect(enemy, player);
	sensorRay_.Update();
}

void ChaseEnemyState::PlayerChaseUpdate()
{



}

void ChaseEnemyState::SetChaseSetting(float length)
{
	// 索敵範囲
	enemy_->detectionSystem_.SetSearchLength(length);
	//searchLength_ = length;
}
