#include "CheckPointEditor.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../externals/nlohmann/json.hpp"
#include <fstream>
#include <cassert>
#include "../../../AllSceneObject/StageNumberManager.h"

//名前空間
using namespace nlohmann;

void CheckPointEditor::ImGuiDraw()
{

	const float imGuiSpeed = 0.1f;

	ImGui::Begin("CheckPointEditor");

	if (ImGui::Button("Save")) {
		SaveFile("CheckPoint");
	}

	// ステージの数だけ回す
	uint32_t stageCount = 0;

	if (ImGui::BeginTabBar("StageNum")) {

		for (std::map<std::string, Group>::iterator datasItr = datas_.begin();
			datasItr != datas_.end(); ++datasItr) {

			std::string stageName = "Stage" + std::to_string(stageCount);

			if (stageCount < 10) {
				stageName = "Stage0" + std::to_string(stageCount);
			}
			if (ImGui::BeginTabItem(stageName.c_str())) {

				std::string stageName = datasItr->first;
				Group& group = datasItr->second;

				// ブロックの追加
				ImGui::SeparatorText("CheckPointAdd");

				ImGui::DragFloat2("AddCheckPointPosition", &addCheckPointData_.position.x, imGuiSpeed);
				ImGui::DragInt("AddCheckPointNumber", &addCheckPointData_.checkPointNumber, imGuiSpeed, 0);
				ImGui::DragInt("AddCheckPointNum", &addCheckPointNum_, 0.1f, 0);

				if (addCheckPointNum_ < 0) {
					addCheckPointNum_ = 0;
				}

				if (addCheckPointData_.checkPointNumber < 0) {
					addCheckPointData_.checkPointNumber = 0;
				}

				std::string nameCheckPointAdd = "CheckPointAdd" + std::to_string(stageCount);

				if (ImGui::Button(nameCheckPointAdd.c_str())) {
					// キー
					std::string key = "CheckPoint" + std::to_string(addCheckPointNum_);

					if (addCheckPointNum_ < 10) {
						key = "CheckPoint0" + std::to_string(addCheckPointNum_);
					}

					// 追加
					SetValue(stageName, key, addCheckPointData_);
					addCheckPointNum_++;
				}


				// ブロックの削除
				ImGui::SeparatorText("CheckPointDelete");

				ImGui::DragInt("DeleteCheckPointNum", &deleteCheckPointNum_, 0.1f, 0);

				std::string nameTerrainDelete = "CheckPointDelete" + std::to_string(stageCount);

				if (ImGui::Button(nameTerrainDelete.c_str())) {
					// キー
					std::string key = "CheckPoint" + std::to_string(deleteCheckPointNum_);

					if (deleteCheckPointNum_ < 10) {
						key = "CheckPoint0" + std::to_string(deleteCheckPointNum_);
					}

					// 指定グループに指定キーが存在するか
					if (datasItr->second.find(key) != datasItr->second.end()) {
						// 指定グループから指定のキーの値を取得
						datasItr->second.erase(key);
					}

				}

				// ブロックの値の修正
				ImGui::SeparatorText("CheckPointEdit");

				for (std::map<std::string, Item>::iterator groupItr = group.begin();
					groupItr != group.end(); ++groupItr) {

					Item& item = groupItr->second;

					std::string name = groupItr->first;
					ImGui::SeparatorText(name.c_str());

					std::string namePosition = stageName + name + "Position";
					std::string nameCheckPointNumber = stageName + name + "CheckPointNumber";

					ImGui::DragFloat2(namePosition.c_str(), &item.position.x, imGuiSpeed);
					ImGui::DragInt(nameCheckPointNumber.c_str(), &item.checkPointNumber, imGuiSpeed);

				}

				ImGui::EndTabItem();

			}

			stageCount++;

		}

		// タブバーを終了
		ImGui::EndTabBar();

	}

	ImGui::End();

}

void CheckPointEditor::LoadFiles()
{

	datas_.clear();

	std::string saveDirectryPath = kDirectoryPath;
	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(saveDirectryPath)) {
		return;
	}
	LoadFile("CheckPoint");

}

void CheckPointEditor::LoadFile(const std::string& groupName)
{

	// 読み込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に開く
	ifs.open(filePath);
	// ファイルオープン失敗
	if (!std::filesystem::exists(filePath)) {
		assert(0);
	}
	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// ステージの数だけ回す
	uint32_t stageCount = 0;

	// ファイル読み込み
	while (1) {

		// グループを検索
		std::string name = groupName + std::to_string(stageCount);

		if (stageCount < 10) {
			name = groupName + "0" + std::to_string(stageCount);
		}

		json::iterator itGroup = root.find(name);

		// 未登録チェック
		if (itGroup == root.end()) {
			datas_[name];
		}
		else {
			// 各アイテムについて
			for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
				// アイテム名を取得
				const std::string& itemName = itItem.key();

				CheckPointData value = *itItem;
				SetValue(name, itemName, value);

			}
		}

		// ループを抜ける
		if (++stageCount == StageNumberManager::kStageMax) {
			break;
		}

	}

}

void CheckPointEditor::SetValue(const std::string& groupName, const std::string& key, CheckPointData value)
{

	// グループの参照を取得
	Group& group = datas_[groupName];
	if (group.find(key) != group.end()) {
		return;
	}
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

CheckPointData CheckPointEditor::GetValue(const std::string& groupName, const std::string& key)
{

	// 指定グループが存在するか
	assert(datas_.find(groupName) != datas_.end());
	//  グループの参照を取得
	Group& group = datas_[groupName];
	// 指定グループに指定キーが存在するか
	assert(group.find(key) != group.end());
	// 指定グループから指定のキーの値を取得
	return group[key];

}

void CheckPointEditor::SaveFile(const std::string& groupName)
{

	json root;
	root = json::object();

	for (std::map<std::string, Group>::iterator datasItr = datas_.begin();
		datasItr != datas_.end(); ++datasItr) {

		std::string name = datasItr->first;

		// jsonオブジェクト登録
		root[name] = json::object();

		// 各項目について
		for (std::map<std::string, Item>::iterator itItem = datasItr->second.begin();
			itItem != datasItr->second.end(); ++itItem) {

			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			Item& item = itItem->second;

			CheckPointData values = item;
			root[name][itemName] = values;
		}

	}

	// ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::filesystem::create_directories(kDirectoryPath);
	}
	// 書き込むJSONファイルのフルパスを合成する
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
