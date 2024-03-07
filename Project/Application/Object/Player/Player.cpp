#include "Player.h"

#include "../../Collider2D/CollisionConfig2D.h"
#include "../../../Engine/2D/ImguiManager.h"

void Player::Initialize(Model* model)
{
	// ポインタの設定
	input_ = Input::GetInstance();
	// 基底クラスの初期化
	IObject::Initialize(model);
	// コライダーの初期化
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributePlayer);
	boxCollider_.SetCollisionMask(kCollisionAttributeEnemy);

	// ステートの作成
	ChangeState(std::make_unique<GroundState>());
}

void Player::Update()
{
	// ステートの更新
	if (actionState_) {
		actionState_->Update();
	}

	// 基底クラスの更新
	IObject::Update();

	// 武器の更新
	if (weapon_) {
		if (input_->TriggerKey(DIK_E)) {
			weapon_->ChangeRequest(Weapon::StateName::kReturn);
			//weapon_->ChangeState(std::make_unique<ReturnState>());
		}
		if (input_->TriggerKey(DIK_Q)) {
			weapon_->throwDirect_ = throwDirect_;
			weapon_->ChangeRequest(Weapon::StateName::kThrown);
		}
		weapon_->Update();
	}

}

void Player::Draw(BaseCamera camera)
{
	// プレイヤーの描画
	model_->Draw(worldtransform_, camera,material_.get());
	// 武器の描画
	if (weapon_) {
		weapon_->Draw(camera);
	}
}

void Player::ImGuiDraw()
{
	ImGui::Begin("Player");

	if (ImGui::BeginTabBar("Param")) {
		// 共通項目
		if (ImGui::BeginTabItem("Common")) {
			float absValue = 30.0f;
			// 座標
			ImGui::DragFloat3("translate", &worldtransform_.transform_.translate.x, 0.01f, -absValue, absValue);
			ImGui::DragFloat3("velocity", &velocity_.x);
			// 重力
			ImGui::DragFloat("Gravity", &gravity_, 0.01f, -absValue, absValue);
		
			ImGui::DragFloat3("ThrowDirect", &throwDirect_.x, 0.01f, -10, 10);
			
			throwDirect_ = throwDirect_.Normalize(throwDirect_);

			ImGui::EndTabItem();
		}
		// 地上
		if (ImGui::BeginTabItem("OnGround")) {

			ImGui::EndTabItem();
		}
		// 空中
		if (ImGui::BeginTabItem("Aerial")) {

			ImGui::EndTabItem();
		}

		// タブバーを終了
		ImGui::EndTabBar();
	}

	ImGui::SeparatorText("");
	// ステート確認
	if (typeid(*actionState_) == typeid(AerialState)) {

	}
	else {

	}
	// どっちかを判断
	std::string name = typeid(*actionState_).name();

	ImGui::Text(name.c_str());

	ImGui::End();

	// 武器のImGUi
	if (weapon_) {
		weapon_->ImGuiDraw();
	}

}

void Player::OnCollision(ColliderParentObject2D* target, uint32_t tag)
{

	target, tag;

}

void Player::ChangeState(std::unique_ptr<IActionState> newState)
{
	// ポインタの設定
	newState->PreInitialize(this);
	// ステートごとの初期化
	newState->Initialize();
	// ステート渡し
	actionState_ = std::move(newState);
}
