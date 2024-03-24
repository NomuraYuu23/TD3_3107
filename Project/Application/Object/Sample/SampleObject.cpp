#include "SampleObject.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/Physics/InertiaTensor.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/Input/Input.h"
//#include ""

SampleObject::~SampleObject()
{

}

void SampleObject::Initialize(Model* model)
{

	model_ = model;

	material_.reset(Material::Create());

	worldtransform_.Initialize(model_->GetRootNode());

	// 初期ローカル座標
	std::vector<Vector3> initPositions;
	initPositions.resize(worldtransform_.GetNodeDatas().size());
	for (uint32_t i = 0; i < initPositions.size(); ++i) {
		initPositions[i] = { 0.0f, 0.0f, 0.0f };
	}
	//initPositions[2] = { 20.0f, 0.0f, 0.0f };

	std::vector<Quaternion> initRotations;
	initRotations.resize(worldtransform_.GetNodeDatas().size());
	for (uint32_t i = 0; i < initRotations.size(); ++i) {
		initRotations[i] = { 0.0f, 0.0f, 0.0f, 1.0f };
	}

	std::vector<Vector3> initScalings;
	initScalings.resize(worldtransform_.GetNodeDatas().size());
	for (uint32_t i = 0; i < initScalings.size(); ++i) {
		initScalings[i] = { 1.0f, 1.0f, 1.0f };
	}

	animation_.Initialize(
		model_->GetNodeAnimationData(),
		initPositions,
		initRotations,
		initScalings,
		worldtransform_.GetNodeNames());

	animation_.startAnimation(0, true);
	animation_.startAnimation(1, true);

	enableLighting_ = 0;

	shininess_ = 100.0f;

	RegisteringGlobalVariables();

	ApplyGlobalVariables();

	const Vector3 centerOfGravity = {0.0f,0.0f,0.0f};
	const Vector3 pointOfAction = { 0.0f,1.0f,0.0f };
	const Vector3 force = { 1.0f,0.0f,1.0f };;

	//rigidBody_.torque = RigidBody::TorqueCalc(centerOfGravity, pointOfAction, force);

	//rigidBody_.inertiaTensor  = InertiaTensor::CreateRectangular(10.0f, Vector3{ 2.0f, 2.0f, 2.0f}); // 慣性テンソル
	//rigidBody_.basicPostureInertiaTensor = InertiaTensor::CreateRectangular(10.0f, Vector3{ 2.0f, 2.0f, 2.0f }); // 基本姿勢での慣性テンソル

	//rigidBody_.postureMatrix = Matrix4x4::MakeRotateXYZMatrix({0.0f,0.0f,0.0f}); // 姿勢行列

	//rigidBody_.angularVelocity = { 0.0f,0.0f,0.0f }; // 角速度
	////Quaternion angularAcceleration; // 角加速度
	//rigidBody_.angularMomentum = { 0.0f,0.0f,0.0f }; // 角運動量

}

void SampleObject::Update()
{

	ApplyGlobalVariables();

	worldtransform_.SetNodeLocalMatrix(animation_.AnimationUpdate());

	//rigidBody_.postureMatrix =  RigidBody::PostureCalc(rigidBody_.postureMatrix, rigidBody_.angularVelocity, kDeltaTime_);

	//rigidBody_.inertiaTensor = RigidBody::InertiaTensorCalc(rigidBody_.postureMatrix, rigidBody_.basicPostureInertiaTensor);

	//rigidBody_.angularMomentum = RigidBody::AngularMomentumCalc(rigidBody_.angularMomentum, rigidBody_.torque, kDeltaTime_);

	//rigidBody_.angularVelocity = RigidBody::AngularVelocityCalc(rigidBody_.inertiaTensor, rigidBody_.angularMomentum);

	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		animation_.stopAnimation(1);
	}

	rigidBody_.torque = { 0.0f,0.0f,0.0f };

	worldtransform_.UpdateMatrix();

	material_->SetEnableLighting(enableLighting_);
	material_->SetShininess(shininess_);

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
