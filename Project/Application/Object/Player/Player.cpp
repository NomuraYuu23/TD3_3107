#include "Player.h"

#include "../../Collider2D/CollisionConfig2D.h"
#include "../../../Engine/Collision2D/Collision2D.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/Math.h"
#include "../ObjectList.h"
#include "../GameUtility/MathUtility.h"
#include "../../UI/GameUIManager.h"
#include "../../Particle/EmitterName.h"

void Player::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);

	worldtransform_.transform_.translate = GlobalVariables::GetInstance()->GetVector3Value("Player", "RespawnPos");
	float scale = GlobalVariables::GetInstance()->GetFloatValue("Player", "ScaleValue");
	worldtransform_.transform_.scale = { scale,scale,scale };
	worldtransform_.UpdateMatrix();

	// ライティング有効
	enableLighting_ = EnableLighting::HalfLambert;
	material_->SetEnableLighting(enableLighting_);

	// コライダーの初期化
	circleCollider_.radius_ = 0.985f;
	circleCollider_.Initialize(position2D_, circleCollider_.radius_, this);
	circleCollider_.SetCollisionAttribute(kCollisionAttributePlayer);
	circleCollider_.SetCollisionMask(kCollisionAttributeEnemy);

	// システム系の初期化
	SystemInitialize();

	// ステートの作成
	ChangeState(std::make_unique<GroundState>());

	// 武器の親設定
	//weapon_->SettingParent();
	worldtransform_.UpdateMatrix();
	weapon_->SetParentAdress(&worldtransform_);
	// ステート変更
	weapon_->ChangeRequest(Weapon::StateName::kHold);

	isGround_ = false;

	velocity2DManager_ = std::make_unique<Velocity2DManager>();
	velocity2DManager_->Initialize();

	// デバッグ以外の場合行う
	#ifdef _RELEASE
	// アニメーション関連初期化
	anim_ = std::make_unique<PlayerAnimManager>(); // 生成
	anim_->Init(this);							   // 初期化
	#endif // !_DEBUG

	// デバック以外の場合行う
	#ifdef _RELEASE

	// トランスフォーム生成
	emitTransform_ = std::make_unique<EulerTransform>();
	*emitTransform_ = worldtransform_.transform_;
	emitTransform_->translate.z += 1.0f;

	// ここで環境パーティクルの再生を行う
	fallingLeafDesc_.transform = emitTransform_.get();
	fallingLeafDesc_.instanceCount = 1;
	fallingLeafDesc_.frequency = 0.25f;
	fallingLeafDesc_.lifeTime = 5.0f;
	fallingLeafDesc_.particleModelNum = kBambooLeaf;
	fallingLeafDesc_.paeticleName = kFallingLeafParticle;
	fallingLeafDesc_.velocity = { -1.0f, -1.0f, 0.0f };

	// 無限生成エミッタで生成し続ける
	ParticleManager::GetInstance()->MakeEmitter(&fallingLeafDesc_, EmitterName::kInfiniteEmitter);

	#endif // !_DEBUG
}

void Player::Update()
{
	// 前フレームの座標
	prevPosition_ = worldtransform_.GetWorldPosition();

	// ステートの更新
	if (actionState_ /*&& !recoil_.IsActive()*/) {
		actionState_->Update();
	}

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_O)) {
		worldtransform_.transform_.translate = { 300.0f,36.0f };
		worldtransform_.UpdateMatrix();
	}
	if (Input::GetInstance()->TriggerKey(DIK_I)) {
		worldtransform_.transform_.translate = { 400.0f,50.0f };
		worldtransform_.UpdateMatrix();
	}

#endif // _DEBUG


	SystemUpdate();

	// 武器の更新
	if (weapon_) {
		weapon_->Update();
	}

	// ポニーテール更新
	if (ponytail_ != nullptr) {
		// 行列を求める
		Matrix4x4 result = localMatrixManager_->GetNodeDatas()[10].matrix * worldtransform_.worldMatrix_;
		ponyAnchorPos_ = { result.m[3][0], result.m[3][1], result.m[3][2] };

		// アンカー設定
		ponytail_->SetAnchor(0, true);

		// 追従先座標を渡す
		ponytail_->SetPosition(0, ponyAnchorPos_);

		// 更新
		ponytail_->Update();
	}

	// 槍ジャン攻撃判定中ならパーティクルを発生させる
	if (IsJumpAttack()) {
		Vector3 inv = { -velocity_.x, -velocity_.y, velocity_.z };

		// 槍ジャンプパーティクル再生
		EmitterDesc desc;
		desc.transform = &worldtransform_.transform_;
		desc.velocity = inv;
		desc.instanceCount = 2;
		desc.frequency = 0.01f;
		desc.lifeTime = 0.025f;
		desc.particleModelNum = kSpark;
		desc.paeticleName = kPlayerAttackParticle;

		ParticleManager::GetInstance()->MakeEmitter(&desc, 0);
	}


	// デバック以外の場合行う
	#ifdef _RELEASE
	
	// 生成座標のデータを書き込む
	*emitTransform_ = worldtransform_.transform_;
	emitTransform_->translate.z += 1.0f;

	#endif // !_DEBUG

	// 基底クラスの更新
	IObject::Update();

	#ifdef _RELEASE
	// アニメーション更新
	anim_->Update();
	#endif // !_DEBUG

	// UIマネージャーに接地状態を渡す
	if (uiManager_ != nullptr) {
		uiManager_->SetIsGround(isGround_);
	}

	// 矢印UI更新関数
	if (!isGameClear_) {
		ArrowUIUpdate();
	}

	// コライダー
	circleCollider_.radius_ = worldtransform_.transform_.scale.x;
	CircleColliderUpdate();
	// 足元のコライダー
	footCollider_.Update();
	// レイ
	cameraRay_.Update();

	// 放物線
	if (throwDirect_.x != 0 || throwDirect_.y != 0) {
		parabola_.Update(
			worldtransform_.GetWorldPosition(),
			throwDirect_, this);
	}
	//else {
	//	parabola_.Reset();
	//}

	velocity2DManager_->SetVelocity(Vector2{ velocity_.x / 16.0f , -velocity_.y / 16.0f });

}

void Player::Draw(const BaseCamera& camera)
{
	if (isDraw_) {
		// 矢印用の座標
		Vector3 offset = throwDirect_ * 2.0f;
		screenPos_ = MathUtility::WorldToScreen(worldtransform_.GetWorldPosition() + offset, &const_cast<BaseCamera&>(camera));

		// プレイヤーの描画
		ModelDraw::AnimObjectDesc desc;
		desc.camera = &const_cast<BaseCamera&>(camera);
		desc.localMatrixManager = localMatrixManager_.get();
		desc.material = material_.get();
		desc.model = model_;
		desc.worldTransform = &worldtransform_;

		// デバッグ以外の場合行う
		#ifdef _RELEASE
		if (anim_->GetIsRight()) {
			ModelDraw::AnimObjectDraw(desc);
			ModelDraw::AnimObjectDraw(desc, 2);
		}
		else {
			ModelDraw::AnimInverseObjectDraw(desc);
			ModelDraw::AnimInverseObjectDraw(desc, 2);
		}
		#endif // !_DEBUG
		// デバッグのみで行う
		#ifndef _RELEASE
		ModelDraw::AnimObjectDraw(desc);
		ModelDraw::AnimObjectDraw(desc, 2);
		#endif // _DEBUG

		// 武器の描画
		if (weapon_) {
			weapon_->Draw(camera);
		}
		// 足場のモデル描画
		if (isDebugDraw_) {
			footCollider_.DebugDraw(camera);
		}

		// ポニーテール描画
		if (ponytail_ != nullptr) {
			ponytail_->Draw(const_cast<BaseCamera&>(camera));
		}

		// 矢印描画
		if (arrowModel_ != nullptr && std::holds_alternative<HoldState*>(weapon_->GetNowState()) && !isGameClear_) {
			arrowMaterial_->SetUvTransform(arrowUVTransform_.transform_);
			ModelDraw::NormalObjectDesc desc;
			desc.camera = &const_cast<BaseCamera&>(camera);
			desc.material = arrowMaterial_.get();
			desc.model = arrowModel_;
			desc.worldTransform = &arrowTransform_;
			ModelDraw::NormalObjectDraw(desc);
		}
	}
}

void Player::ImGuiDraw()
{
	ImGui::Begin("Player");
	landingAdjuster_.ImGuiDraw();
	controller_.ImGuiDraw();
	hpManager_.ImGuiDraw();
	correctSystem_.ImGuiDraw();
	slowEffect_->ImGuiDraw();

	// ゲームスピード
	//float ratio = IObject::sPlaySpeed;
	//ImGui::DragFloat("playTime", &ratio);
	//sPlaySpeed = ratio;
	// 反動フラグ
	ImGui::Text("%d : IsRecoil", recoil_.IsActive());
	int tex = IsCanReturn();
	ImGui::Text("%d : RetunCan", tex);
	// 状態の名前取得
	std::string name = typeid(*actionState_).name();
	ImGui::Text(name.c_str());
	// 座標リセット
	if (ImGui::Button("PosReset")) {
		Reset(Vector3{4.0f,3.0f,0.0f});
	}

	ImGui::DragFloat3("PlayerDirect", &worldtransform_.direction_.x);

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

	ImGui::Text("\n arrowTransfporm");
	ImGui::DragFloat3("rotate", &arrowTransform_.transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("scale", &arrowTransform_.transform_.scale.x, 0.01f);
	ImGui::Text("\n arrowUVTransfporm");
	ImGui::DragFloat3("UVscale", &arrowUVTransform_.transform_.scale.x, 0.01f);
	ImGui::DragFloat3("UVrotate", &arrowUVTransform_.transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("UVTranslate", &arrowUVTransform_.transform_.translate.x, 0.1f);

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
				float value = 3.0f;
				if (worldtransform_.GetWorldPosition().x > weapon_->worldtransform_.GetWorldPosition().x) {
					weapon_->velocity_.x = -1.0f * value;
				}
				else {
					weapon_->velocity_.x = 1.0f * value;
				}
				SetFallTimer();
				weapon_->ChangeRequest(Weapon::StateName::kFreeFall);
				return;
			}
			else {
				// 移動ベクトルが下向きの時にのみ
				if (velocity_.y < 0 && (!recoil_.IsActive()) && !isOneStepOn_) {

					// 踏む際の武器設定
					weapon_->TreadSetting();
					
					Vector2 leftStick = Input::GetInstance()->GetLeftAnalogstick();

					leftStick = { leftStick.x / SHRT_MAX,leftStick.y / SHRT_MAX };

					if (leftStick.x != 0) {
						// 槍じゃんステートへ
						spearJumpAccepter_.Start(2.0f);
						jumpAttack_.StartAccept(GlobalVariables::GetInstance()->GetFloatValue("SpearJump", "AttackFrame"));
						ChangeState(std::make_unique<SpearAerialState>());
					}
					else {
						// 槍じゃんステートへ
						ChangeState(std::make_unique<ActionWaitState>());
					}

					//// キャストして方向設定
					//SpearAerialState* state = dynamic_cast<SpearAerialState*>(actionState_.get());

					//state->InitializeDirection(leftStick);

					return;
				}
			}

			return;
		}
		// 帰ってきてる時の衝突
		else if (std::holds_alternative<ReturnState*>(weapon_->GetNowState())) {
			//Vector2 leftStick = Input::GetInstance()->GetLeftAnalogstick();
			//Vector2 direct = {};
			//if (leftStick.x > 0) {
			//	direct.x = 1.0f;
			//}
			//else if(leftStick.x < 0){
			//	direct.x = -1.0f;
			//}
			//// 反動生成
			//recoil_.CreateRecoil(Vector3(direct.x, direct.y, 0));
			
			//float acceptFrame = GlobalVariables::GetInstance()->GetFloatValue("Dash", "AcceptFrame");
			//assistDash_.StartAccept(acceptFrame);
			//// 着地している場合早期リターン
			if (std::holds_alternative<GroundState*>(nowState_)) {
				return;
			}
			//float upperPower = 25.0f;
			//if (velocity_.y < 0) {
			//	velocity_.y = 0;
			//}
			//velocity_.y += upperPower;
			return;
		}
	}
	// 雑魚敵との当たり判定
	else if (std::holds_alternative<Enemy*>(target)) {
		// 無敵中なら早期
		//if (invisibleTimer_.IsActive()) {
		//	return;
		//}
#ifdef _DEMO
		return;
#endif // !_DEBUG


		if (hpManager_.InvisibleActive() || knockBackSystem_.AcceptActive() || jumpAttack_.IsActive()) {
			return;
		}
		// 反動生成
		Enemy** enemy = std::get_if<Enemy*>(&target);
		if (std::holds_alternative<EnemyWaitState*>((*enemy)->GetState())) {
			return;
		}

		// 持ってないかどうか
		if (std::holds_alternative<HoldState*>(weapon_->GetNowState())) {
			// 持ってるから何か起きる
			// 反動生成
			//Enemy** enemy = std::get_if<Enemy*>(&target);
			Vector3 newDirect = {};
			newDirect.x = worldtransform_.GetWorldPosition().x - (*enemy)->GetWorldPosition().x;
			newDirect.y = 1.0f;
			knockBackSystem_.CreateKnockBack(newDirect);
			// 引き寄せの
			float value = 3.0f;
			if (worldtransform_.GetWorldPosition().x > weapon_->worldtransform_.GetWorldPosition().x) {
				weapon_->velocity_.x = -1.0f * value;
			}
			else {
				weapon_->velocity_.x = 1.0f * value;
			}
			SetFallTimer();

			weapon_->ChangeRequest(Weapon::StateName::kFreeFall);

			weaponGuardEffect_->StartShockWave(15.0f);

		}
		else {

			hpManager_.OnHit(1);
		}
		// キャンセル
		assistDash_.SlowCancel();

	}
	// ボス
	else if (std::holds_alternative<PrevSmallBoss*>(target)) {
		// 無敵中なら早期
		//if (invisibleTimer_.IsActive()) {
		//	return;
		//}

		// 持ってないかどうか
		if (std::holds_alternative<HoldState*>(weapon_->GetNowState())) {
			// 持ってるから何か起きる

		}
		else {
			hpManager_.OnHit(1);
		}

	}
	// 地形との当たり判定
	else if (std::holds_alternative<Terrain*>(target)) {
		// ジャンプ中の攻撃を中断
		jumpAttack_.Cancel();

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

		// capsule用
		Vector3 lerpPos = Ease::Easing(Ease::EaseName::Lerp, prevPosition_, worldtransform_.GetWorldPosition(), 0.95f);

		// 最小・最大値
		Vector2 plMin = { lerpPos.x - circleCollider_.radius_,lerpPos.y - circleCollider_.radius_ };
		Vector2 plMax = { lerpPos.x + circleCollider_.radius_,lerpPos.y + circleCollider_.radius_ };

		// 四頂点
		IObject::FourTop player4Point = IObject::GenerateFourTop(plMin, plMax);

		IObject::CollisionType type = IObject::GetCollisionType(player4Point, { minPos.x,minPos.y }, { maxPos.x,maxPos.y });
		Vector2 correctPosition = {};
		float correctValue = 0.1f;
		switch (type)
		{
			// 左側
		case IObject::kLeftSide:
			// プレイヤーの修正されたX座標を計算
			correctPosition.x = targetPos.x + targetRad.x + (scale2D_.x / 2.0f) + correctValue;
			worldtransform_.transform_.translate.x = correctPosition.x;
			velocity_.x = 0;
			break;
			// 右側
		case IObject::kRightSide:
			// プレイヤーの修正されたX座標を計算
			correctPosition.x = targetPos.x - targetRad.x - (scale2D_.x / 2.0f) - correctValue;
			worldtransform_.transform_.translate.x = correctPosition.x;
			velocity_.x = 0;
			break;
			// 上側
		case IObject::kTopSide:
			// バカデか移動
			if (std::fabsf(velocity_.y) > 85.0f) {
				// プレイヤーの修正されたY座標を計算
				if (velocity_.y > 0) {
					correctPosition.y = targetPos.y - targetRad.y - (scale2D_.y / 2.0f) - correctValue;
					worldtransform_.transform_.translate.y = correctPosition.y;
					velocity_.y = 0;
					return;
				}
				else if (velocity_.y < 0) {
					correctPosition.y = targetPos.y + targetRad.y + (scale2D_.y / 2.0f) + correctValue;
					worldtransform_.transform_.translate.y = correctPosition.y;
					velocity_.y = 0;
					return;
				}
			}
			else {
				correctPosition.y = targetPos.y - targetRad.y - (scale2D_.y / 2.0f) - correctValue;
				worldtransform_.transform_.translate.y = correctPosition.y;
				velocity_.y = 0;
				return;
			}
			break;
			// 下側
		case IObject::kBottomSide:
			// バカデか移動
			if (std::fabsf(velocity_.y) > 85.0f) {
				// プレイヤーの修正されたY座標を計算
				if (velocity_.y > 0) {
					correctPosition.y = targetPos.y - targetRad.y - (scale2D_.y / 2.0f) - correctValue;
					worldtransform_.transform_.translate.y = correctPosition.y;
					velocity_.y = 0;
					return;
				}
				else if (velocity_.y < 0) {
					correctPosition.y = targetPos.y + targetRad.y + (scale2D_.y / 2.0f) + correctValue;
					worldtransform_.transform_.translate.y = correctPosition.y;
					velocity_.y = 0;
					// プレイヤーが下向きに移動しており、空中にいる場合、着地状態に変更
					if (std::holds_alternative<AerialState*>(GetNowState()) || std::holds_alternative<SpearAerialState*>(GetNowState())) {
						ChangeState(std::make_unique<GroundState>());
					}
					isGround_ = true;

					return;
				}
			}
			// じゃない
			else {
				correctPosition.y = targetPos.y + targetRad.y + (circleCollider_.radius_) + correctValue;
				worldtransform_.transform_.translate.y = correctPosition.y;
				velocity_.y = 0;
				// プレイヤーが下向きに移動しており、空中にいる場合、着地状態に変更
				if (std::holds_alternative<AerialState*>(GetNowState()) || std::holds_alternative<SpearAerialState*>(GetNowState())) {
					ChangeState(std::make_unique<GroundState>());
				}
				isGround_ = true;
				return;
			}
			break;

			///---一点のみの衝突---///
#pragma region 左下
		case IObject::kLBPoint:
			// Yの移動量の方が大きい
			if (std::fabsf(moveDirect.x) < std::fabsf(moveDirect.y)) {
				if (moveDirect.y < 0) {
					// プレイヤーの修正されたY座標を計算
					correctPosition.y = targetPos.y + targetRad.y + (scale2D_.y / 2.0f) + correctValue;
					worldtransform_.transform_.translate.y = correctPosition.y;
					// プレイヤーが下向きに移動しており、空中にいる場合、着地状態に変更
					if (std::holds_alternative<AerialState*>(GetNowState()) || std::holds_alternative<SpearAerialState*>(GetNowState())) {
						ChangeState(std::make_unique<GroundState>());
					}
					isGround_ = true;
				}
			}
			// Xの移動量の方が大きい
			else if (std::fabsf(moveDirect.x) > std::fabsf(moveDirect.y)) {
				// プレイヤーの修正されたX座標を計算
				correctPosition.x = targetPos.x + targetRad.x + (scale2D_.x / 2.0f) + correctValue;
				worldtransform_.transform_.translate.x = correctPosition.x;
				velocity_.x = 0;
			}
			return;
			break;
#pragma endregion

#pragma region 左上
		case IObject::kLTPoint:
			// Yの移動量の方が大きい
			if (std::fabsf(moveDirect.x) < std::fabsf(moveDirect.y)) {
				if (moveDirect.y > 0) {
					// プレイヤーの修正されたY座標を計算
					correctPosition.y = targetPos.y - targetRad.y - (scale2D_.y / 2.0f) - correctValue;
					worldtransform_.transform_.translate.y = correctPosition.y;
					velocity_.y = 0;
				}
			}
			// Xの移動量の方が大きい
			else if (std::fabsf(moveDirect.x) > std::fabsf(moveDirect.y)) {
				// プレイヤーの修正されたX座標を計算
				correctPosition.x = targetPos.x + targetRad.x + (scale2D_.x / 2.0f) + correctValue;
				worldtransform_.transform_.translate.x = correctPosition.x;
				velocity_.x = 0;
			}
			return;
			break;
#pragma endregion

#pragma region 右下
		case IObject::kRBPoint:
			// Yの移動量の方が大きい
			if (std::fabsf(moveDirect.x) < std::fabsf(moveDirect.y)) {
				if (moveDirect.y < 0) {
					// プレイヤーの修正されたY座標を計算
					correctPosition.y = targetPos.y + targetRad.y + (scale2D_.y / 2.0f) + correctValue;
					worldtransform_.transform_.translate.y = correctPosition.y;
					// プレイヤーが下向きに移動しており、空中にいる場合、着地状態に変更
					if (std::holds_alternative<AerialState*>(GetNowState()) || std::holds_alternative<SpearAerialState*>(GetNowState())) {
						ChangeState(std::make_unique<GroundState>());
					}
					isGround_ = true;
				}
			}
			// Xの移動量の方が大きい
			else if (std::fabsf(moveDirect.x) > std::fabsf(moveDirect.y)) {
				// プレイヤーの修正されたX座標を計算
				correctPosition.x = targetPos.x - targetRad.x - (scale2D_.x / 2.0f) - correctValue;
				worldtransform_.transform_.translate.x = correctPosition.x;
				velocity_.x = 0;
			}
			return;
			break;
#pragma endregion

#pragma region 右上
		case IObject::kRTPoint:
			// Yの移動量の方が大きい
			if (std::fabsf(moveDirect.x) < std::fabsf(moveDirect.y)) {
				if (moveDirect.y > 0) {
					// プレイヤーの修正されたY座標を計算
					correctPosition.y = targetPos.y - targetRad.y - (scale2D_.y / 2.0f) - correctValue;
					worldtransform_.transform_.translate.y = correctPosition.y;
					velocity_.y = 0;
				}
			}
			// Xの移動量の方が大きい
			else if (std::fabsf(moveDirect.x) > std::fabsf(moveDirect.y)) {
				// プレイヤーの修正されたX座標を計算
				correctPosition.x = targetPos.x - targetRad.x - (scale2D_.x / 2.0f) - correctValue;
				worldtransform_.transform_.translate.x = correctPosition.x;
				velocity_.x = 0;
			}
			return;
			break;
#pragma endregion

			///---（３点以上）---///
		case IObject::kMultiPoints:
			correctPosition = Ease::Easing(Ease::EaseName::Lerp, Vector2{ worldtransform_.GetWorldPosition().x,worldtransform_.GetWorldPosition().y },
				Vector2{ prevPosition_.x,prevPosition_.y }, 0.15f);
			worldtransform_.transform_.translate = { correctPosition.x,correctPosition.y,0 };

			break;
		case IObject::kNone:

			break;
		}

		IObject::FourTop block4Top = IObject::GenerateFourTop({ minPos.x,minPos.y }, { maxPos.x,maxPos.y });
		if (IObject::IsInsideCheck(worldtransform_.GetWorldPosition(), block4Top)) {
			Vector3 wPosP = worldtransform_.GetWorldPosition();
			if (wPosP.x > targetPos.x) {
				// プレイヤーの修正されたX座標を計算
				correctPosition.x = targetPos.x + targetRad.x + (scale2D_.x / 2.0f) + correctValue;
				worldtransform_.transform_.translate.x = correctPosition.x;
				velocity_.x = 0;
			}
			else {
				// プレイヤーの修正されたX座標を計算
				correctPosition.x = targetPos.x - targetRad.x - (scale2D_.x / 2.0f) - correctValue;
				worldtransform_.transform_.translate.x = correctPosition.x;
				velocity_.x = 0;
			}
		}

		// 更新
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

void Player::ClearProcessing()
{

	// クリアフラグ
	isGameClear_ = true;

	// 速度
	velocity_.x = 0.0f;

	// ステート変更
	weapon_->ChangeRequest(Weapon::StateName::kHold);

}

void Player::DrawLinesMap(DrawLine* drawLine)
{

	if (std::holds_alternative<HoldState*>(weapon_->GetNowState())){
		//parabola_.DrawMap(drawLine);

		Vector3 s = parabola_.GetStartPos();
		Vector3 e = parabola_.GetEndPos();
		subArrowVector_ = e - s;
	}

	//if (!std::holds_alternative<ThrownState*>(weapon_->GetNowState())) {
	//	LineForGPU lineForGPU;

	//	// 色
	//	lineForGPU.color[0] = connectingSpearLineColor_;
	//	lineForGPU.color[1] = connectingSpearLineColor_;

	//	lineForGPU.position[0] = worldtransform_.GetWorldPosition();
	//	lineForGPU.position[1] = weapon_->worldtransform_.GetWorldPosition();
	//	drawLine->Map(lineForGPU);
	//}

}

void Player::Reset(const Vector3& position)
{

	float addPositionY = 2.0f;

	worldtransform_.transform_.translate = position;
	worldtransform_.transform_.translate.y += addPositionY;
	velocity_ = {};
	worldtransform_.UpdateMatrix();
	isGround_ = true;

	// 復活の処理
	hpManager_.Respawn();

	weapon_->ChangeRequest(Weapon::StateName::kHold);

	localMatrixManager_->Map();

	// プレイヤーは再度描画する
	isDraw_ = true;

	// ポニーテールがセットされてる場合
	if (ponytail_ != nullptr) {
		// 行列を求める
		Matrix4x4 result = localMatrixManager_->GetNodeDatas()[11].matrix * worldtransform_.worldMatrix_;
		ponyAnchorPos_ = { result.m[3][0], result.m[3][1], result.m[3][2] };

		// 初期化の段階で全ばねの座標をセットする
		for (int i = 0; i < ponytail_->GetSpring().size() + 1; i++) {
			// 追従先座標を渡す
			ponytail_->SetPosition(i, ponyAnchorPos_);
		}

		// 更新
		ponytail_->Update();
	}

	hpManager_.HitEffectTimerEnd();

}

void Player::SetFallTimer()
{
	float fallTimerFrame = GlobalVariables::GetInstance()->GetFloatValue("Weapon", "KickBackCooltime");
	this->fallTimer_.StartSetting(fallTimerFrame);
}

void Player::ArrowUIUpdate()
{
	// 線の長さを取得する
	float s = Vector3::Length(subArrowVector_);
	// 正規化ベクトルを求める
	Vector3 v = Vector3::Normalize(subArrowVector_);
	
	// 矢印UIの回転処理
	float l = std::sqrtf(v.x * v.x + v.y * v.y);
	float cos = v.x / l;
	float sin = v.y / l;

	float angle = std::acos(cos);

	// 負の角度だった場合の処理
	if (sin < 0) {
		angle = -angle;
	}

	/*Vector2 stickDirect = controller_.GetInput()->GetRightAnalogstick();

	Vector2 deadZone = { stickDirect.x / SHRT_MAX,stickDirect.y / SHRT_MAX };
	float deadZoneValue = 0.25f;
	if ((std::fabsf(stickDirect.x) > deadZoneValue || std::fabsf(stickDirect.y) > deadZoneValue)){
		currentArrowStagingTime_ += kDeltaTime_;
	}*/

	// 演出時間でUV座標をイージング
	if (currentArrowStagingTime_ < arrowStagingTime_) {
		arrowUVTransform_.transform_.translate = Ease::Easing(Ease::EaseName::Lerp, { 0.0f, 0.0f, 0.0f }, { -1.0f ,0.0f, 0.0f }, currentArrowStagingTime_ / arrowStagingTime_);
		currentArrowStagingTime_ += kDeltaTime_;
	}
	else {
		arrowUVTransform_.transform_.translate = { -1.0f, 0.0f, 0.0f };
		currentArrowStagingTime_ = 0.0f;
	}

	// 座標初期化
	arrowTransform_.transform_.translate = worldtransform_.transform_.translate;
	arrowTransform_.transform_.scale.x = s;
	arrowUVTransform_.transform_.scale.x = s;
	arrowTransform_.transform_.rotate.z = angle;
	// トランスフォーム更新
	arrowTransform_.UpdateMatrix();
	arrowUVTransform_.UpdateMatrix();
}

void Player::SetPonyTail(Model* model)
{
	// 行列を求める
	Matrix4x4 result = localMatrixManager_->GetNodeDatas()[11].matrix * worldtransform_.worldMatrix_;
	ponyAnchorPos_ = { result.m[3][0], result.m[3][1], result.m[3][2] };

	// ポニーテール用紐生成
	ponytail_ = std::make_unique<String>();
	// 初期化
	ponytail_->Initialize(
		model,
		ponyAnchorPos_,
		0.1f,
		750.0f,
		2.0f,
		0.5f);

	// アンカー設定
	ponytail_->SetAnchor(0, true);

	// 初期化の段階で全ばねの座標をセットする
	for (int i = 0; i < ponytail_->GetSpring().size(); i++) {
		// 追従先座標を渡す
		ponytail_->SetPosition(i, ponyAnchorPos_);
	}

	// 更新
	ponytail_->Update();
}

void Player::SetArrowModel(Model* model)
{
	// モデル取得
	arrowModel_ = model;

	float s = Vector3::Length(subArrowVector_);

	// 座標初期化
	arrowTransform_.Initialize(arrowModel_->GetRootNode());
	arrowTransform_.transform_.translate = worldtransform_.transform_.translate;
	arrowTransform_.transform_.scale.x = s;
	arrowUVTransform_.transform_.scale.x = s;
	// トランスフォーム更新
	arrowTransform_.UpdateMatrix();
	arrowUVTransform_.UpdateMatrix();

	// 矢印マテリアル初期化
	arrowMaterial_.reset(Material::Create());
	arrowMaterial_->SetEnableLighting(EnableLighting::None);
	arrowMaterial_->SetShininess(shininess_);
	
	// uvトランスフォーム初期化
	arrowUVTransform_.Initialize();
}

void Player::SystemInitialize()
{
	// 入力処理受付クラス
	controller_.Initialize(this);
	// 反動クラス
	recoil_.Initialize(this);
	// 足場クラス
	footCollider_.Initialize(model_, this);
	// 補正クラス
	correctSystem_.Initialize(this);
	// HPクラス
	hpManager_.Initialize(this);
	// コンボクラス
	jumpCombo_.Reset();
	// 
	knockBackSystem_.Initialize(this);
	// 空中ダッシュのクラス
	assistDash_.Initialize(this);
	// 放物線
	parabola_.Initialize();
	connectingSpearLineColor_ = { 0.8f, 0.0f, 0.8f, 1.0f };
	// レイ
	rayLength_ = -100.0f;
	cameraRay_.Initialize(this);
	// スローエフェクト
	slowEffect_ = std::make_unique<SlowEffect>();
	slowEffect_->Initalize(this);
	// 着地アシスト
	landingAdjuster_.Initialize(this, weapon_.get());

	// ガードエフェクト
	weaponGuardEffect_ = std::make_unique<WeaponGuardEffect>();
	weaponGuardEffect_->Initialize(this);

	// ジャンプ攻撃
	jumpAttack_.Initialize(this);
}

void Player::SystemUpdate()
{
	// 操作クラス
	controller_.Update();
	// 反動クラス
	recoil_.Update();
	// ノックバック
	knockBackSystem_.Update();
	// 
	correctSystem_.Update(enemyManager_);
	// 落下中の引き寄せタイマークラス
	fallTimer_.Update();
	// 無敵時間の処理もするので更新必須
	hpManager_.Update();
	// 仮の踏んだ時のチェック
	spearJumpAccepter_.Update();
	// 空中ダッシュ
	assistDash_.Update();
	// スローエフェクト
	slowEffect_->Update();
	// 着地アシスト
	landingAdjuster_.Update();

	// 
	throwStopTimer_.Update();

	// ガードエフェクト
	weaponGuardEffect_->Update();
	// ジャンプ攻撃判定
	jumpAttack_.Update();
}
