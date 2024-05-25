#include "BackGround.h"
#include "../../../Engine/3D/ModelDraw.h"
#include "../../imgui/imgui.h"

void BackGround::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);

	// トランスフォーム初期化
	worldtransform_.transform_.translate = { 150.0f, -0.0f, 300.0f };
	worldtransform_.transform_.scale = { 3.0f, 3.0f, 3.0f };

	// トランスフォーム初期化
	transform2_.Initialize(model_->GetRootNode());
	transform2_.transform_.translate = { 450.0f, -0.0f, 300.0f };
	transform2_.transform_.scale = { 3.0f, 3.0f, 3.0f };
	transform2_.UpdateMatrix();

	// ライティング有効
	enableLighting_ = EnableLighting::HalfLambert;
	material_->SetEnableLighting(enableLighting_);
}

void BackGround::Update()
{
	// 親がいる場合
	if (parent_) {
		/*worldtransform_.transform_.translate.x = parent_->GetWorldPosition().x + 50.0f;
		worldtransform_.transform_.translate.y = parent_->GetWorldPosition().y;*/
	}

	// 基底クラスの更新
	IObject::Update();
}

void BackGround::Draw(const BaseCamera& camera)
{
	ModelDraw::AnimObjectDesc desc;
	desc.camera = &const_cast<BaseCamera&>(camera);
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldtransform_;
	ModelDraw::AnimObjectDraw(desc);

	desc.worldTransform = &transform2_;
	ModelDraw::AnimObjectDraw(desc);
}

void BackGround::ImGuiDraw()
{
	ImGui::Begin("BackGround");
	ImGui::DragFloat3("transform", &worldtransform_.transform_.translate.x);
	ImGui::End();
}

void BackGround::OnCollision(ColliderParentObject2D target)
{
}

Box BackGround::GetBoxCollider()
{
	return Box();
}

Vector2 BackGround::GetColliderPosition()
{
	return Vector2();
}

Vector2 BackGround::GetColliderSize()
{
	return Vector2();
}
