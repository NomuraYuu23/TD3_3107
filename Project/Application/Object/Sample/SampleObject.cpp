#include "SampleObject.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Input/Input.h"
#include "../../Collider2D/CollisionConfig2D.h"

SampleObject::~SampleObject()
{

}

void SampleObject::Initialize(Model* model)
{

	model_ = model;

	material_.reset(Material::Create());

	worldtransform_.Initialize(model_->GetRootNode());

	enableLighting_ = 0;

	shininess_ = 100.0f;

	// 2D用座標・サイズ
	position2D_ = { worldtransform_.transform_.translate.x,worldtransform_.transform_.translate.y };
	scale2D_ = { worldtransform_.transform_.scale.x * 2.0f, worldtransform_.transform_.scale.y * 2.0f };

	// コライダーの初期化
	Box::Initialize(position2D_, scale2D_.x, scale2D_.y, this);

	this->SetCollisionAttribute(kCollisionAttributeEnemy);
	this->SetCollisionMask(kCollisionAttributePlayer);

	RegisteringGlobalVariables();

	ApplyGlobalVariables();

}

void SampleObject::Update()
{

	//ApplyGlobalVariables();


	Input* input = Input::GetInstance();
	float tmpSpeed = 0.05f;
	if (input->PushKey(DIK_LEFT)) {
		worldtransform_.transform_.translate.x -= tmpSpeed;
	}
	else if (input->PushKey(DIK_RIGHT)) {
		worldtransform_.transform_.translate.x += tmpSpeed;
	}
	worldtransform_.UpdateMatrix();
	position2D_ = { worldtransform_.transform_.translate.x,worldtransform_.transform_.translate.y };

	material_->SetEnableLighting(enableLighting_);
	material_->SetShininess(shininess_);

	Box::Update(position2D_, scale2D_.x, scale2D_.y);
}

void SampleObject::Draw(BaseCamera camera)
{

	model_->Draw(worldtransform_, camera, material_.get());

}

void SampleObject::ImGuiDraw()
{

	ImGui::Begin("SampleObject");

	ImGui::Text("enableLighting");
	ImGui::RadioButton("None", &enableLighting_, EnableLighting::None);
	ImGui::SameLine();
	ImGui::RadioButton("Lambert", &enableLighting_, EnableLighting::Lambert);
	ImGui::SameLine();
	ImGui::RadioButton("HalfLambert", &enableLighting_, EnableLighting::HalfLambert);
	ImGui::SameLine();
	ImGui::RadioButton("PhongReflection", &enableLighting_, EnableLighting::PhongReflection);
	ImGui::SameLine();
	ImGui::RadioButton("BlinnPhongReflection", &enableLighting_, EnableLighting::BlinnPhongReflection);

	ImGui::DragFloat("shininess", &shininess_);
	float absValue = 30.0f;
	ImGui::DragFloat3("translate", &worldtransform_.transform_.translate.x, 0.01f, -absValue, absValue);

	ImGui::End();

}

void SampleObject::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// グループ名
	const std::string groupName = "SampleObject";

	// スケール
	globalVariables->AddItem(groupName, "Scale", worldtransform_.transform_.scale);
	// 回転
	globalVariables->AddItem(groupName, "Rotate", worldtransform_.transform_.rotate);
	// 位置
	globalVariables->AddItem(groupName, "Transform", worldtransform_.transform_.translate);

}

void SampleObject::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// グループ名
	const std::string groupName = "SampleObject";

	// スケール
	worldtransform_.transform_.scale = globalVariables->GetVector3Value(groupName, "Scale");
	// 回転
	worldtransform_.transform_.rotate = globalVariables->GetVector3Value(groupName, "Rotate");
	// 位置
	worldtransform_.transform_.translate = globalVariables->GetVector3Value(groupName, "Transform");

}
