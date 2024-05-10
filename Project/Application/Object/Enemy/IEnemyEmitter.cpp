#include "IEnemyEmitter.h"
#include "Enemy.h"

#include <numbers>
#include <cmath>

uint32_t IEnemyEmitter::sSerialNumber_ = 0;

void IEnemyEmitter::Initialize(Model* model)
{
	LargeNumberOfObjects::Initialize(model);

	worldTransform_.Initialize();
}

void IEnemyEmitter::Update()
{
	// リストの更新処理
	LargeNumberOfObjects::Update();

	// フラグによる死亡処理
	objects_.remove_if([this](std::unique_ptr<OneOfManyObjects>& enemy) {
		if (enemy->IsDead()) {
			enemy.reset();
			return true;
		}
		return false;
		});

	nowAngle_ += 1.0f / rotation_;
	//if (nowAngle_ >= 1.57f) {
	//	nowAngle_ = 0;
	//}

	worldTransform_.transform_.rotate.z = nowAngle_;
	worldTransform_.UpdateMatrix();
}

void IEnemyEmitter::InitializeEmitter(float spinSpeed)
{

	rotation_ = spinSpeed;

}

void IEnemyEmitter::CreateEnemy(const Vector3& transformPosition, float distance, uint32_t enemyCount)
{
	// エミッター側に書き込み
	worldTransform_.transform_.translate = transformPosition;
	distance_ = distance;
	maxCount_ = enemyCount;
	// 敵の角度生成
	float angleIncrement = 2.0f * (float)std::numbers::pi / maxCount_;
	Vector3 center = worldTransform_.GetWorldPosition();
	// 敵の生成
	for (uint32_t i = 0; i < maxCount_; ++i) {
		// 角度からオフセットの計算
		float angle = i * angleIncrement;
		Vector3 newPosition = {};
		newPosition.x = center.x + (std::cosf(angle) * distance_);
		newPosition.y = center.y + (std::sinf(angle) * distance_);

		// 生成
		std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
		obj->Initialize();
		//static_cast<Enemy*>(obj.get())->SetParent(&worldTransform_);
		static_cast<Enemy*>(obj.get())->SetEmitter(this);

		obj->transform_.translate = newPosition;
		// 初期化
		static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<EnemyGroundState>(), 0);
		// リストに追加
		objects_.push_back(std::move(obj));
	}

}
