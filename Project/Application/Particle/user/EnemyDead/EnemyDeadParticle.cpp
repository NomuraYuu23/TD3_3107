#include "EnemyDeadParticle.h"

void EnemyDeadParticle::Initialize(const Vector3& position, const Vector3& size)
{
	// 基底クラス初期化
	IParticle::Initialize(position, size);
}

void EnemyDeadParticle::Update(const Matrix4x4& billBoardMatrix)
{
	// 速度ベクトルにしたがって粒子を移動させる
	transform_.translate += velocity_;

	// 基底クラス更新
	IParticle::Update(billBoardMatrix);
}
