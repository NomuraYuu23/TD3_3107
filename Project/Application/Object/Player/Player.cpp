#include "Player.h"

#include "../../Collider2D/CollisionConfig2D.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/Math.h"
#include "../ObjectList.h"
#include "../../../Engine/Collision2D/Collision2D.h"
#include "../GameUtility/MathUtility.h"

void Player::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);

	worldtransform_.transform_.translate = { 4.0f,3.0f,0 };

	// コライダーの初期化
	circleCollider_.radius_ = 0.95f;
	circleCollider_.Initialize(position2D_, circleCollider_.radius_, this);
	circleCollider_.SetCollisionAttribute(kCollisionAttributePlayer);
	circleCollider_.SetCollisionMask(kCollisionAttributeEnemy);

	// 入力処理受付クラス
	controller_.Initialize(this);
	// 反動クラス
	recoil_.Initialize(this);
	// 足場クラス
	footCollider_.Initialize(model, this);
	// コンボクラス
	jumpCombo.Reset();

	// ステートの作成
	ChangeState(std::make_unique<GroundState>());

	weapon_->SettingParent();
	isGround_ = false;
	//gravity_ = 35.0f;

}

void Player::Update()
{
	
	// 前フレームの座標
	prevPosition_ = worldtransform_.GetWorldPosition();
	//velocity_ = {};
	// ステートの更新
	if (actionState_ && !recoil_.IsActive()) {
		actionState_->Update();
	}

	// 操作クラス
	controller_.Update();
	// 反動クラス
	recoil_.Update();

	// 武器の更新
	if (weapon_) {
		weapon_->Update();
	}

	// 基底クラスの更新
	IObject::Update();
	// コライダー
	CircleColliderUpdate();
	// 足元のコライダー
	footCollider_.Update();
}

void Player::Draw(const BaseCamera& camera)
{
	Vector3 offset = throwDirect_ * 2.0f;
	screenPos_ = MathUtility::WorldToScreen(worldtransform_.GetWorldPosition() + offset, &const_cast<BaseCamera&>(camera));
	//screenPos_ = MathUtility::WorldToScreen(worldtransform_.GetWorldPosition(), &camera);
	// プレイヤーの描画
	model_->Draw(worldtransform_, const_cast<BaseCamera&>(camera),material_.get());
	// 武器の描画
	if (weapon_) {
		weapon_->Draw(camera);
	}

	if (isDebugDraw_) {
		footCollider_.DebugDraw(camera);
	}
}

void Player::ImGuiDraw()
{
	ImGui::Begin("Player");

	float ratio = IObject::sPlaySpeed;
	ImGui::DragFloat("playTime", &ratio);
	sPlaySpeed = ratio;
	ImGui::Text("%d : IsRecoil", recoil_.IsActive());
	//ImGui::DragFloat("")

	if (ImGui::Button("Normal")) {
		sPlaySpeed = 1.0f;
	}

	if (ImGui::Button("Slow")) {
		sPlaySpeed = 2.5f;
	}	

	if (ImGui::Button("PosReset")) {
		worldtransform_.transform_.translate = { 4.0f,3.0f,0 };
		velocity_ = {};
		worldtransform_.UpdateMatrix();
		isGround_ = true;
	}

	ImGui::Checkbox("DrawFootCollider", &isDebugDraw_);

	Vector3 direct = worldtransform_.GetWorldPosition() - prevPosition_;
	direct = Vector3::Normalize(direct);
	ImGui::Text("%f : X", std::fabs(direct.x));
	ImGui::Text("%f : Y", std::fabs(direct.y));

	ImGui::DragFloat("thres", &threshold_y_, 0.01f, 0, 1.0f);
	int count = this->jumpCombo.GetCount();
	ImGui::DragInt("ComboCount", &count);

	if (ImGui::BeginTabBar("Param")) {

		// 共通項目
		if (ImGui::BeginTabItem("Common")) {
			float absValue = 30.0f;
			// 座標
			ImGui::DragFloat3("translate", &worldtransform_.transform_.translate.x, 0.01f, -absValue, absValue);
			ImGui::DragFloat3("velocity", &velocity_.x);
			ImGui::DragFloat3("Scale", &worldtransform_.transform_.scale.x);
		
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
			ImGui::Text("IsGround : %d", isGround_);

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

	footCollider_.ImGuiDraw();

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
			
			// 移動ベクトルが下向きの時にのみ
			if (velocity_.y < 0 && (!recoil_.IsActive())) {
				// 踏む際の武器設定
				weapon_->TreadSetting();
				jumpCombo.Add();
				//ChangeState(std::make_unique<ActionWaitState>());
				ChangeState(std::make_unique<SpearAerialState>());
			}
			return;
		}
		// 帰ってきてる時の衝突
		else if (std::holds_alternative<ReturnState*>(weapon_->nowState_)) {
			//// 着地している場合早期リターン
			if (std::holds_alternative<GroundState*>(nowState_)) {
				return;
			}
			// 反動生成
			recoil_.CreateRecoil(Vector3::Normalize(worldtransform_.GetWorldPosition() - weapon_->worldtransform_.GetWorldPosition()));
			throwDirect_ = Vector3::Normalize(worldtransform_.GetWorldPosition() - weapon_->worldtransform_.GetWorldPosition());
			return;
		}

	}
	// 地形との当たり判定
	else if (std::holds_alternative<Terrain*>(target)) {

		// 前の座標から現座標へのベクトル
		Vector3 moveDirect = worldtransform_.GetWorldPosition() - prevPosition_;
		moveDirect = Vector3::Normalize(moveDirect);

		//// 移動していない場合
		//if (moveDirect.x == 0 && moveDirect.y == 0 || velocity_.x == 0 && velocity_.y == 0) {
		//	return;
		//}

		Vector2 targetPos = {};
		Vector2 targetRad = {};
		// 対象の情報取得
		std::visit([&](const auto& a) {
			targetPos = a->GetColliderPosition();
			targetRad = a->GetColliderSize();
			}, target);
		targetRad *= 0.5f;
		// 右上
		Vector3 maxPos = {
			targetPos.x + targetRad.x,	// 右
			targetPos.y + targetRad.y,	// 上
		};
		// 左下
		Vector3 minPos = {
			targetPos.x - targetRad.x,	// 左
			targetPos.y - targetRad.y,	// 下
		};

		
		Vector2 p2tDist = { targetPos.x - worldtransform_.GetWorldPosition().x,targetPos.y - worldtransform_.GetWorldPosition().y };

		//Vector3 directVector = Vector3::Normalize(velocity_);

		/*if (std::fabs(moveDirect.x) > std::fabs(moveDirect.y)) {*/
		//if (std::fabs(moveDirect.x) != 0 && velocity_.x != 0) {
		if(std::fabs(p2tDist.x) > std::fabs(p2tDist.y)){
			// サイズ分
			float offset = 0.1f;
			targetRad.x += offset + circleCollider_.radius_;
			targetRad.y += offset + circleCollider_.radius_;

			if (worldtransform_.GetWorldPosition().x < targetPos.x) {
				// 修正x座標
				float correctX = minPos.x - (circleCollider_.radius_ + offset);
				worldtransform_.transform_.translate.x = correctX;
			}
			else if (worldtransform_.GetWorldPosition().x > targetPos.x) {
				// 修正x座標
				float correctX = maxPos.x + (circleCollider_.radius_ + offset);
				worldtransform_.transform_.translate.x = correctX;
			}

			//// 初期化
			//if (recoil_.IsActive() && !recoil_.IsAccept()) {
			//	// 方向
			//	weapon_->throwDirect_ = Vector3::Normalize(velocity_);

			//	// 受付フラグ
			//	recoil_.Accept();
			//	// ここ定数に変更
			//	velocity_.x *= -1.0f;

			//	weapon_->ChangeRequest(Weapon::StateName::kThrown);
			//	ChangeState(std::make_unique<SpearAerialState>());

			//}
			if(!recoil_.IsActive() && !recoil_.IsAccept()){
				velocity_.x = 0;
			}

			worldtransform_.UpdateMatrix();

		}
		//else if (std::fabs(moveDirect.x) < (std::fabs(moveDirect.y) - threshold_y_)) {
		else if(std::fabs(p2tDist.x) < std::fabs(p2tDist.y)){
			// 移動文
			float offset = 0.1f;
			targetRad.x += offset + circleCollider_.radius_;
			targetRad.y += offset + circleCollider_.radius_;

			// 上向き
			// 上向きの場合のみ早期
			if (moveDirect.y > 0) {
				// 修正y座標
				float correctY = targetPos.y - targetRad.y;
				worldtransform_.transform_.translate.y = correctY;
			}
			// 下向き
			else if (moveDirect.y < 0) {
				// 修正y座標
				float correctY = targetPos.y + targetRad.y;
				worldtransform_.transform_.translate.y = correctY;
			}

			worldtransform_.UpdateMatrix();

			// ジャンプ中・槍ジャンプ中なら
			if (std::holds_alternative<AerialState*>(GetNowState()) || std::holds_alternative<SpearAerialState*>(GetNowState())) {
				ChangeState(std::make_unique<GroundState>());
			}
		}

		// 反動のキャンセル
		if (recoil_.IsActive() && (isGround_)) {
			recoil_.CancelRecoil();
		}
		else if (recoil_.IsActive() && !recoil_.IsAccept()) {
			// 方向
			weapon_->throwDirect_ = throwDirect_;

			// 受付フラグ
			recoil_.Accept();
			recoil_.CancelRecoil();
			// ここ定数に変更
			velocity_.x *= -0.25f;

			weapon_->ChangeRequest(Weapon::StateName::kThrown);
			ChangeState(std::make_unique<SpearAerialState>());
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
