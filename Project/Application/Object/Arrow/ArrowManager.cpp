#include "ArrowManager.h"
#include "Arrow.h"
#include "../../../Engine/2D/ImguiManager.h"


void ArrowManager::Initialize(Model* model)
{

	LargeNumberOfObjects::Initialize(model);

	addTransform_ = { 1.0f,1.0f,1.0f, 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };

	addNameNumber_ = 0;

}

void ArrowManager::Update()
{

	LargeNumberOfObjects::Update();

}

void ArrowManager::ImGuiDraw()
{

	ImGui::Begin("ArrowManager");

	std::string title = "";

	ImGui::SeparatorText("Add");
	title = "Add.Number";
	ImGui::DragInt(title.c_str(), &addNameNumber_,0.1f, 0);
	title = "Add.scale";
	ImGui::DragFloat3(title.c_str(), &addTransform_.scale.x, 0.01f, 0.01f);
	title = "Add.rotate";
	ImGui::DragFloat3(title.c_str(), &addTransform_.rotate.x, 0.01f);
	title = "Add.translate";
	ImGui::DragFloat3(title.c_str(), &addTransform_.translate.x, 0.01f);

	if (ImGui::Button("Add")) {
		std::string addName = "Arrow" + addNameNumber_;
		Register(addTransform_, addName);
	}

	std::list<std::unique_ptr<OneOfManyObjects>>::iterator itr = objects_.begin();
	for (; itr != objects_.end(); ++itr) {
		Arrow* obj = static_cast<Arrow*>(itr->get());
		obj->ImGuiDraw();
	}

	ImGui::End();

}

void ArrowManager::Register(const EulerTransform& transform, const std::string& name)
{

	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Arrow>();
	// 初期化
	static_cast<Arrow*>(obj.get())->Initialize(transform, name);
	// 追加
	objects_.push_back(std::move(obj));

}
