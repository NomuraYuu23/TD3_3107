#include "WaveAttack.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../../../Engine/3D/ModelDraw.h"

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

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &const_cast<BaseCamera&>(camera);
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.textureHandles;
	desc.worldTransform;
	ModelDraw::AnimObjectDraw(desc);

}

void WaveAttack::ImGuiDraw()
{

	

}

void WaveAttack::OnCollision(ColliderParentObject2D target)
{
	target;
}
