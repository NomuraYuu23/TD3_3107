#include "MapEditor.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../externals/nlohmann/json.hpp"
#include <fstream>
#include <cassert>

//名前空間
using namespace nlohmann;

void MapEditor::ImGuiDraw()
{

	const float imGuiSpeed = 0.1f;

	ImGui::Begin("BlockEdit");

	if (ImGui::Button("Save")) {
		SaveFile("Stage");
	}

	// ステージの追加
	ImGui::SeparatorText("StageAdd");

	ImGui::DragInt("StageAddNum", &stageAddNum_, 0.1f, 0);

	if(ImGui::Button("StageAdd")) {
		std::string stageName = "Stage" + std::to_string(stageAddNum_);
		datas_[stageName];
		maxStages_++;
		stageAddNum_++;
	}

	// ステージの削除
	ImGui::SeparatorText("StageDelete");

	ImGui::DragInt("StageDeleteNum", &stageDeleteNum_, 0.1f, 0);

	if (ImGui::Button("StageDelete")) {
		// キー
		std::string key = "Stage" + std::to_string(stageDeleteNum_);

		// 指定グループに指定キーが存在するか
		if (datas_.find(key) != datas_.end()) {
			// 指定グループから指定のキーの値を取得
			datas_.erase(key);
			maxStages_--;
		}

	}


	// ステージの数だけ回す
	uint32_t stageCount = 0;

	for (std::map<std::string, Group>::iterator datasItr = datas_.begin();
		datasItr != datas_.end(); ++datasItr) {

		std::string stageName = datasItr->first;
		Group& group = datasItr->second;
		ImGui::SeparatorText(stageName.c_str());

		// ブロックの追加
		ImGui::SeparatorText("TerrainAdd");

		ImGui::DragFloat2("AddPosition", &addMapBlockData_.position.x, imGuiSpeed);
		ImGui::DragFloat2("AddSize", &addMapBlockData_.size.x, imGuiSpeed);
		ImGui::DragInt("AddTerrainNum", &addMapBlockNum_, 0.1f, 0);

		if (addMapBlockNum_ < 0) {
			addMapBlockNum_ = 0;
		}

		std::string nameTerrainAdd = "TerrainAdd" + std::to_string(stageCount);

		if (ImGui::Button(nameTerrainAdd.c_str())) {
			// キー
			std::string key = "Terrain" + std::to_string(addMapBlockNum_);
			// 追加
			SetValue(stageName, key, addMapBlockData_);
			addMapBlockNum_++;
		}


		// ブロックの削除
		ImGui::SeparatorText("TerrainDelete");

		ImGui::DragInt("DeleteTerrainNum", &deleteMapBlockNum_, 0.1f, 0);

		std::string nameTerrainDelete = "TerrainDelete" + std::to_string(stageCount);
		
		if (ImGui::Button(nameTerrainDelete.c_str())) {
			// キー
			std::string key = "Terrain" + std::to_string(deleteMapBlockNum_);

			// 指定グループに指定キーが存在するか
			if (datasItr->second.find(key) != datasItr->second.end()) {
				// 指定グループから指定のキーの値を取得
				datasItr->second.erase(key);
			}

		}

		// ブロックの値の修正
		ImGui::SeparatorText("TerrainEdit");

		for (std::map<std::string, Item>::iterator groupItr = group.begin();
			groupItr != group.end(); ++groupItr) {

			Item& item = groupItr->second;

			std::string name = groupItr->first;
			ImGui::SeparatorText(name.c_str());

			std::string namePosition = stageName + name + "Position";
			std::string nameSize = stageName + name + "Size";

			ImGui::DragFloat2(namePosition.c_str(), &item.position.x, imGuiSpeed);
			ImGui::DragFloat2(nameSize.c_str(), &item.size.x, imGuiSpeed);

		}

		stageCount++;

	}

	ImGui::End();

}

void MapEditor::LoadFiles()
{

	datas_.clear();

	std::string saveDirectryPath = kDirectoryPath;
	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(saveDirectryPath)) {
		return;
	}
	std::filesystem::directory_iterator dir_it(saveDirectryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		LoadFile(filePath.stem().string());

	}

}

void MapEditor::LoadFile(const std::string& groupName)
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

	// マップ数
	maxStages_ = 0;

	// ファイル読み込み
	while (1) {

		// グループを検索
		std::string name = groupName + std::to_string(maxStages_);

		json::iterator itGroup = root.find(name);

		// 未登録チェック
		if (itGroup == root.end()) {
			break;
		}
		else {
			maxStages_++;
		}

		// 各アイテムについて
		for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
			// アイテム名を取得
			const std::string& itemName = itItem.key();

			MapBlockData value = *itItem;
			SetValue(name, itemName, value);

		}
	}

}

void MapEditor::SetValue(const std::string& groupName, const std::string& key, MapBlockData value)
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

MapBlockData MapEditor::GetValue(const std::string& groupName, const std::string& key)
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

void MapEditor::SaveData(const std::string& groupName)
{

}

void MapEditor::SaveFile(const std::string& groupName)
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

			MapBlockData values = item;
			root[name][itemName] = values;
;
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
