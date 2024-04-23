#include "IAttackObject.h"

uint32_t IAttackObject::sSerialNumber_;

void IAttackObject::Initialize(Model* model)
{
	// モデル
	model_ = model;

	// マテリアル
	material_.reset(Material::Create());

	// 行列
	worldtransform_.Initialize(model_->GetRootNode());
	worldtransform_.UpdateMatrix();

	// 2D用座標・サイズ
	position2D_ = { worldtransform_.transform_.translate.x,worldtransform_.transform_.translate.y };
	scale2D_ = { worldtransform_.transform_.scale.x * 2.0f, worldtransform_.transform_.scale.y * 2.0f };
	
	// 型番号
	serialNum_ = sSerialNumber_;
	sSerialNumber_++;

	// ローカル行列マネージャー
	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());
	localMatrixManager_->Map();

}

void IAttackObject::Update()
{
	// 行列・座標更新
	worldtransform_.UpdateMatrix();
	position2D_ = { worldtransform_.GetWorldPosition().x,worldtransform_.GetWorldPosition().y };
}
