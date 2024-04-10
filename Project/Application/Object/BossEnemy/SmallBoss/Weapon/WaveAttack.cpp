#include "WaveAttack.h"
#include "../../../Engine/Math/DeltaTime.h"

void WaveAttack::Initialize(Model* model)
{
	IAttackObject::Initialize(model);

	movePower_ = 5.0f;

}

void WaveAttack::Update()
{

	worldtransform_.transform_.translate += velocity_ * kDeltaTime_;

	// 基底
	IAttackObject::Update();
}

void WaveAttack::Draw(const BaseCamera& camera)
{
	model_->Draw(worldtransform_, const_cast<BaseCamera&>(camera));
}

void WaveAttack::ImGuiDraw()
{

	

}

void WaveAttack::OnCollision(ColliderParentObject2D target)
{
	target;
}
