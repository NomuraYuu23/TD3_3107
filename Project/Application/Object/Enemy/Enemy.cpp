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

	// コライダーの初期化
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, 0.0f, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributeEnemy);
	boxCollider_.SetCollisionMask(kCollisionAttributeTerrain);

	// シリアル番号振り
	serialNum_ = sSerialNumber_;
	sSerialNumber_++;

}

void Enemy::Update()
{
	// 設定した状態の処理
	if (state_) {
		state_->Update();
	}

	// 基底クラスの更新
	OneOfManyObjects::Update();
	// 2D更新
	position2D_ = { worldMatrix_.m[3][0],worldMatrix_.m[3][1] };
	// コライダー
	BoxColliderUpdate();

}

void Enemy::ImGuiDraw()
{
	std::string name = "Enemy" + std::to_string(serialNum_);
	ImGui::Begin(name.c_str());

	ImGui::End();

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

		// 武器のポインタにキャスト
		Weapon** weaponPtr = std::get_if<Weapon*>(&target);
		if (weaponPtr != nullptr) {
			Weapon* weapon = *weaponPtr;
			if (std::holds_alternative<ThrownState*>(weapon->GetNowState())) {
				isDead_ = true;
			}
		}
		
	}
	// それ以外
	else {
		return;
	}
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
