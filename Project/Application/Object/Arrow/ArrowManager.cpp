#include "ArrowManager.h"
#include "Arrow.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../externals/nlohmann/json.hpp"
#include <fstream>
#include "../../AllSceneObject/StageNumberManager.h"

//名前空間
using namespace nlohmann;

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

	ImGui::SeparatorText("Save");

	if (ImGui::Button("Save")) {
		Save();
	}

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
		std::string addName = "Arrow" + std::to_string(addNameNumber_);
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

void ArrowManager::Save()
{

	json root;
	root = json::object();

	// jsonオブジェクト登録
	root["Arrow"] = json::object();

	for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator itr = objects_.begin();
		itr != objects_.end(); ++itr) {

		Arrow* obj = static_cast<Arrow*>(itr->get());

		std::string name = obj->GetName();

		root["Arrow"][name] = obj->transform_;

	}

	// ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::filesystem::create_directories(kDirectoryPath);
	}
	// 書き込むJSONファイルのフルパスを合成する

	std::string groupName = "Arrow_" + StageNumberManager::stageNum_;

	std::string filePath = kDirectoryPath + groupName + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込み用に開く
	ofs.open(filePath);

	// ファイルオープン失敗？
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}
	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();

}
