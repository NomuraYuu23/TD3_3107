#include "Weapon.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Input/Input.h"
#include "../../Collider2D/CollisionConfig2D.h"

void Weapon::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);
	worldtransform_.transform_.translate.x = 2.0f;
	worldtransform_.usedDirection_ = true;
	// 親子関係でのオフセット
	localOffset_ = { 2.0f,0,0 };
	scale2D_ *= 0.95f;
	// コライダーの初期化
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributeEnemy);
	boxCollider_.SetCollisionMask(kCollisionAttributePlayer);

	// ステート変更
	ChangeState(std::make_unique<HoldState>());

}

void Weapon::Update()
{
	// 状態ごとの更新
	if (state_) {
		state_->Update();
	}

	// 二段ジャンプのクールタイム設定
	if (isTread_ && timer_.IsEnd()) {
		isTread_ = false;
	}

	// タイマー
	timer_.Update();

	// 基底クラスの更新
	IObject::Update();
	// コライダー
	BoxColliderUpdate();
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
	
	// 回転処理
	ImGui::DragFloat3("Rotation", &worldtransform_.direction_.x, 0.1f, -360.0f, 360.0f);
	worldtransform_.direction_ = Vector3::Normalize(this->throwDirect_);

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
	// 壁・ブロックとの衝突判定
	if (std::holds_alternative<Terrain*>(target)) {
		// 投げられている状態なら刺さった状態へ
		if (std::holds_alternative<ThrownState*>(nowState_)) {
			ChangeRequest(Weapon::StateName::kImpaled);
			return;
		}
	}

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

void Weapon::TreadSetting()
{
	isTread_ = true;
	timer_.Start(30.0f);
}
