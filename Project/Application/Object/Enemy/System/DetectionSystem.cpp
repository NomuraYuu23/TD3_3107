#include "DetectionSystem.h"
#include "../../ObjectList.h"

void DetectionSystem::Initialize(Enemy* enemy)
{
	// それぞれのポインタを渡す
	enemy_ = enemy;

}

void DetectionSystem::Update(bool isRayCollision)
{
	if (std::holds_alternative<ChaseEnemyState*>(enemy_->GetState())) {
		if (!isRayCollision) {
			// 追尾フラグ設定
			isChase_ = ShouldChase(enemy_->GetPlayer()->worldtransform_.GetWorldPosition(), searchLength_);
		}
		else {
			isChase_ = false;
		}
	}

	if (isChase_) {
		ChaseUpdate();
	}
}

bool DetectionSystem::ShouldChase(const Vector3& target, float searchLength)
{
	// それぞれの座標
	Vector3 playerPosition = target;
	Vector3 enemyPosition = enemy_->GetWorldPosition();
	// 距離の長さ
	float length = Vector3::Length(playerPosition - enemyPosition);

	if (length < searchLength) {
		return true;
	}

	return false;
}

void DetectionSystem::ChaseUpdate()
{

	Vector3 velocity = enemy_->GetPlayer()->worldtransform_.GetWorldPosition() - enemy_->GetWorldPosition();
	velocity = Vector3::Normalize(velocity);

	float moveRate = 5.0f;

	// 追尾計算（今後変更予定
	enemy_->transform_.translate.x += velocity.x * (1.0f / 60.0f) * (1.0f / GameSystemManager::sGameSpeed) * moveRate;
	enemy_->transform_.translate.y += velocity.y * (1.0f / 60.0f) * (1.0f / GameSystemManager::sGameSpeed) * moveRate;

	// 方向
	enemy_->direction_ = { velocity.x,velocity.y };

	enemy_->MatrixUpdate();
}
