#include "Collision2DManager.h"
#include "Collision2D.h"
#include "../../Application/Object/ObjectList.h"
#include "../../Application/Collider2D/CollisionConfig2D.h"

void Collision2DManager::Initialize()
{

	ListClear();

}

void Collision2DManager::ListClear()
{
	colliders_.clear();
}

void Collision2DManager::ListRegister(ColliderShape2D collider)
{

	colliders_.push_back(collider);

}

void Collision2DManager::CheakAllCollision()
{

	// リスト内のペアを総当たり
	std::list<ColliderShape2D>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		ColliderShape2D colliderA = *itrA;
		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<ColliderShape2D>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得する
			ColliderShape2D colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}

}

void Collision2DManager::CheckCollisionPair(ColliderShape2D colliderA, ColliderShape2D colliderB)
{

	std::visit([](const auto& a, const auto& b) {
		// 衝突フィルタリング
		//if (!(a->GetCollisionAttribute() & b->GetCollisionMask()) ||
		//	!(b->GetCollisionAttribute() & a->GetCollisionMask())) {
		//	return;
		//}

		if (a->GetCollisionAttribute() == b->GetCollisionAttribute()) {
			return;
		}

		if ((a->GetCollisionAttribute() == kCollisionAttributeEnemy && b->GetCollisionAttribute() == kCollisionAttributeTerrain) ||
			(a->GetCollisionAttribute() == kCollisionAttributeTerrain && b->GetCollisionAttribute() == kCollisionAttributeEnemy)) {
			return;
		}

		//if (a->GetCollisionAttribute() == kCollisionAttributeRay || b->GetCollisionAttribute() == kCollisionAttributeRay) {
		//	return;
		//}
		//if (b->GetCollisionAttribute() == kCollisionAttributeRay) {
		//	return;
		//}

		if (Collision2D::IsCollision(*a, *b)) {
			if (a->GetCollisionAttribute() == kCollisionAttributeRay) {
				// 衝突処理
				std::visit([=](const auto& x, const auto& y) {

					x->OnCollision(y);

					}, a->GetParentObject(), b->GetParentObject());
			}
			else if (b->GetCollisionAttribute() == kCollisionAttributeRay) {
				// 衝突処理
				std::visit([=](const auto& x, const auto& y) {

					y->OnCollision(x);

					}, a->GetParentObject(), b->GetParentObject());
			}
			else {
				// 衝突処理
				std::visit([=](const auto& x, const auto& y) {

					x->OnCollision(y);
					y->OnCollision(x);

					}, a->GetParentObject(), b->GetParentObject());
			}
		}
		}, colliderA, colliderB);

}
