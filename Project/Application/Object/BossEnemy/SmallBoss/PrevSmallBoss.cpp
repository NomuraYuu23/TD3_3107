#include "PrevSmallBoss.h"

void PrevSmallBoss::Initialize(Model* model)
{

	IBoss::Initialize(model);
	worldtransform_.transform_.translate = { 50.0f,20.0f,0 };
}

void PrevSmallBoss::Update()
{
	// 状態の更新
	if (state_) {
		state_->Update();
	}

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
