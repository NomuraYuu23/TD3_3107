#include "GameBasicCamera.h"
#include "../../../Engine/2D/ImguiManager.h"

void GameBasicCamera::Initialize()
{
	BaseCamera::Initialize();

	transform_.translate.z = -85.0f;

}

void GameBasicCamera::Update()
{
	// 更新
	BaseCamera::Update();

}

void GameBasicCamera::ImGuiDraw()
{

	ImGui::Begin("GameCamera");

	ImGui::DragFloat3("Position", &transform_.translate.x, 0.01f, -200.0f, 200.0f);

	ImGui::End();

}
