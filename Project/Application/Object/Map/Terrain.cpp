#include "Terrain.h"
#include "../../Collider2D/CollisionConfig2D.h"
#include "../../../Engine/2D/ImguiManager.h"

uint32_t Terrain::sSerialNumber_ = 0;

void Terrain::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);

	// コライダーの初期化
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, 0.0f, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributeTerrain);
	boxCollider_.SetCollisionMask(kCollisionAttributePlayer);

	// シリアル番号振り
	serialNum_ = sSerialNumber_;
	sSerialNumber_++;

}

void Terrain::Update()
{

	// 基底クラスの更新
	IObject::Update();
	// コライダー
	BoxColliderUpdate();
}

void Terrain::Draw(BaseCamera camera)
{
	model_->Draw(worldtransform_, camera);
}

void Terrain::ImGuiDraw()
{
	std::string name = "Terrain" + std::to_string(serialNum_);
	ImGui::SeparatorText(name.c_str());
	name = name + "Position";
	ImGui::DragFloat3(name.c_str(), &worldtransform_.transform_.translate.x, 0.01f, -30.0f, 30.0f);
	name = "Terrain" + std::to_string(serialNum_) + "Coll";
	ImGui::DragFloat2(name.c_str(), &boxCollider_.position_.x);

	//ImGui::End();

}

void Terrain::OnCollision(ColliderParentObject2D target)
{
	target;
}
