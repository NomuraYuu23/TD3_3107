#include "Enemy.h"
#include "../../Collider2D/CollisionConfig2D.h"

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
	boxCollider_.SetCollisionMask(kCollisionAttributeWeapon);

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

}

void Enemy::OnCollision(ColliderParentObject2D target)
{
	target;
	BoxColliderUpdate();
}

void Enemy::GenerateSetting()
{



}

void Enemy::StateInitialize(std::unique_ptr<IEnemyState> newState, uint32_t attackPattern)
{
	// ステートの初期化前の情報設定
	newState->PreInitialize(this, attackPattern);
	// ステートの初期化
	newState->Initialize();
	// ステートの設定
	state_ = std::move(newState);
}
