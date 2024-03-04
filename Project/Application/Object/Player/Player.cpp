#include "Player.h"

#include "../../Collider2D/CollisionConfig2D.h"
#include "../../../Engine/2D/ImguiManager.h"

void Player::Initialize(Model* model)
{
	// ポインタの設定
	input_ = Input::GetInstance();
	// 基底クラスの初期化
	IObject::Initialize(model);
	// ステートの作成
	ChangeState(std::make_unique<GroundState>());

}

void Player::Update()
{
	// ステートの更新
	if (actionState_) {
		actionState_->Update();
	}

	// 投げる処理



	// 基底クラスの更新
	IObject::Update();
}

void Player::Draw(BaseCamera camera)
{
	model_->Draw(worldtransform_, camera,material_.get());
}

void Player::ImGuiDraw()
{
	ImGui::Begin("Player");

	float absValue = 30.0f;
	// 座標
	ImGui::DragFloat3("translate", &worldtransform_.transform_.translate.x, 0.01f, -absValue, absValue);


	// ステート確認
	if (typeid(*actionState_) == typeid(AerialState)) {
		stateCheck_ = true;
	}
	else {
		stateCheck_ = false;
	}

	int isFlag = stateCheck_;
	// どっちかを判断
	ImGui::InputInt("checkBut", &isFlag);
	std::string name = typeid(*actionState_).name();

	ImGui::Text(name.c_str());

	ImGui::End();

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
