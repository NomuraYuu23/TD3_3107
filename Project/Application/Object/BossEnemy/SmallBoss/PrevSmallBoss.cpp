#include "PrevSmallBoss.h"

void PrevSmallBoss::Initialize(Model* model)
{

	IBoss::Initialize(model);

}

void PrevSmallBoss::Update()
{

	// 更新処理
	IBoss::Update();
	BoxColliderUpdate();
}

void PrevSmallBoss::Draw(const BaseCamera& camera)
{
	// 描画
	model_->Draw(worldtransform_, const_cast<BaseCamera&>(camera));
}

void PrevSmallBoss::ImGuiDraw()
{
}

void PrevSmallBoss::OnCollision(ColliderParentObject2D target)
{
	target;
}
