#include "Weapon.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Input/Input.h"

#include "../../Collider2D/CollisionConfig2D.h"
#include "../GameUtility/MathUtility.h"
#include "../ObjectList.h"
#include "../Player/Player.h"

#include "../../Particle/EmitterName.h"
#include "../../Particle/ParticleName.h"

void Weapon::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// ライティング有効
	enableLighting_ = EnableLighting::HalfLambert;
	material_->SetEnableLighting(enableLighting_);

	// 親子関係でのオフセット
	worldtransform_.transform_.translate = GlobalVariables::GetInstance()->GetVector3Value("Weapon", "LocalPosition");
	worldtransform_.usedDirection_ = true;
	float scaleRate = globalVariables->GetFloatValue("Weapon", "ScaleRate");
	//float scaleRate = 2.5f;
	worldtransform_.transform_.scale = { 1.0f,1.0f,1.0f };
	scale2D_ = { scaleRate * 2.0f,0.10f };

	// コライダーの初期化
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, 0.0f, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributeWeapon);
	boxCollider_.SetCollisionMask(kCollisionAttributePlayer);

	// ステート変更
	//ChangeState(std::make_unique<HoldState>());
	// 戻るレート
	returnRate_ = 1.3f;
	dotAngle_ = globalVariables->GetFloatValue("Weapon", "AngleDot");

	// システム初期化
	SystemInitialize();

	// デバッグ以外の場合行う
	#ifdef _RELEASE
	// アニメーション関連初期化
	spearAnim_ = std::make_unique<SpearAnimManager>();  // 生成
	spearAnim_->Init(this);							   // 初期化
	#endif // !_DEBUG

	//velocity2DManager_ = std::make_unique<Velocity2DManager>();
	//velocity2DManager_->Initialize();

}

void Weapon::Update()
{
	isGravity_ = false;
	isEnemyImpaled_ = false;
	isPlayerJumpAccept_ = player_->spearJumpAccepter_.IsActive();
	//shockWaveManager_->SetCenter({ worldtransform_.GetWorldPosition().x,worldtransform_.GetWorldPosition().y });
	prevDirect_ = { worldtransform_.direction_.x,worldtransform_.direction_.y };

	// 状態ごとの更新
	if (state_) {
		state_->Update();
	}

	// 二段ジャンプのクールタイム設定
	if (isTread_ && timer_.IsEnd()) {
		isTread_ = false;
	}
	// システム更新
	this->SystemUpdate();

	// 基底クラスの更新
	IObject::Update();

	// デバッグ以外の場合行う
	#ifdef _RELEASE

	// プレイヤーが槍を保持している場合
	if (isHold_) {
		// リングの角度を求める
		float angle;

		// ベクトルの大きさを計算
		double magnitude = std::sqrt(throwDirect_.x * throwDirect_.x + throwDirect_.y * throwDirect_.y);

		// x と y が両方とも 0 の場合は、角度が未定義なので、0を返す
		if (magnitude == 0) {
			angle = 0.0f;
		};

		// cosθ = x / magnitude を使って角度を計算
		double cos_theta = throwDirect_.x / magnitude;
		angle = static_cast<float>(std::acos(cos_theta));

		// y が負の場合は、角度を反転させる
		if (throwDirect_.y < 0.0f) {
			angle = -angle;
		}

		// 武器にリングを追従
		ringUnderTransform_.transform_.translate = worldtransform_.transform_.translate;
		ringTopTransform_.transform_.translate = worldtransform_.transform_.translate;
		ringUnderTransform_.transform_.rotate.z = angle;
		ringTopTransform_.transform_.rotate.z = angle;
		// ワールドトランスフォームの更新
		ringUnderTransform_.UpdateMatrix();
		ringTopTransform_.UpdateMatrix();
	}
	
	// 槍が投げられている状態であれば
	if (!std::holds_alternative<HoldState*>(nowState_)) {
		ringUnderTransform_.transform_.scale = Ease::Easing(Ease::EaseName::EaseOutQuad, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, (ringCurrentTime_ / ringStagingTime_));
		ringTopTransform_.transform_.scale = Ease::Easing(Ease::EaseName::EaseOutQuad, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, (ringCurrentTime_ / ringStagingTime_));
		ringColor_.w = 1.0f - (ringCurrentTime_ / ringStagingTime_);
		Vector3 ringColorRGB = Vector3::Lerp( { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.1f }, (ringCurrentTime_ / ringStagingTime_));
		ringColor_ = { ringColorRGB.x, ringColorRGB.y, ringColorRGB.z, ringColor_.w };

		// ワールドトランスフォームの更新
		ringUnderTransform_.UpdateMatrix();
		ringTopTransform_.UpdateMatrix();

		if (ringCurrentTime_ < ringStagingTime_) {
			// 演出時間加算
			ringCurrentTime_ += kDeltaTime_;
		}
		else { // 時間超過時
			// 演出時間リセット
			ringCurrentTime_ = ringStagingTime_;
		}
		
		// マテリアル色を設定
		ringMaterial_->SetColor(ringColor_);

		// マテリアル更新
		ringMaterial_->Update(ringUVTransform_.transform_, ringColor_, EnableLighting::None, 100.0f,0.0f);
	}
	else {
		// サイズを0に
		ringUnderTransform_.transform_.scale = { 0.0f, 0.0f, 0.0f };
		ringTopTransform_.transform_.scale = { 0.0f, 0.0f, 0.0f };
	}

	// リングアニメーションが再生されていない場合
	//if (!spearAnim_->GetRingAnim().GetRunningAnimation(SpearAnimManager::RingShot)) {
	//	spearAnim_->PlayRingAnimation(SpearAnimManager::RingIdle, true);
	//}
	//else {
	//	// 再生中であればそのアニメーションに合わせてマテリアルを透明にしていく
	//	ringColor_ = { 1.0f, 1.0f, 1.0f, MathUtility::Lerp(1.0f, 0.0f, spearAnim_->GetRingAnim().GetAnimationProgress(SpearAnimManager::RingShot)) };
	//	// マテリアル色を設定
	//	ringMaterial_->SetColor(ringColor_);

	//	// マテリアル更新
	//	ringMaterial_->Update(ringUVTransform_.transform_, ringColor_, EnableLighting::None, 100.0f);
	//}

	// アニメーション更新
	spearAnim_->Update();
	#endif // !_DEBUG

	// コライダー
	Vector3 direct = worldtransform_.direction_;
	//float angle = MathUtility::CalcAngle({ direct.x,direct.y });
	float angle = std::atan2f(direct.y, direct.x) * (180.0f / 3.14f);
	boxCollider_.Update(position2D_, scale2D_.x, scale2D_.y, angle);

	//velocity2DManager_->SetVelocity(Vector2{ /*returnDirect_.x **/ 10.0f, returnDirect_.y * 10.0f });

}

void Weapon::Draw(const BaseCamera& camera)
{
	if (std::holds_alternative<ThrownState*>(nowState_))
	{
		// 画面外に出たら戻るステートに変更
		float deadLength = 750.0f;
		if (MathUtility::CheckOutScreen(worldtransform_.GetWorldPosition(), deadLength, camera)) {
			ChangeRequest(StateName::kReturn);
		}
	}

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &const_cast<BaseCamera&>(camera);
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldtransform_;
	ModelDraw::AnimObjectDraw(desc);

	//ModelDraw::AnimObjectDraw(desc, 2);

}

void Weapon::UnderRingDraw(const BaseCamera& camera)
{
	// リング描画
	if (isDrawRing_) {
		// 武器リングの描画
		ModelDraw::AnimObjectDesc ringDesc;
		ringDesc.camera = &const_cast<BaseCamera&>(camera);
		ringDesc.localMatrixManager = ringLocalMatrix_.get();
		ringDesc.material = ringMaterial_.get();
		ringDesc.model = ringUnderModel_;
		ringDesc.worldTransform = &ringUnderTransform_;
		ModelDraw::AnimObjectDraw(ringDesc);
	}
}

void Weapon::TopRingDraw(const BaseCamera& camera)
{
	// リング描画
	if (isDrawRing_) {
		// 武器リングの描画
		ModelDraw::AnimObjectDesc ringDesc;
		ringDesc.camera = &const_cast<BaseCamera&>(camera);
		ringDesc.localMatrixManager = ringLocalMatrix_.get();
		ringDesc.material = ringMaterial_.get();
		ringDesc.model = ringTopModel_;
		ringDesc.worldTransform = &ringTopTransform_;
		ModelDraw::AnimObjectDraw(ringDesc);
	}
}

void Weapon::ImGuiDraw()
{
	ImGui::Begin("Weapon");
	// 衝撃波をまとめてるシステムのImGUi
	ImGui::SeparatorText("EffectSystem");
	shockEffect_.ImGuiDraw();
	ImGui::Text("\n");

	ImGui::Separator();
	// 親子変更
	if (ImGui::Button("ParentDelete")) {
		ReleaseParent();
	}
	if (ImGui::Button("ParentAdd")) {
		SettingParent();
	}

	if (ImGui::Button("ResetButton"))
	{
		ChangeRequest(StateName::kHold);
	}

	ImGui::DragFloat("dotAngle", &dotAngle_, 0.01f, -1.0f, 1.0f);

	Vector3 direct = worldtransform_.direction_;
	float angle = MathUtility::CalcAngle(position2D_, { direct.x,direct.y });

	ImGui::DragFloat("kakudo", &angle);
	ImGui::Text("%d : isGravity", isGravity_);
	ImGui::DragFloat3("Velocity", &this->velocity_.x);
	ImGui::DragFloat("angle", &rotateAngle_, 0.01f, 0, 100.0f);
	angle = std::atan2f(direct.y, direct.x) * (180.0f / 3.14f);

	ImGui::DragFloat("DirectAngle", &angle);

	ImGui::Text("%d : IsCollision", isCollisionCheck_);

	isCollisionCheck_ = false;
	ImGui::SeparatorText("Collider");
	ImGui::DragFloat2("CollV2", &boxCollider_.position_.x);
	ImGui::DragFloat2("ScaleColl", &scale2D_.x, 0.01f, 0, 10.0f);

	ImGui::SeparatorText("WorldTransform");
	// ローカル座標
	ImGui::DragFloat3("localPos", &worldtransform_.transform_.translate.x, 0.01f, -40.0f, 40.0f);
	// サイズ
	ImGui::DragFloat3("scla", &worldtransform_.transform_.scale.x, 0.01f, 0, 100);
	// 回転処理
	ImGui::DragFloat3("RotateDirect", &worldtransform_.direction_.x, 0.01f, -360.0f, 360.0f);
	// オイラー角
	ImGui::DragFloat3("Rotation", &worldtransform_.transform_.rotate.x);

	ImGui::Checkbox("isDirect", &worldtransform_.usedDirection_);

	// どっちかを判断
	std::string name = typeid(*state_).name();

	ImGui::Text(name.c_str());

	ImGui::Separator();
	std::string stateName;
	switch (hitBlockType_)
	{
	case Terrain::BlockType::kNone:
		stateName = "kNone";
		break;
	case Terrain::BlockType::kTerrain:
		stateName = "kTerrain";
		break;
	case Terrain::BlockType::kObstacle:
		stateName = "kObstacle";
		break;
	case Terrain::BlockType::kWall:
		stateName = "kWall";
		break;
	case Terrain::BlockType::kMaxSize:
		stateName = "kMaxSize";
		break;
	}
	ImGui::Text(stateName.c_str());

	if (ImGui::BeginTabBar("State")) {
		if (ImGui::BeginTabItem("Thrown")) {

			ImGui::DragFloat3("ThrowDirect", &this->throwDirect_.x);

			ImGui::EndTabItem();
		}

		// 共通項目
		if (ImGui::BeginTabItem("Return")) {

			ImGui::DragFloat2("returnDirect", &returnDirect_.x);
			ImGui::DragFloat2("invDirect", &invDirect_.x);
			ImGui::DragFloat("returnRate", &returnRate_, 0.01f, 0.01f, 10.0f);

			ImGui::EndTabItem();
		}

		// タブバーを終了
		ImGui::EndTabBar();
	}

	ImGui::End();

	if (state_) {
		state_->ImGuiUpdate();
	}

}

void Weapon::OnCollision(ColliderParentObject2D target)
{
	//if (std::holds_alternative<Terrain*>(target)) {
	//	isCollisionCheck_ = true;
	//	target;
	//}
	if (std::holds_alternative<Enemy*>(target)) {
		isCollisionCheck_ = true;
		target;
	}
	// 持っている状態なら早期
	if (std::holds_alternative<HoldState*>(nowState_)) {
		return;
	}

	// 投げられてる状態
	if (std::holds_alternative<ThrownState*>(nowState_))
	{
		// プレイヤーとの
		if (std::holds_alternative<Player*>(target)) {
			return;
		}

		Vector2 targetPos = {};
		// 対象の情報取得
		std::visit([&](const auto& a) {
			targetPos = a->GetColliderPosition();
			}, target);
		// 敵
		if (std::holds_alternative<Enemy*>(target)) {
			invDirect_ = Vector2(worldtransform_.direction_.x, worldtransform_.direction_.y) * (-1.0f);
			ChangeRequest(Weapon::StateName::kImpaled);
			return;
		}
		// 壁・ブロックとの衝突判定
		else if (std::holds_alternative<Terrain*>(target)) {
			if (throwInvTimer_.IsActive()) {

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
				Vector3 weaponPosition = worldtransform_.GetWorldPosition();
				// 最小・最大値
				float maxSize = (boxCollider_.scale_.x > boxCollider_.scale_.y) ? boxCollider_.scale_.x : boxCollider_.scale_.y;
				Vector2 weaponMin = { weaponPosition.x - maxSize,weaponPosition.y - maxSize };
				Vector2 weaponMax = { weaponPosition.x + maxSize,weaponPosition.y + maxSize };

				// 四頂点
				IObject::FourTop weapon4Point = IObject::GenerateFourTop(weaponMin, weaponMax);
				IObject::CollisionType type = IObject::GetCollisionType(weapon4Point, { minPos.x,minPos.y }, { maxPos.x,maxPos.y });

				if ((worldtransform_.direction_.y > 0) && type == IObject::CollisionType::kBottomSide) {
					return;
				}
				else if ((worldtransform_.direction_.y < 0) && type == IObject::CollisionType::kTopSide) {
					return;
				}
				else if ((worldtransform_.direction_.x < 0) && type == IObject::CollisionType::kRightSide) {
					return;
				}
				else if ((worldtransform_.direction_.x > 0) && type == IObject::CollisionType::kLeftSide) {
					return;
				}

				if (worldtransform_.direction_.y < 0 &&
					((type == IObject::CollisionType::kRTPoint) || (type == IObject::CollisionType::kLTPoint))) {
					return;
				}
				else if (worldtransform_.direction_.y > 0 &&
					((type == IObject::CollisionType::kRBPoint) || (type == IObject::CollisionType::kLBPoint))) {
					return;
				}
				else if (worldtransform_.direction_.x > 0 &&
					((type == IObject::CollisionType::kLTPoint) || (type == IObject::CollisionType::kLBPoint))) {
					return;
				}
				else if (worldtransform_.direction_.x < 0 &&
					((type == IObject::CollisionType::kRBPoint) || (type == IObject::CollisionType::kRTPoint))) {
					return;
				}


			}
			// ポインタに
			Terrain** terrainPtr = std::get_if<Terrain*>(&target);
			// タイプ
			hitBlockType_ = (*terrainPtr)->typeNumber_;

			// 逆ベクトル
			invDirect_ = Vector2(worldtransform_.direction_.x, worldtransform_.direction_.y) * (-1.0f);

			Vector2 targetPos = {};
			Vector2 targetRad = {};
			// 対象の情報取得
			std::visit([&](const auto& a) {
				targetPos = a->GetColliderPosition();
				targetRad = a->GetColliderSize();
				}, target);
			targetRad *= 0.5f;
			// 右上
			Vector2 maxPos = {
				targetPos.x + targetRad.x,	// 右
				targetPos.y + targetRad.y,	// 上
			};
			// 左下
			Vector2 minPos = {
				targetPos.x - targetRad.x,	// 左
				targetPos.y - targetRad.y,	// 下
			};

			IObject::FourTop blockFourTop = IObject::GenerateFourTop(minPos, maxPos);
			// めり込み回避用処理
			if (IObject::IsInsideCheck(worldtransform_.GetWorldPosition(), blockFourTop)) {
				worldtransform_.transform_.translate.x += invDirect_.x * 1.5f;
				worldtransform_.transform_.translate.y += invDirect_.y * 1.5f;
				worldtransform_.UpdateMatrix();
			}
			ChangeRequest(Weapon::StateName::kImpaled);
			return;
		}
	}
	// 返ってくる状態
	else if (std::holds_alternative<ReturnState*>(nowState_))
	{
		// プレイヤーとの
		if (std::holds_alternative<Player*>(target)) {
			// 差分ベクトルを求める
			Vector3 sub = player_->worldtransform_.transform_.translate - worldtransform_.transform_.translate;
			// 求めた差分ベクトル
			sub = Vector3::Normalize(sub);
			sub *= 0.5f;

			// デバッグ以外の場合行う
			#ifdef _RELEASE

			// キャッチ時パーティクル再生
			EmitterDesc desc;
			desc.transform = &player_->worldtransform_.transform_;
			desc.instanceCount = 5;
			desc.frequency = 0.01f;
			desc.lifeTime = 0.01f;
			desc.particleModelNum = kSpearLeaf;
			desc.paeticleName = kSpearCatchParticle;
			desc.velocity = { sub.x, sub.y, 0.0f };

			ParticleManager::GetInstance()->MakeEmitter(&desc, 0);

			#endif // _DEBUG

			ChangeRequest(Weapon::StateName::kHold);

			return;
		}
	}
	else if (std::holds_alternative<FreeFallState*>(nowState_)) {
		// プレイヤーとの
		if (!std::holds_alternative<Terrain*>(target)) {
			return;
		}
		else {
			// 落下中
			if (!attractInvTimer_.IsActive()) {
				// 角度修正
				if (std::fabsf(worldtransform_.direction_.x) >= 0.85f && std::fabsf(worldtransform_.direction_.x) <= 1.0f) {
					//if (worldtransform_.direction_.x > 0) {
					//	worldtransform_.direction_.x = 0.7f;
					//}
					//else {
					//	worldtransform_.direction_.x = -0.7f;
					//}
					Vector3 newDirect = MathUtility::RotateVector(Vector3(prevDirect_.x, prevDirect_.y, 0), (-3.14f / 24.0f) * 2.0f);

					worldtransform_.direction_ = { newDirect.x,newDirect.y };
					// 対象の情報取得
					Vector2 targetPos = {};
					Vector2 targetRadius = {};

					std::visit([&](const auto& a) {
						targetPos = a->GetColliderPosition();
						targetRadius = a->GetColliderSize();
						}, target);

					// 座標修正
					if (targetPos.y + targetRadius.y > worldtransform_.GetWorldPosition().y && (targetPos.x+ targetRadius.x> worldtransform_.GetWorldPosition().x && targetPos.x - targetRadius.x < worldtransform_.GetWorldPosition().x)) {
						worldtransform_.transform_.translate.y = targetPos.y + targetRadius.y;
					}

				}
				// ステート変更
				ChangeRequest(Weapon::StateName::kImpaled);
			}
			
			return;
		}
	}
	else if (std::holds_alternative<ImpaledState*>(nowState_)) {
		if (std::holds_alternative<Enemy*>(target)) {
			isEnemyImpaled_ = true;
			if (isPlayerJumpAccept_) {
				player_->SetFallTimer(); 
				ChangeRequest(Weapon::StateName::kFreeFall);
			}
			//Player** player = std::get_if<Player*>(&target);			
			//if (player != nullptr) {
			//	//Player* player = *playerPtr;
			//	if (std::holds_alternative<AttractState*>((*player)->GetNowState())) {
			//		ChangeRequest(Weapon::StateName::kFreeFall);
			//	}
			//}
		}
	}
}

void Weapon::SystemInitialize()
{
	shockEffect_.Initialize(this);
}

void Weapon::SystemUpdate()
{
	// タイマー
	timer_.Update();
	attractInvTimer_.Update();
	throwInvTimer_.Update();

	shockEffect_.Update();
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

void Weapon::SetRingModel(Model* upperModel, Model* underModel)
{
	// モデル取得
	ringUnderModel_ = underModel;
	ringTopModel_	= upperModel;

	// マテリアル生成
	ringMaterial_.reset(Material::Create());

	// トランスフォーム初期化
	ringUnderTransform_.Initialize(ringUnderModel_->GetRootNode());
	ringTopTransform_.Initialize(ringTopModel_->GetRootNode());
	// DirectiontoDirectionを使用
	//ringTransform_.usedDirection_ = true;
	// トランスフォーム更新
	ringUnderTransform_.UpdateMatrix();
	ringTopTransform_.UpdateMatrix();

	// ローカル行列マネージャ初期化
	ringLocalMatrix_ = std::make_unique<LocalMatrixManager>();
	ringLocalMatrix_->Initialize(ringUnderModel_->GetRootNode());

	// リングアニメーションのセットアップを開始する
	//spearAnim_->SetUpRingAnim();

	// uvトランスフォームの初期化
	ringUVTransform_.Initialize();
	// 更新
	ringUVTransform_.UpdateMatrix();
}

void Weapon::ChangeRequest(Weapon::StateName request)
{
	// 重力のフラグリセット
	//isGravity_ = false;
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
	case Weapon::StateName::kFreeFall:
		ChangeState(std::make_unique<FreeFallState>());
		break;
	}
}

void Weapon::TreadSetting()
{
	isTread_ = true;
	timer_.Start(30.0f);
}
