#include "Enemy.h"
#include "../../Collider2D/CollisionConfig2D.h"
#include "../ObjectList.h"
#include "../Weapon/WeaponState/StateList.h"

uint32_t Enemy::sSerialNumber_ = 0;

void Enemy::Initialize()
{
	// 基底クラスの初期化
	OneOfManyObjects::Initialize();

	// コライダー用の座標・スケール
	position2D_ = { transform_.translate.x,transform_.translate.y };
	scale2D_ = { transform_.scale.x, transform_.scale.y };
	scale2D_ = { 2.0f,2.0f };
	// コライダーの初期化
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, 0.0f, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributeEnemy);
	boxCollider_.SetCollisionMask(kCollisionAttributeTerrain);

	// シリアル番号振り
	serialNum_ = sSerialNumber_;
	sSerialNumber_++;

	name_ = "Enemy" + std::to_string(serialNum_);

	isDead_ = false;
	isGround_ = false;
}

void Enemy::Update()
{
	prevPosition_ = { transform_.translate.x,transform_.translate.y };
	// 前フレーム座標取得
	prevTranslate_ = transform_.translate;
	// 設定した状態の処理
	if (state_) {
		state_->Update();
	}
	interval_.Update();
	//transform_.translate = {}
	if (parentEmitter_) {
		CheckParent();
	}
	MatrixUpdate();
	// 2D更新
	position2D_ = { worldMatrix_.m[3][0],worldMatrix_.m[3][1] };
	// コライダー
	BoxColliderUpdate();

}

void Enemy::ImGuiDraw()
{
	//ImGui::Begin(name.c_str());
	ImGui::SeparatorText(name_.c_str());
	ImGui::DragFloat3("WorldPosition", &transform_.translate.x);
	std::string rot = "transform" + name_;
	ImGui::DragFloat3(rot.c_str(), &transform_.rotate.x, 0.01f);
	ImGui::DragFloat2("scale", &scale2D_.x);
	ImGui::Text("%d", isDead_);
	std::string name = typeid(*state_).name();
	ImGui::Text(name.c_str());

	if (ImGui::Button("Release")) {
		this->ReleaseParent();
	}


	//ImGui::End();

}

void Enemy::OnCollision(ColliderParentObject2D target)
{
	/// ワンちゃん壁との判定も取るかも

	// プレイヤーの場合
	if (std::holds_alternative<Player*>(target)) {
		Player** playerPtr = std::get_if<Player*>(&target);
		if (playerPtr != nullptr) {
			Player* player = *playerPtr;
			if (std::holds_alternative<HoldState*>(player->GetWeapon()->GetNowState())) {
				// こいつ吹っ飛ぶ処理をここに
				//transform_.translate.y += 1;
			}
		}
	}
	// 武器の場合
	else if (std::holds_alternative<Weapon*>(target)) {

		//// 武器のポインタにキャスト
		Weapon** weapon = std::get_if<Weapon*>(&target);
		if (std::holds_alternative<ImpaledState*>((*weapon)->GetNowState())) {
			if ((*weapon)->IsEnemyImpaled()) {
				return;
			}
			if (!std::holds_alternative<EnemyWaitState*>(judState_)) {
				ChangeState(std::make_unique<EnemyWaitState>(), IEnemyState::AttackPattern::kMaxSize);
				weapon_ = (*weapon);
				// 槍が刺さった効果音を再生
				weapon_->GetPlayer()->gameAudioManager_->PlayWave(GameAudioNameIndex::kSpearSting);
			}
		}
		//else if (std::holds_alternative<FreeFallState*>((*weapon)->GetNowState()) ||
		//	std::holds_alternative<ReturnState*>((*weapon)->GetNowState())) {
		else if((*weapon)->IsPlayerJump()){
			// 死亡パーティクル再生
			EmitterDesc desc;
			desc.transform = &transform_;
			desc.instanceCount = 25;
			desc.frequency = 0.01f;
			desc.lifeTime = 0.01f; 
			desc.particleModelNum = kCircle;
			desc.paeticleName = kEnemyDeadParticle;

			ParticleManager::GetInstance()->MakeEmitter(&desc, 0);

			// 敵を倒す効果音を再生
			(*weapon)->GetPlayer()->gameAudioManager_->PlayWave(GameAudioNameIndex::kEliminateEnemy);

			isDead_ = true;
		}

	}
	else if (std::holds_alternative<Terrain*>(target)) {

		Vector2 targetPos = {};
		Vector2 targetRad = {};
		// 対象の情報取得
		std::visit([&](const auto& a) {
			targetPos = a->GetColliderPosition();
			targetRad = a->GetColliderSize();
			}, target);

		if (std::fabsf(velocity_.x) > 0) {
			velocity_.x *= -1.0f;
		}
		
		if (std::fabsf(velocity_.y) > 0) {
			//velocity_.y *= -1.0f;
			isGround_ = true;
			velocity_.y = 0;
			if (targetPos.y > prevPosition_.y) {
				transform_.translate.y = targetPos.y - targetRad.y;
			}
			else if (targetPos.y < prevPosition_.y) {
				transform_.translate.y = targetPos.y + targetRad.y;
			}
		}
	}
	// それ以外
	else {
		return;
	}
}

void Enemy::MatrixUpdate()
{
	// 基底クラスの更新
	OneOfManyObjects::Update();
}

void Enemy::GenerateSetting()
{



}

void Enemy::StateInitialize(std::unique_ptr<IEnemyState> newState, uint32_t attackPattern)
{
	// ステートの初期化前の情報設定
	newState->PreInitialize(this, static_cast<IEnemyState::AttackPattern>(attackPattern));
	// ステートの初期化
	newState->Initialize();
	// ステートの設定
	state_ = std::move(newState);
}

void Enemy::ChangeState(std::unique_ptr<IEnemyState> newState, IEnemyState::AttackPattern pattern)
{
	if (pattern == IEnemyState::AttackPattern::kMaxSize) {
		newState->PreInitialize(this);
	}
	else {
		newState->PreInitialize(this, pattern);
	}

	newState->Initialize();

	state_ = std::move(newState);
}

void Enemy::CheckParent()
{
	if (!parent_) {
		//if (!parentEmitter_->IsRotateReturn() && (goalAngle_ == parentEmitter_->GetNowAngle())) {
		if (!interval_.IsActive() && (goalAngle_ == parentEmitter_->GetNowAngle())) {
			if (weapon_) {
				weapon_->ChangeRequest(Weapon::StateName::kReturn);
			}
			ResetParent();
			ChangeState(std::make_unique<EnemyAerialState>(), static_cast<IEnemyState::AttackPattern>(0));
		}
	}
}
