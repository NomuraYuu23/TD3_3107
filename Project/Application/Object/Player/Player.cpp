#include "Player.h"

#include "../../Collider2D/CollisionConfig2D.h"
#include "../../../Engine/Collision2D/Collision2D.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/Math.h"
#include "../ObjectList.h"
#include "../GameUtility/MathUtility.h"

void Player::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);

	worldtransform_.transform_.translate = { 4.0f,10.0f,0 };

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
	// 補正クラス
	correctSystem_.Initialize(this);
	// コンボクラス
	jumpCombo_.Reset();

	// ステートの作成
	ChangeState(std::make_unique<GroundState>());

	// 武器の親設定
	weapon_->SettingParent();
	isGround_ = false;

	// 放物線
	parabola_.Initialize();

	// レイ
	rayLength_ = -100.0f;
	cameraRay_.Initialize(this);

	// プレイヤーと槍をつなぐ線
	connectingSpearLine_.reset(DrawLine::Create());
	connectingSpearLineColor_ = { 0.8f,0.0f,0.8f,1.0f };
	
}

void Player::Update()
{
	// 前フレームの座標
	prevPosition_ = worldtransform_.GetWorldPosition();

	// ステートの更新
	if (actionState_ && !recoil_.IsActive()) {
		actionState_->Update();
	}

	// 操作クラス
	controller_.Update();
	// 反動クラス
	recoil_.Update();
	// 
	//correctSystem_.Update(enemyManager_);

	// 武器の更新
	if (weapon_) {
		weapon_->Update();
	}

	// 放物線
	if (throwDirect_.x != 0 || throwDirect_.y != 0) {
		parabola_.Update(
			worldtransform_.GetWorldPosition(),
			throwDirect_,this);
	}
	//else {
	//	parabola_.Reset();
	//}
	// 基底クラスの更新
	IObject::Update();
	// コライダー
	CircleColliderUpdate();
	// 足元のコライダー
	footCollider_.Update();
	// レイ
	cameraRay_.Update();
}

void Player::Draw(const BaseCamera& camera)
{
	// 矢印用の座標
	Vector3 offset = throwDirect_ * 2.0f;
	screenPos_ = MathUtility::WorldToScreen(worldtransform_.GetWorldPosition() + offset, &const_cast<BaseCamera&>(camera));

	// プレイヤーの描画
	model_->Draw(worldtransform_, const_cast<BaseCamera&>(camera),material_.get());
	// 武器の描画
	if (weapon_) {
		weapon_->Draw(camera);
	}
	// 足場のモデル描画
	if (isDebugDraw_) {
		footCollider_.DebugDraw(camera);
	}
}

void Player::ImGuiDraw()
{
	ImGui::Begin("Player");
	// ゲームスピード
	float ratio = IObject::sPlaySpeed;
	ImGui::DragFloat("playTime", &ratio);
	sPlaySpeed = ratio;
	// 反動フラグ
	ImGui::Text("%d : IsRecoil", recoil_.IsActive());
	// 状態の名前取得
	std::string name = typeid(*actionState_).name();
	ImGui::Text(name.c_str());
	// 座標リセット
	if (ImGui::Button("PosReset")) {
		worldtransform_.transform_.translate = { 4.0f,3.0f,0 };
		velocity_ = {};
		worldtransform_.UpdateMatrix();
		isGround_ = true;
	}

	perVec = { -up.y,up.x };
	float dot = Vector2::Dot(perVec, tag);

	ImGui::DragFloat2("dir", &up.x);
	ImGui::DragFloat2("targ", &tag.x);
	ImGui::DragFloat2("per", &perVec.x);
	ImGui::DragFloat("Dot", &dot);

	// 足場の描画表示
	ImGui::Checkbox("DrawFootCollider", &isDebugDraw_);

	ImGui::DragFloat("FloorPos:Y", &floorPrevY_);
	ImGui::DragFloat("RayLength", &rayLength_, -500.0f, 500.0f);

	ImGui::DragFloat2("Screen", &screenPos_.x);

	// ジャンプのコンボ数
	int count = jumpCombo_.GetCount();
	ImGui::DragInt("ComboCount", &count);

	ImGui::Text("\n");
	ImGui::SeparatorText("State");

	if (ImGui::BeginTabBar("Param")) {
		float absValue = 300.0f;

		// 共通項目
		if (ImGui::BeginTabItem("Common")) {
			// 座標
			ImGui::DragFloat3("translate", &worldtransform_.transform_.translate.x, 0.01f, -absValue, absValue);
			// 速度
			ImGui::DragFloat3("velocity", &velocity_.x);
			// スケール
			ImGui::DragFloat3("Scale", &worldtransform_.transform_.scale.x);
		
			ImGui::EndTabItem();
		}
		// コライダー用
		if (ImGui::BeginTabItem("Collider")) {
			footCollider_.ImGuiDraw();

			ImGui::DragFloat2("ColliderPos", &circleCollider_.position_.x, 0.01f, -absValue, absValue);
			ImGui::DragFloat2("ColliderSize", &circleCollider_.scale_.x, 0.01f, 0, 10.0f);
			ImGui::DragFloat("Radius", &circleCollider_.radius_, 0.01f, 0, 10.0f);
			ImGui::EndTabItem();
		}

		// 地上
		if (ImGui::BeginTabItem("OnGround")) {
			// 着地フラグ
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
		// 空中
		if (ImGui::BeginTabItem("Aerial")) {
			// レイ
			cameraRay_.ImGuiDraw();
			ImGui::EndTabItem();
		}

		// タブバーを終了
		ImGui::EndTabBar();
	}

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
		// 壁に刺さっている状態なら
		if (std::holds_alternative<ImpaledState*>(weapon_->GetNowState()) && !weapon_->IsTread()) {			
			// 引き寄せ中の衝突をリターン
			if (std::holds_alternative<AttractState*>(nowState_)) {
				// 引き寄せの
				if (worldtransform_.GetWorldPosition().x > weapon_->worldtransform_.GetWorldPosition().x) {
					weapon_->velocity_.x = -1.0f;
				}
				else {
					weapon_->velocity_.x = 1.0f;
				}
				weapon_->ChangeRequest(Weapon::StateName::kFreeFall);
				return;
			}
			else {
				// 移動ベクトルが下向きの時にのみ
				if (velocity_.y < 0 && (!recoil_.IsActive()) && !isOneStepOn_) {

					// 踏む際の武器設定
					weapon_->TreadSetting();
					// 槍じゃんステートへ
					ChangeState(std::make_unique<SpearAerialState>());
					return;
				}
			}

			return;
		}
		// 帰ってきてる時の衝突
		else if (std::holds_alternative<ReturnState*>(weapon_->GetNowState())) {
			//// 着地している場合早期リターン
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

		// 前の座標から現座標へのベクトル
		Vector3 moveDirect = worldtransform_.GetWorldPosition() - prevPosition_;
		moveDirect = Vector3::Normalize(moveDirect);

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

		// 衝突したブロックへのベクトル
		Vector2 p2tDist = { targetPos.x - worldtransform_.GetWorldPosition().x,targetPos.y - worldtransform_.GetWorldPosition().y };

		Vector3 lerpPos = Ease::Easing(Ease::EaseName::Lerp, worldtransform_.GetWorldPosition(), prevPosition_, 0.15f);

		// 最小・最大値
		//Vector2 plMin = { worldtransform_.GetWorldPosition().x - scale2D_.x,worldtransform_.GetWorldPosition().y - scale2D_.y };
		//Vector2 plMax = { worldtransform_.GetWorldPosition().x + scale2D_.x,worldtransform_.GetWorldPosition().y + scale2D_.y };
		Vector2 plMin = { lerpPos.x - scale2D_.x,lerpPos.y - scale2D_.y };
		Vector2 plMax = { lerpPos.x + scale2D_.x,lerpPos.y + scale2D_.y };

		// 四頂点
		IObject::FourTop player4Point = IObject::GenerateFourTop(plMin, plMax);
		//IObject::FourTop block4P = IObject::GenerateFourTop({ minPos.x,minPos.y }, {maxPos.x,maxPos.y});
		//Vector2 perMove = { -velocity_.y,velocity_.x };
		//// 移動ベクトルの垂線
		//perMove = Vector2::Normalize(perMove);
		//float dircDot = Vector2::Dot(perMove, p2tDist);

		IObject::CollisionType type = IObject::GetCollisionType(player4Point, { minPos.x,minPos.y }, { maxPos.x,maxPos.y });
		Vector2 correctPosition = {};
		float correctValue = 0.1f;
		switch (type)
		{
		case IObject::kLeftSide:
			// プレイヤーの修正されたX座標を計算
			correctPosition.x = targetPos.x + targetRad.x + (scale2D_.x / 2.0f) + correctValue;
			worldtransform_.transform_.translate.x = correctPosition.x;
			velocity_.x = 0;
			break;
		case IObject::kRightSide:
			// プレイヤーの修正されたX座標を計算
			correctPosition.x = targetPos.x - targetRad.x - (scale2D_.x / 2.0f) - correctValue;
			worldtransform_.transform_.translate.x = correctPosition.x;
			velocity_.x = 0;
			break;
		case IObject::kTopSide:
			// プレイヤーの修正されたY座標を計算
			correctPosition.y = targetPos.y - targetRad.y - (scale2D_.y / 2.0f) - correctValue;
			worldtransform_.transform_.translate.y = correctPosition.y;
			velocity_.y = 0;

			break;
		case IObject::kBottomSide:
			// プレイヤーの修正されたY座標を計算
			correctPosition.y = targetPos.y + targetRad.y + (scale2D_.y / 2.0f) + correctValue;
			worldtransform_.transform_.translate.y = correctPosition.y;
			// プレイヤーが下向きに移動しており、空中にいる場合、着地状態に変更
			if (std::holds_alternative<AerialState*>(GetNowState()) || std::holds_alternative<SpearAerialState*>(GetNowState())) {
				ChangeState(std::make_unique<GroundState>());
			}
			isGround_ = true;
			//velocity_.y = 0;
			break;
		case IObject::kLBPoint:
			if (moveDirect.y < 0) {
				// プレイヤーの修正されたY座標を計算
				correctPosition.y = targetPos.y + targetRad.y + (scale2D_.y / 2.0f) + correctValue;
				worldtransform_.transform_.translate.y = correctPosition.y;
				// プレイヤーが下向きに移動しており、空中にいる場合、着地状態に変更
				if (std::holds_alternative<AerialState*>(GetNowState()) || std::holds_alternative<SpearAerialState*>(GetNowState())) {
					ChangeState(std::make_unique<GroundState>());
				}
			}
			break;
		case IObject::kLTPoint:

			break;
		case IObject::kRBPoint:
			if (moveDirect.y < 0) {
				// プレイヤーの修正されたY座標を計算
				correctPosition.y = targetPos.y + targetRad.y + (scale2D_.y / 2.0f) + correctValue;
				worldtransform_.transform_.translate.y = correctPosition.y;
				// プレイヤーが下向きに移動しており、空中にいる場合、着地状態に変更
				if (std::holds_alternative<AerialState*>(GetNowState()) || std::holds_alternative<SpearAerialState*>(GetNowState())) {
					ChangeState(std::make_unique<GroundState>());
				}
			}
			break;
		case IObject::kRTPoint:

			break;
		case IObject::kMultiPoints:
			correctPosition = Ease::Easing(Ease::EaseName::Lerp, Vector2{ worldtransform_.GetWorldPosition().x,worldtransform_.GetWorldPosition().y },
				Vector2{ prevPosition_.x,prevPosition_.y }, 0.15f);
			worldtransform_.transform_.translate = { correctPosition.x,correctPosition.y,0 };

			break;
		case IObject::kNone:

			break;
		}

		worldtransform_.UpdateMatrix();

		// 座標以外の処理
		if (std::holds_alternative<AttractState*>(nowState_)) {
			Vector3 newDirect = weapon_->worldtransform_.GetWorldPosition() - worldtransform_.GetWorldPosition();
			ChangeState(std::make_unique<AerialState>());
			velocity_ = Vector3::Normalize(newDirect) * -15.0f;
			return;
		}

		// 反動のキャンセル
		if (recoil_.IsActive() && std::holds_alternative<GroundState*>(GetNowState())) {
			recoil_.CancelRecoil();
		}
		// 反動中かつ壁ジャンの受付をしていない場合
		else if (recoil_.IsActive() && !recoil_.IsAccept()) {
			// 方向
			//weapon_->throwDirect_ = throwDirect_;
			// X軸
			//if (std::fabs(p2tDist.x) > std::fabs(p2tDist.y)) {
			//	if (velocity_.x > 0) {
			//		weapon_->throwDirect_ = { 1.0f,0,0 };
			//	}
			//	else {
			//		weapon_->throwDirect_ = { -1.0f,0,0 };
			//	}
			//}
			//// Y軸
			//else if (std::fabs(p2tDist.x) < std::fabs(p2tDist.y)) {
			//	if (velocity_.y > 0) {
			//		weapon_->throwDirect_ = { 0,-1.0f,0 };
			//	}
			//	else {
			//		weapon_->throwDirect_ = { 0,1.0f,0 };
			//	}
			//}

			weapon_->throwDirect_ = Vector3::Normalize(moveDirect);
			
			weapon_->worldtransform_.transform_.translate = worldtransform_.GetWorldPosition();
			// 受付フラグ
			recoil_.Accept();
			recoil_.CancelRecoil();

			// 武器のステートを変更
			// 先にステート変更しないと速度の初期化が行われるため
			weapon_->ChangeRequest(Weapon::StateName::kThrown);
			// プレイヤーのステートを変更
			ChangeState(std::make_unique<SpearAerialState>());

			// 壁じゃんの時の値
			Vector2 power = { 10.0f,40.0f };
			if (type == IObject::kRightSide || type == IObject::kRBPoint || type == IObject::kRTPoint) {
				velocity_.x = power.x * -1.0f;
			}
			else {
				velocity_.x = power.x;
			}

			velocity_.y = power.y;

		}

	}
	// 雑魚敵との当たり判定
	else if (std::holds_alternative<Enemy*>(target)) {
		// 無敵中なら早期
		if (invisibleTimer_.IsActive()) {
			return;
		}

		// 持ってないかどうか
		if (std::holds_alternative<HoldState*>(weapon_->GetNowState())) {
			// 持ってるから何か起きる

		}
		else {
			// 持ってないから死ぬ
			isDead_ = true;
		}

	}
	// ボス
	else if (std::holds_alternative<PrevSmallBoss*>(target)) {
		// 無敵中なら早期
		if (invisibleTimer_.IsActive()) {
			return;
		}

		// 持ってないかどうか
		if (std::holds_alternative<HoldState*>(weapon_->GetNowState())) {
			// 持ってるから何か起きる

		}
		else {
			// 持ってないから死ぬ

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

void Player::DrawLines(BaseCamera& baseCamera)
{

	parabola_.Draw(baseCamera);

	connectingSpearLine_->Draw(
		worldtransform_.GetWorldPosition(),
		weapon_->worldtransform_.GetWorldPosition(),
		connectingSpearLineColor_,
		connectingSpearLineColor_,
		baseCamera);

}
