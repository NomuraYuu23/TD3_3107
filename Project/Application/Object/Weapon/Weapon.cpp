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
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, 0.0f, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributeEnemy);
	boxCollider_.SetCollisionMask(kCollisionAttributePlayer);

	// ステート変更
	ChangeState(std::make_unique<HoldState>());
	// 重力
	gravityValue_ = 35.0f;
	// 戻るレート
	returnRate_ = 1.3f;
	// 投げの速度
	throwSpeedRate_ = 10.0f;
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

	ImGui::DragFloat("Speed", &throwSpeedRate_, 0.01f, 0, 200.0f);
	
	// 回転処理
	ImGui::DragFloat3("Rotation", &worldtransform_.direction_.x, 0.1f, -360.0f, 360.0f);

	// ローカル座標
	ImGui::DragFloat3("localPos", &worldtransform_.transform_.translate.x, 0.01f, -40.0f, 40.0f);
	// オフセット
	ImGui::DragFloat3("localOffset", &localOffset_.x, 0.01f, -40.0f, 40.0f);
	// どっちかを判断
	std::string name = typeid(*state_).name();

	ImGui::Text(name.c_str());

	ImGui::Text("%d : isGravity", isGravity_);

	ImGui::DragFloat3("RotateDirect", &worldtransform_.direction_.x);

	ImGui::DragFloat3("ThrowDirect", &this->throwDirect_.x);
	ImGui::DragFloat3("Velocity", &this->velocity_.x);

	ImGui::DragFloat2("CollV2", &boxCollider_.position_.x);

	ImGui::DragFloat2("returnDirect", &returnDirect_.x);
	ImGui::DragFloat2("invDirect", &invDirect_.x);

	ImGui::DragFloat("returnRate", &returnRate_, 0.01f, 0.01f, 10.0f);

	ImGui::End();

	if (state_) {
		state_->ImGuiUpdate();
	}

}

void Weapon::OnCollision(ColliderParentObject2D target)
{
	// 持っている状態なら早期
	if (std::holds_alternative<HoldState*>(nowState_)) {
		return;
	}

	// 投げられてる状態
	if (std::holds_alternative<ThrownState*>(nowState_))
	{
		// 壁・ブロックとの衝突判定
		if (std::holds_alternative<Terrain*>(target)) {
			invDirect_ = Vector2(worldtransform_.direction_.x, worldtransform_.direction_.y) * (-1.0f);
			//invDirect_ = boxCollider_.position_ - targetPosition;
			ChangeRequest(Weapon::StateName::kImpaled);
			return;
		}
	}
	// 返ってくる状態
	else if (std::holds_alternative<ReturnState*>(nowState_))
	{
		// プレイヤーとの
		if (std::holds_alternative<Player*>(target)) {
			ChangeRequest(Weapon::StateName::kHold);
			return;
		}
	}

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

void Weapon::ChangeRequest(Weapon::StateName request)
{
	// 重力のフラグリセット
	isGravity_ = false;
	// リクエストに応じてステート変更
	switch (request)
	{
	case Weapon::StateName::kHold:
		ChangeState(std::make_unique<HoldState>());
		break;
	case Weapon::StateName::kThrown:
		ChangeState(std::make_unique<ThrownState>());
		break;
	case Weapon::StateName::kImpaled:
		ChangeState(std::make_unique<ImpaledState>());
		break;
	case Weapon::StateName::kReturn:
		ChangeState(std::make_unique<ReturnState>());
		break;
	case Weapon::StateName::kWait:
		ChangeState(std::make_unique<ReturnWaitState>());
		break;
	}
}

void Weapon::TreadSetting()
{
	isTread_ = true;
	timer_.Start(30.0f);
}
