#include "Weapon.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Input/Input.h"

#include "../../Collider2D/CollisionConfig2D.h"
#include "../GameUtility/MathUtility.h"
#include "../ObjectList.h"
#include "../Player/Player.h"

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
	#ifndef _DEBUG
	// アニメーション関連初期化
	anim_ = std::make_unique<SpearAnimManager>();  // 生成
	anim_->Init(this);							   // 初期化
	#endif // !_DEBUG

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
	#ifndef _DEBUG
	// アニメーション更新
	anim_->Update();
	#endif // !_DEBUG

	// コライダー
	Vector3 direct = worldtransform_.direction_;
	//float angle = MathUtility::CalcAngle({ direct.x,direct.y });
	float angle = std::atan2f(direct.y, direct.x) * (180.0f / 3.14f);
	boxCollider_.Update(position2D_, scale2D_.x, scale2D_.y, angle);


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

		// 壁・ブロックとの衝突判定
		if (std::holds_alternative<Terrain*>(target)) {

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
				IObject::FourTop player4Point = IObject::GenerateFourTop(weaponMin, weaponMax);
 				IObject::CollisionType type = IObject::GetCollisionType(player4Point, { minPos.x,minPos.y }, { maxPos.x,maxPos.y });

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

			invDirect_ = Vector2(worldtransform_.direction_.x, worldtransform_.direction_.y) * (-1.0f);
			ChangeRequest(Weapon::StateName::kImpaled);
			return;
		}
		else if (std::holds_alternative<Enemy*>(target)) {
			invDirect_ = Vector2(worldtransform_.direction_.x, worldtransform_.direction_.y) * (-1.0f);
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
