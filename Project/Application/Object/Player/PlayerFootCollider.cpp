#include "PlayerFootCollider.h"
#include "../ObjectList.h"
#include "../../Collider2D/CollisionConfig2D.h"

void PlayerFootCollider::Initialize(Model* model, Player* parent)
{
	
	model_ = model;
	player_ = parent;

	// マテリアル
	enableLighting_ = 0;
	shininess_ = 100.0f;

	material_.reset(Material::Create());
	material_->SetEnableLighting(enableLighting_);
	material_->SetShininess(shininess_);

	// 行列
	worldtransform_.Initialize(model_->GetRootNode());
	worldtransform_.UpdateMatrix();

	// 2D用座標・サイズ
	position2D_ = { worldtransform_.GetWorldPosition().x,worldtransform_.GetWorldPosition().y };
	scale2D_ = { 0.95f*1.95f, 0.2f };

	worldtransform_.SetParent(&player_->worldtransform_);
	worldtransform_.transform_.translate.y = -(player_->circleCollider_.radius_ + 0.05f);

	// コライダー
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, 0, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributePlayer);
	boxCollider_.SetCollisionMask(kCollisionAttributeTerrain);
}

void PlayerFootCollider::Update()
{
	// 行列・座標更新
	worldtransform_.UpdateMatrix();
	position2D_ = { worldtransform_.GetWorldPosition().x,worldtransform_.GetWorldPosition().y};
	boxCollider_.Update(position2D_, scale2D_.x, scale2D_.y, 0);

	player_->isGround_ = false;

}

void PlayerFootCollider::ImGuiDraw()
{

	ImGui::DragFloat3("footPos", &worldtransform_.transform_.translate.x);
	ImGui::DragFloat3("footScale", &worldtransform_.transform_.scale.x);
	ImGui::DragFloat2("coPos", &position2D_.x);
	ImGui::DragFloat2("scale2D", &scale2D_.x);

}

void PlayerFootCollider::OnCollision(ColliderParentObject2D target)
{
	if (!std::holds_alternative<Terrain*>(target)) {
		return;
	}
	else {
		player_->isGround_ = true;
	}

}

void PlayerFootCollider::DebugDraw(BaseCamera camera)
{
	model_->Draw(worldtransform_, camera, material_.get());
}
