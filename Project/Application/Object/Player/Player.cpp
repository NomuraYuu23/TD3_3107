#include "Player.h"

#include "../../Collider2D/CollisionConfig2D.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Math/Ease.h"
#include "../ObjectList.h"

void Player::Initialize(Model* model)
{
	// 入力処理受付クラス
	controller_.Initialize(this);
	// 反動クラス
	recoil_.Initialize(this);

	// 基底クラスの初期化
	IObject::Initialize(model);

	worldtransform_.transform_.translate = { 0,1,0 };

	// コライダーの初期化
	circleCollider_.radius_ = 0.95f;
	circleCollider_.Initialize(position2D_, circleCollider_.radius_, this);
	circleCollider_.SetCollisionAttribute(kCollisionAttributePlayer);
	circleCollider_.SetCollisionMask(kCollisionAttributeEnemy);

	footCollider_.position_ = { position2D_.x,position2D_.y - scale2D_.x - 0.5f };
	footCollider_.scale_ = { scale2D_.x * 0.75f,scale2D_.x * 0.2f };
	footCollider_.collider_.Initialize(footCollider_.position_, footCollider_.scale_.x, footCollider_.scale_.y, 0.0f,this);

	// ステートの作成
	ChangeState(std::make_unique<GroundState>());

	weapon_->SettingParent();
	isGround_ = false;
}

void Player::Update()
{
	// 前フレームの座標
	prevPosition_ = worldtransform_.GetWorldPosition();
	//velocity_ = {};
	// ステートの更新
	if (actionState_) {
		actionState_->Update();
	}

	// 操作クラス
	controller_.Update();
	// 反動クラス
	recoil_.Update();

	// 基底クラスの更新
	IObject::Update();
	// コライダー
	CircleColliderUpdate();
	footCollider_.collider_.Update(footCollider_.position_, footCollider_.scale_.x, footCollider_.scale_.y, 0.0f);

	// 武器の更新
	if (weapon_) {
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

	float ratio = IObject::sPlaySpeed;
	ImGui::DragFloat("playTime", &ratio);
	sPlaySpeed = ratio;
	if (ImGui::Button("Normal")) {
		sPlaySpeed = 1.0f;
	}

	if (ImGui::Button("Slow")) {
		sPlaySpeed = 2.5f;
	}	

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

			if (ImGui::Button("IsGround")) {
				if (isGround_) {
					isGround_ = false;
				}
				else {
					isGround_ = true;
				}
			}


			ImGui::EndTabItem();
		}
		// コライダー用
		if (ImGui::BeginTabItem("Collider")) {
			ImGui::DragFloat2("ColliderPos", &circleCollider_.position_.x, 0.01f, 0, 10.0f);
			ImGui::DragFloat2("ColliderSize", &circleCollider_.scale_.x, 0.01f, 0, 10.0f);
			ImGui::DragFloat("Radius", &circleCollider_.radius_, 0.01f, 0, 10.0f);
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

void Player::OnCollision(ColliderParentObject2D target)
{
	// 武器との衝突
	if (std::holds_alternative<Weapon*>(target)) {

		if (std::holds_alternative<HoldState*>(weapon_->nowState_)) {
			return;
		}
		// 壁に刺さっている状態なら
		else if (std::holds_alternative<ImpaledState*>(weapon_->nowState_) && !weapon_->GetIsTread()) {
			// 地上か待機状態なら早期
			if (std::holds_alternative<GroundState*>(nowState_) || std::holds_alternative<ActionWaitState*>(nowState_)) {
				return;
			}

			//if (weapon_->worldtransform_.GetWorldPosition().y + weapon_->scale2D_.y < worldtransform_.GetWorldPosition().y) {
			//	// 
			//	weapon_->TreadSetting();
			//	ChangeState(std::make_unique<ActionWaitState>());
			//}
			
			// 移動ベクトルが下向きの時にのみ
			if (velocity_.y < 0) {
				// 
				weapon_->TreadSetting();
				//ChangeState(std::make_unique<ActionWaitState>());
				ChangeState(std::make_unique<AerialState>());
			}
			return;
		}
		// 帰ってきてる時の衝突
		else if (std::holds_alternative<ReturnState*>(weapon_->nowState_)) {
			// 着地している場合早期リターン
			if (std::holds_alternative<GroundState*>(nowState_)) {
				return;
			}
			// 反動生成
			recoil_.CreateRecoil(Vector3::Normalize(worldtransform_.GetWorldPosition() - weapon_->worldtransform_.GetWorldPosition()));

			return;
		}

	}
	// 地形との当たり判定
	else if (std::holds_alternative<Terrain*>(target)) {

		// 速度がなければ
		if (velocity_.x == 0 && velocity_.y == 0) {
			return;
		}
		// 横移動
		if (std::fabsf(velocity_.x) != 0) {

			Vector2 targetPos = {};
			Vector2 targetRad = {};
			// 対象の情報取得
			std::visit([&](const auto& a) {
				targetPos = a->GetColliderPosition();
				targetRad = a->GetColliderSize();
				}, target);

			// 移動文
			float offset = 0.1f;
			targetRad.x += offset;
			targetRad.y += offset;

			if (velocity_.x > 0) {
				Vector3 correctPosition = { targetPos.x - targetRad.x,worldtransform_.GetWorldPosition().y,0 };
				worldtransform_.transform_.translate = correctPosition;
			}
			else if(velocity_.x < 0){
				Vector3 correctPosition = { targetPos.x + targetRad.x,worldtransform_.GetWorldPosition().y,0 };
				worldtransform_.transform_.translate = correctPosition;
			}
			// 初期化
			velocity_.x = 0;

		}		

		if (std::fabsf(velocity_.y) != 0) {
			Vector2 targetPos = {};
			Vector2 targetRad = {};
			std::visit([&](const auto& a) {
				targetPos = a->GetColliderPosition();
				targetRad = a->GetColliderSize();
				}, target);

			float offset = 0.05f;
			targetRad.y += offset;

			// 上向き
			// 上向きの場合のみ早期
			if (velocity_.y > 0) {
				Vector3 correctPosition = { worldtransform_.GetWorldPosition().x,targetPos.y - targetRad.y,0 };
				worldtransform_.transform_.translate = correctPosition;
				return;
			}
			// 下向き
			else if(velocity_.y < 0){
				Vector3 correctPosition = { worldtransform_.GetWorldPosition().x,targetPos.y + targetRad.y,0 };
				worldtransform_.transform_.translate = correctPosition;
			}


			isGround_ = true;

			//velocity_.y = 0;

			if (std::holds_alternative<AerialState*>(GetNowState())) {
				ChangeState(std::make_unique<GroundState>());
				return;
			}

		}

	}

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
