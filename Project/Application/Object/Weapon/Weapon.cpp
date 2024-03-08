#include "Weapon.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Input/Input.h"
#include "../../Collider2D/CollisionConfig2D.h"

void Weapon::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);
	worldtransform_.transform_.translate.x = 2.0f;

	// 親子関係でのオフセット
	localOffset_ = { 2.0f,0,0 };

	// コライダーの初期化
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributeEnemy);
	boxCollider_.SetCollisionMask(kCollisionAttributePlayer);
	ChangeState(std::make_unique<HoldState>());

}

void Weapon::Update()
{
	// 状態ごとの更新
	if (state_) {
		state_->Update();
	}

	// 基底クラスの更新
	IObject::Update();
}

void Weapon::Draw(BaseCamera camera)
{

	model_->Draw(worldtransform_, camera);

}

void Weapon::ImGuiDraw()
{
	ImGui::Begin("Weapon");

	// 親子変更
	if (ImGui::Button("ParentDelete")) {
		ReleaseParent();
	}
	if (ImGui::Button("ParentAdd")) {
		SettingParent();
	}
	
	// ローカル座標
	ImGui::DragFloat3("localPos", &worldtransform_.transform_.translate.x, 0.01f, -40.0f, 40.0f);
	// オフセット
	ImGui::DragFloat3("localOffset", &localOffset_.x, 0.01f, -40.0f, 40.0f);
	// どっちかを判断
	std::string name = typeid(*state_).name();

	ImGui::Text(name.c_str());

	ImGui::End();

	if (state_) {
		state_->ImGuiUpdate();
	}

}

void Weapon::OnCollision(ColliderParentObject2D target)
{
	target;
}

void Weapon::ChangeState(std::unique_ptr<IWeaponState> newState)
{
	// ポインタの設定
	newState->PreInitialize(this);
	// ステートごとの初期化
	newState->Initialize();
	// ステート渡し
	state_ = std::move(newState);
}
