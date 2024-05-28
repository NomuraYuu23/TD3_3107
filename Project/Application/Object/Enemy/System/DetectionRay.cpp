#include "DetectionRay.h"
#include "../../ObjectList.h"
#include "../../../Collider2D/CollisionConfig2D.h"

void DetectionRay::Initialize(Enemy* enemy)
{
	// ポインタ
	enemy_ = enemy;

	// レイ設定
	directRay_.Initialize(enemy_->position2D_, direct_, this);
	directRay_.SetCollisionAttribute(kCollisionAttributeRay);
	directRay_.SetCollisionMask(kCollisionAttributeTerrain);
}

void DetectionRay::Update()
{

	// 初期化
	isCollision_ = false;

	// レイの更新
	directRay_.Update(enemy_->position2D_, direct_);

}

void DetectionRay::OnCollision(ColliderParentObject2D target)
{
	if (std::holds_alternative<Terrain*>(target)) {
		isCollision_ = true;
	}
}
