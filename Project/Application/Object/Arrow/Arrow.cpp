#include "Arrow.h"
#include "../../../Engine/2D/ImguiManager.h"

void Arrow::Initialize(
	const EulerTransform& transform,
	const std::string& name)
{

	OneOfManyObjects::Initialize();

	transform_ = transform;

	name_ = name;

	Update();

}

void Arrow::ImGuiDraw()
{

	std::string title = "";

	ImGui::SeparatorText(name_.c_str());
	title = name_ + ".scale";
	ImGui::DragFloat3(title.c_str(), &transform_.scale.x, 0.01f, 0.01f);
	title = name_ + ".rotate";
	ImGui::DragFloat3(title.c_str(), &transform_.rotate.x, 0.01f);
	title = name_ + ".translate";
	ImGui::DragFloat3(title.c_str(), &transform_.translate.x, 0.01f);
	
	ImGui::Separator();
	if (ImGui::Button("Delete")) {
		isDead_ = true;
	}

}
