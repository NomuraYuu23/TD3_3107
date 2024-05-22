#include "DetectionSystem.h"
#include "../Enemy.h"

void DetectionSystem::Initialize(Enemy* enemy)
{
	// それぞれのポインタを渡す
	enemy_ = enemy;

}

bool DetectionSystem::ShouldChase(const Vector3& target)
{
	// それぞれの座標
	Vector3 playerPosition = target;
	Vector3 enemyPosition = enemy_->GetWorldPosition();
	// 距離の長さ
	float length = Vector3::Length(playerPosition - enemyPosition);

	float minLength = 50.0f;

	if (length < minLength) {
		return false;
	}

	return false;
}
