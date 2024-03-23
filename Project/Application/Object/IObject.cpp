#include "IObject.h"

float IObject::sPlaySpeed = 1;

void IObject::Initialize(Model* model)
{
	model_ = model;

	// マテリアル
	enableLighting_ = 0;
	shininess_ = 100.0f;

	material_.reset(Material::Create());
	material_->SetEnableLighting(enableLighting_);
	material_->SetShininess(shininess_);

	// 行列
	worldtransform_.Initialize(model_->GetRootNode());
	worldtransform_.UpdateMatrix();

	// 2D用座標・サイズ
	position2D_ = { worldtransform_.transform_.translate.x,worldtransform_.transform_.translate.y };
	scale2D_ = { worldtransform_.transform_.scale.x * 2.0f, worldtransform_.transform_.scale.y * 2.0f };

}

void IObject::Update()
{
	// 行列・座標更新
	worldtransform_.UpdateMatrix();
	position2D_ = { worldtransform_.GetWorldPosition().x,worldtransform_.GetWorldPosition().y };
}
