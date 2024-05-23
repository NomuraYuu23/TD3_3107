#include "Terrain.h"
#include "../../Collider2D/CollisionConfig2D.h"
#include "../../../Engine/2D/ImguiManager.h"

uint32_t Terrain::sSerialNumber_ = 0;

void Terrain::Initialize()
{
	// 基底クラスの初期化
	OneOfManyObjects::Initialize();

	// コライダー用の座標・スケール
	position2D_ = { transform_.translate.x,transform_.translate.y };
	scale2D_ = { transform_.scale.x * 2.0f, transform_.scale.y * 2.0f };

	// コライダーの初期化
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, 0.0f, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributeTerrain);
	boxCollider_.SetCollisionMask(kCollisionAttributePlayer);

	// シリアル番号振り
	serialNum_ = sSerialNumber_;
	sSerialNumber_++;

	// マテリアル
	enableLighting_ = 0;
	shininess_ = 100.0f;

	material_.reset(Material::Create());
	material_->SetEnableLighting(enableLighting_);
	material_->SetShininess(shininess_);
	//this->transform_

	name_ = "noName";

	//	ライティング設定
	materialData_.enableLighting = EnableLighting::HalfLambert;
}

void Terrain::Update()
{

	// デバッグの場合のみ行う
#ifdef _DEBUG

	// 地形マテリアルのuvトランスフォームを更新
	MaterialUpdate();

#endif // _DEBUG

	// 基底クラスの更新
	OneOfManyObjects::Update();

	// 2D更新
	position2D_ = { worldMatrix_.m[3][0],worldMatrix_.m[3][1]};
	// コライダー
	BoxColliderUpdate();
}

void Terrain::ImGuiDraw()
{
	std::string name = "Terrain" + std::to_string(serialNum_);
	ImGui::SeparatorText(name.c_str());
	name = name + "Position";
	ImGui::DragFloat3(name.c_str(), &transform_.translate.x, 0.01f, -30.0f, 30.0f);
	name = "Terrain" + std::to_string(serialNum_) + "Coll";
	ImGui::DragFloat2(name.c_str(), &boxCollider_.position_.x);

	//ImGui::End();

}

void Terrain::OnCollision(ColliderParentObject2D target)
{
	target;
}

void Terrain::MaterialUpdate()
{
	// UVトランスフォームをスケールに合わせて調整
	materialData_.uvTransform = Matrix4x4::MakeAffineMatrix({ transform_.scale.x / 2.0f, 1.0f, 1.0f }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f });
}
