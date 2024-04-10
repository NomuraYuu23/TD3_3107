#include "CameraRay.h"
#include "../../Player.h"
#include "../../../ObjectList.h"
#include "../../../../Collider2D/CollisionConfig2D.h"

void CameraRay::Initialize(Player* player)
{
	player_ = player;
	direct_ = { player_->rayLength_,0};
	directRay_.Initialize(player_->position2D_, direct_, this);
	directRay_.SetCollisionAttribute(kCollisionAttributeRay);
	directRay_.SetCollisionMask(kCollisionAttributeTerrain);

}

void CameraRay::Update()
{
	// 地面
	player_->floorPrevY_ = minPositionY_;
	// レイの更新
	//direct_ = { /*player_->rayLength_*/0,player_->rayLength_ };
	direct_ = { 0,player_->rayLength_ };
	//direct_ = { player_->rayLength_,0 };
	directRay_.Update(player_->position2D_, direct_);
	//direct_ = player_->position2D_ + Vector2(0, -50.0f);
	//directRay_.Update(player_->position2D_, {0, direct_.y - player_->position2D_.y });

	//minPositionY_ = 0.1f;
}

void CameraRay::OnCollision(ColliderParentObject2D target)
{
	if (!std::holds_alternative<Terrain*>(target)) {
		return;
	}
	else {
		Vector2 targetPos = {};
		// 対象の情報取得
		std::visit([&](const auto& a) {
			targetPos = a->GetColliderPosition();
			}, target);

		if (player_->worldtransform_.GetWorldPosition().y > targetPos.y) {
			minPositionY_ = targetPos.y;
		}
		//if (minPositionY_ == 0.1f) {
		//	minPositionY_ = targetPos.y;
		//}
		//else if (minPositionY_ > targetPos.y) {
		//	minPositionY_ = targetPos.y;
		//}
	}
}

void CameraRay::ImGuiDraw()
{

	ImGui::DragFloat2("RayDirectPos", &direct_.x);
	Vector2 p2d = direct_ - player_->position2D_;
	p2d.x = 0;
	ImGui::DragFloat2("RayDirection", &p2d.x);

}
