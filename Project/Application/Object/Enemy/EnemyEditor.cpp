#include "EnemyEditor.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../externals/nlohmann/json.hpp"
#include <fstream>
#include <cassert>

//名前空間
using namespace nlohmann;

void EnemyEditor::ImGuiDraw()
{

	const float imGuiSpeed = 0.1f;

	ImGui::Begin("EnemyEditor");

	if (ImGui::Button("Save")) {
		SaveFile("Stage");
	}

	// ステージの追加
	ImGui::SeparatorText("StageAdd");

	ImGui::DragInt("StageAddNum", &stageAddNum_, 0.1f, 0);

	if (ImGui::Button("StageAdd")) {
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

	ImGui::RadioButton("SingleEnemy", &mode_, 0); ImGui::SameLine(); ImGui::RadioButton("MultiEnemy", &mode_, 1);

	if (mode_ == 0) {
		ImGuiSingleEnemy();
	}
	else {
		ImGuiMultiEnemy();
	}

	ImGui::End();

}

void EnemyEditor::LoadFiles()
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

void EnemyEditor::LoadFile(const std::string& groupName)
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

			
			// SingleEnemyData型の値を保持していれば
			if (itItem->contains("position") && itItem->find("position")->is_array() &&
				itItem->contains("typeNum") && itItem->find("typeNum")->is_array()) {
				// SingleEnemyData型の値を登録
				SingleEnemyData value = itItem->get<SingleEnemyData>();
				SetValue(groupName, itemName, value);
			}
			// MultiEnemyData型の値を保持していれば
			else if(itItem->contains("position") && itItem->find("position")->is_array() &&
				itItem->contains("distance") && itItem->find("distance")->is_array() &&
				itItem->contains("enemyMaxCount") && itItem->find("enemyMaxCount")->is_array()){
				// MultiEnemyData型の値を登録
				MultiEnemyData value = itItem->get<MultiEnemyData>();
				SetValue(groupName, itemName, value);
			}

		}
	}

}

void EnemyEditor::SetValue(const std::string& groupName, const std::string& key, SingleEnemyData value)
{

	// グループの参照を取得
	EnemyEditorGroup& group = datas_[groupName];
	if (group.find(key) != group.end()) {
		return;
	}
	// 新しい項目のデータを設定
	EnemyEditorItem newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

void EnemyEditor::SetValue(const std::string& groupName, const std::string& key, MultiEnemyData value)
{

	// グループの参照を取得
	EnemyEditorGroup& group = datas_[groupName];
	if (group.find(key) != group.end()) {
		return;
	}
	// 新しい項目のデータを設定
	EnemyEditorItem newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

SingleEnemyData EnemyEditor::GetSingleEnemyDataValue(const std::string& groupName, const std::string& key)
{

	// 指定グループが存在するか
	assert(datas_.find(groupName) != datas_.end());
	//  グループの参照を取得
	EnemyEditorGroup& group = datas_[groupName];
	// 指定グループに指定キーが存在するか
	assert(group.find(key) != group.end());
	// 指定グループから指定のキーの値を取得
	return std::get<SingleEnemyData>(group[key]);

}

MultiEnemyData EnemyEditor::GetMultiEnemyDataValue(const std::string& groupName, const std::string& key)
{

	// 指定グループが存在するか
	assert(datas_.find(groupName) != datas_.end());
	//  グループの参照を取得
	EnemyEditorGroup& group = datas_[groupName];
	// 指定グループに指定キーが存在するか
	assert(group.find(key) != group.end());
	// 指定グループから指定のキーの値を取得
	return std::get<MultiEnemyData>(group[key]);

}

void EnemyEditor::SaveData(const std::string& groupName)
{
}

void EnemyEditor::SaveFile(const std::string& groupName)
{

	json root;
	root = json::object();

	for (std::map<std::string, EnemyEditorGroup>::iterator datasItr = datas_.begin();
		datasItr != datas_.end(); ++datasItr) {

		std::string name = datasItr->first;

		// jsonオブジェクト登録
		root[name] = json::object();

		// 各項目について
		for (std::map<std::string, EnemyEditorItem>::iterator itItem = datasItr->second.begin();
			itItem != datasItr->second.end(); ++itItem) {

			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			EnemyEditorItem& item = itItem->second;

			if (std::holds_alternative<SingleEnemyData>(item)) {
				SingleEnemyData values = std::get<SingleEnemyData>(item);
				root[name][itemName] = values;
			}
			else if(std::holds_alternative<MultiEnemyData>(item)){
				MultiEnemyData values = std::get<MultiEnemyData>(item);
				root[name][itemName] = values;
			}

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

void EnemyEditor::ImGuiSingleEnemy()
{

	const float imGuiSpeed = 0.1f;

	// ステージの数だけ回す
	uint32_t stageCount = 0;

	for (std::map<std::string, EnemyEditorGroup>::iterator datasItr = datas_.begin();
		datasItr != datas_.end(); ++datasItr) {

		std::string stageName = datasItr->first;
		EnemyEditorGroup& group = datasItr->second;
		ImGui::SeparatorText(stageName.c_str());

		// シングルエネミーの追加
		ImGui::SeparatorText("SingleEnemyAdd");

		ImGui::DragFloat3("AddPosition", &addSingleEnemyData_.position.x, imGuiSpeed);
		ImGui::DragInt("AddTypeNum", &addSingleEnemyData_.typeNum, imGuiSpeed, 0);
		ImGui::DragInt("AddSingleEnemyNum", &addSingleEnemyNum_, 0.1f, 0);

		if (addSingleEnemyNum_ < 0) {
			addSingleEnemyNum_ = 0;
		}

		std::string nameTerrainAdd = "SingleEnemyAdd" + std::to_string(stageCount);

		if (ImGui::Button(nameTerrainAdd.c_str())) {
			// キー
			std::string key = "SingleEnemy" + std::to_string(addSingleEnemyNum_);
			// 追加
			SetValue(stageName, key, addSingleEnemyData_);
			addSingleEnemyNum_++;
		}

		// シングルエネミーの削除
		ImGui::SeparatorText("SingleEnemyDelete");

		ImGui::DragInt("DeleteSingleEnemyNum", &deleteSingleEnemyNum_, 0.1f, 0);

		if (deleteSingleEnemyNum_ < 0) {
			deleteSingleEnemyNum_ = 0;
		}

		std::string nameSingleDelete = "SingleEnemyDelete" + std::to_string(stageCount);

		if (ImGui::Button(nameSingleDelete.c_str())) {
			// キー
			std::string key = "SingleEnemy" + std::to_string(deleteSingleEnemyNum_);

			// 指定グループに指定キーが存在するか
			if (datasItr->second.find(key) != datasItr->second.end()) {
				// 指定グループから指定のキーの値を取得
				datasItr->second.erase(key);
			}

		}

		// シングルエネミーの値の修正
		ImGui::SeparatorText("SingleEnemyEdit");

		for (std::map<std::string, EnemyEditorItem>::iterator groupItr = group.begin();
			groupItr != group.end(); ++groupItr) {

			if (std::holds_alternative<SingleEnemyData>(groupItr->second)) {

				SingleEnemyData& item = std::get<SingleEnemyData>(groupItr->second);

				std::string name = groupItr->first;
				ImGui::SeparatorText(name.c_str());

				std::string namePosition = stageName + name + "Position";
				std::string nameTypeNum = stageName + name + "TypeNum";

				ImGui::DragFloat3(namePosition.c_str(), &item.position.x, imGuiSpeed);
				ImGui::DragInt(nameTypeNum.c_str(), &item.typeNum, imGuiSpeed);

			}

		}

		stageCount++;

	}

}

void EnemyEditor::ImGuiMultiEnemy()
{

	const float imGuiSpeed = 0.1f;

	// ステージの数だけ回す
	uint32_t stageCount = 0;

	for (std::map<std::string, EnemyEditorGroup>::iterator datasItr = datas_.begin();
		datasItr != datas_.end(); ++datasItr) {

		std::string stageName = datasItr->first;
		EnemyEditorGroup& group = datasItr->second;
		ImGui::SeparatorText(stageName.c_str());

		// マルチエネミーの追加
		ImGui::SeparatorText("MultiEnemyAdd");

		ImGui::DragFloat3("AddPosition", &addMultieEnemyData_.position.x, imGuiSpeed);
		ImGui::DragFloat("AddDistance", &addMultieEnemyData_.distance, imGuiSpeed, 0);
		ImGui::DragInt("AddEnemyMaxCount", &addMultieEnemyData_.enemyMaxCount, imGuiSpeed, 0);
		ImGui::DragInt("AddMultiEnemyNum", &addMultiEnemyNum_, 0.1f, 0);

		if (addMultiEnemyNum_ < 0) {
			addMultiEnemyNum_ = 0;
		}

		std::string nameTerrainAdd = "MultiEnemyAdd" + std::to_string(stageCount);

		if (ImGui::Button(nameTerrainAdd.c_str())) {
			// キー
			std::string key = "MultiEnemy" + std::to_string(addMultiEnemyNum_);
			// 追加
			SetValue(stageName, key, addSingleEnemyData_);
			addMultiEnemyNum_++;
		}

		// マルチエネミーの削除
		ImGui::SeparatorText("MultiEnemyDelete");

		ImGui::DragInt("DeleteMultiEnemyNum", &deleteMultiEnemyNum_, 0.1f, 0);
		if (deleteMultiEnemyNum_ < 0) {
			deleteMultiEnemyNum_ = 0;
		}

		std::string nameSingleDelete = "MultiEnemyDelete" + std::to_string(stageCount);

		if (ImGui::Button(nameSingleDelete.c_str())) {
			// キー
			std::string key = "MultiEnemy" + std::to_string(deleteMultiEnemyNum_);

			// 指定グループに指定キーが存在するか
			if (datasItr->second.find(key) != datasItr->second.end()) {
				// 指定グループから指定のキーの値を取得
				datasItr->second.erase(key);
			}

		}

		// マルチエネミーの値の修正
		ImGui::SeparatorText("MultiEnemyEdit");

		for (std::map<std::string, EnemyEditorItem>::iterator groupItr = group.begin();
			groupItr != group.end(); ++groupItr) {

			if (std::holds_alternative<MultiEnemyData>(groupItr->second)) {

				MultiEnemyData& item = std::get<MultiEnemyData>(groupItr->second);

				std::string name = groupItr->first;
				ImGui::SeparatorText(name.c_str());

				std::string namePosition = stageName + name + "Position";
				std::string nameDistance = stageName + name + "Distance";
				std::string nameEnemyMaxCount = stageName + name + "EnemyMaxCount";

				ImGui::DragFloat3(namePosition.c_str(), &item.position.x, imGuiSpeed);
				ImGui::DragFloat(nameDistance.c_str(), &addMultieEnemyData_.distance, imGuiSpeed, 0);
				ImGui::DragInt(nameEnemyMaxCount.c_str(), &addMultieEnemyData_.enemyMaxCount, imGuiSpeed, 0);

			}

		}

		stageCount++;

	}

}
