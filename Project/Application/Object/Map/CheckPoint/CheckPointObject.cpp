#include "CheckPointObject.h"
#include "../../ObjectList.h"
#include "../../../Collider2D/CollisionConfig2D.h"

void CheckPointObject::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);
	// コライダー用の座標・スケール
	position2D_ = { worldtransform_.transform_.translate.x,worldtransform_.transform_.translate.y };
	scale2D_ = { worldtransform_.transform_.scale.x, worldtransform_.transform_.scale.y };
	scale2D_ = { 2.0f,2.0f };
	// コライダーの初期化
	//boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, 0.0f, this);
	//boxCollider_.SetCollisionAttribute(kCollisionAttributeGoalObject);
	//boxCollider_.SetCollisionMask(kCollisionAttributeEnemy);
}

void CheckPointObject::Update()
{
	// 基底クラスの更新
	IObject::Update();
	// コライダー
	BoxColliderUpdate();
}

void CheckPointObject::Draw(const BaseCamera& camera)
{
	ModelDraw::AnimObjectDesc desc;
	desc.camera = &const_cast<BaseCamera&>(camera);
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldtransform_;
	ModelDraw::AnimObjectDraw(desc);
}

void CheckPointObject::ImGuiDraw()
{
}

void CheckPointObject::OnCollision(ColliderParentObject2D target)
{
	target;
}
