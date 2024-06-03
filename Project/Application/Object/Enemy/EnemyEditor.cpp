#include "EnemyEditor.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../externals/nlohmann/json.hpp"
#include <fstream>
#include <cassert>
#include "../../AllSceneObject/StageNumberManager.h"

//名前空間
using namespace nlohmann;

void EnemyEditor::ImGuiDraw()
{

	const float imGuiSpeed = 0.1f;

	ImGui::Begin("EnemyEditor");

	if (ImGui::Button("Save")) {
		SaveFile("Stage");
	}

	ImGui::RadioButton("SingleEnemy", &mode_, 0); ImGui::SameLine(); ImGui::RadioButton("MultiEnemy", &mode_, 1); ImGui::RadioButton("ChaseEnemy", &mode_, 2);

	if (mode_ == 0) {
		ImGuiSingleEnemy();
	}
	else if(mode_ == 1){
		ImGuiMultiEnemy();
	}
	else {
		ImGuiChaseEnemy();
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
				size_t tmp = itItem->size();
				// SingleEnemyData型の値を保持していれば
				if (itItem->is_object() && itItem->size() == 4) {
					// SingleEnemyData型の値を登録
					SingleEnemyData value = itItem->get<SingleEnemyData>();
					SetValue(name, itemName, value);
				}
				// MultiEnemyData型の値を保持していれば
				else if(itItem->is_object() && itItem->size() == 5) {
					// MultiEnemyData型の値を登録
					MultiEnemyData value = itItem->get<MultiEnemyData>();
					SetValue(name, itemName, value);
				}
				// ChaseEnemyData型の値を保持していれば
				else if (itItem->is_object() && itItem->size() == 2) {
					// ChaseEnemyData型の値を登録
					ChaseEnemyData value = itItem->get<ChaseEnemyData>();
					SetValue(name, itemName, value);
				}

			}
		}

		// ループを抜ける
		if (++stageCount == StageNumberManager::kStageMax) {
			break;
		}


	}

}

void EnemyEditor::SetValue(const std::string& groupName, const std::string& key, SingleEnemyData value)
{

	// グループの参照を取得
	EnemyEditorGroup& group = datas_[groupName];
	//if (group.find(key) != group.end()) {
	//	return;
	//}
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
	//if (group.find(key) != group.end()) {
	//	return;
	//}
	// 新しい項目のデータを設定
	EnemyEditorItem newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

void EnemyEditor::SetValue(const std::string& groupName, const std::string& key, ChaseEnemyData value)
{
	// グループの参照を取得
	EnemyEditorGroup& group = datas_[groupName];
	//if (group.find(key) != group.end()) {
	//	return;
	//}
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

ChaseEnemyData EnemyEditor::GetChaseEnemyDataValue(const std::string& groupName, const std::string& key)
{
	// 指定グループが存在するか
	assert(datas_.find(groupName) != datas_.end());
	//  グループの参照を取得
	EnemyEditorGroup& group = datas_[groupName];
	// 指定グループに指定キーが存在するか
	assert(group.find(key) != group.end());
	// 指定グループから指定のキーの値を取得
	return std::get<ChaseEnemyData>(group[key]);
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
			else if (std::holds_alternative<ChaseEnemyData>(item)) {
				ChaseEnemyData values = std::get<ChaseEnemyData>(item);
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

	if (ImGui::BeginTabBar("StageNum")) {

		for (std::map<std::string, EnemyEditorGroup>::iterator datasItr = datas_.begin();
			datasItr != datas_.end(); ++datasItr) {

			std::string stageName = "Stage" + std::to_string(stageCount);

			if (stageCount < 10) {
				stageName = "Stage0" + std::to_string(stageCount);
			}

			if (ImGui::BeginTabItem(stageName.c_str())) {

				std::string stageName = datasItr->first;
				EnemyEditorGroup& group = datasItr->second;

				// シングルエネミーの追加
				ImGui::SeparatorText("SingleEnemyAdd");

				ImGui::DragFloat3("AddPosition", &addSingleEnemyData_.position.x, imGuiSpeed);
				ImGui::DragInt("AddTypeNum", &addSingleEnemyData_.typeNum, imGuiSpeed, 0);
				ImGui::DragInt("AddSingleEnemyNum", &addSingleEnemyNum_, 0.1f, 0);
				ImGui::DragFloat3("AddEaseEndPosition", &addSingleEnemyData_.endPosition.x, imGuiSpeed);
				ImGui::DragFloat("AddEndFrame", &addSingleEnemyData_.endFrame, 0.1f, 0);

				if (addSingleEnemyNum_ < 0) {
					addSingleEnemyNum_ = 0;
				}

				if (addSingleEnemyData_.typeNum < 0) {
					addSingleEnemyData_.typeNum = 0;
				}
				else if (addSingleEnemyData_.typeNum > 1) {
					addSingleEnemyData_.typeNum = 1;
				}

				std::string nameSingleAdd = "SingleEnemyAdd" + std::to_string(stageCount);

				if (ImGui::Button(nameSingleAdd.c_str())) {
					// キー
					std::string key = "Enemy" + std::to_string(addSingleEnemyNum_);

					if (addSingleEnemyNum_ < 10) {
						key = "Enemy0" + std::to_string(addSingleEnemyNum_);
					}

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
					std::string key = "Enemy" + std::to_string(deleteSingleEnemyNum_);

					if (deleteSingleEnemyNum_ < 10) {
						key = "Enemy0" + std::to_string(deleteSingleEnemyNum_);
					}


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
						std::string nameEaseEndPosition = stageName + name + "EndPosition";
						std::string namePad = stageName + name + "EndFrame";

						ImGui::DragFloat3(namePosition.c_str(), &item.position.x, imGuiSpeed);
						ImGui::DragInt(nameTypeNum.c_str(), &item.typeNum, imGuiSpeed);
						ImGui::DragFloat3(nameEaseEndPosition.c_str(), &item.endPosition.x, imGuiSpeed);
						ImGui::DragFloat(namePad.c_str(), &item.endFrame, imGuiSpeed);

						if (item.typeNum < 0) {
							item.typeNum = 0;
						}
						else if (item.typeNum > 1) {
							item.typeNum = 1;
						}

					}

				}

				ImGui::EndTabItem();

			}

			stageCount++;

		}

		// データが入ってない分
		for (; stageCount < StageNumberManager::kStageMax; ++stageCount) {
		
			std::string stageName = "Stage" + std::to_string(stageCount);

			if (stageCount < 10) {
				stageName = "Stage0" + std::to_string(stageCount);
			}

			if (ImGui::BeginTabItem(stageName.c_str())) {

				// シングルエネミーの追加
				ImGui::SeparatorText("SingleEnemyAdd");

				ImGui::DragFloat3("AddPosition", &addSingleEnemyData_.position.x, imGuiSpeed);
				ImGui::DragInt("AddTypeNum", &addSingleEnemyData_.typeNum, imGuiSpeed, 0);
				ImGui::DragInt("AddSingleEnemyNum", &addSingleEnemyNum_, 0.1f, 0);
				ImGui::DragFloat3("AddEaseEndPosition", &addSingleEnemyData_.endPosition.x, imGuiSpeed);
				ImGui::DragFloat("AddEndFrame", &addSingleEnemyData_.endFrame, 0.1f, 0);

				if (addSingleEnemyNum_ < 0) {
					addSingleEnemyNum_ = 0;
				}

				if (addSingleEnemyData_.typeNum < 0) {
					addSingleEnemyData_.typeNum = 0;
				}
				else if (addSingleEnemyData_.typeNum > 1) {
					addSingleEnemyData_.typeNum = 1;
				}

				std::string nameSingleAdd = "SingleEnemyAdd" + std::to_string(stageCount);

				if (ImGui::Button(nameSingleAdd.c_str())) {
					// キー
					std::string key = "Enemy" + std::to_string(addSingleEnemyNum_);

					if (addSingleEnemyNum_ < 10) {
						key = "Enemy0" + std::to_string(addSingleEnemyNum_);
					}

					// 追加
					SetValue(stageName, key, addSingleEnemyData_);
					addSingleEnemyNum_++;
				}

				ImGui::EndTabItem();

			}
		
		}

		// タブバーを終了
		ImGui::EndTabBar();

	}

}

void EnemyEditor::ImGuiMultiEnemy()
{

	const float imGuiSpeed = 0.1f;

	// ステージの数だけ回す
	uint32_t stageCount = 0;

	if (ImGui::BeginTabBar("StageNum")) {

		for (std::map<std::string, EnemyEditorGroup>::iterator datasItr = datas_.begin();
			datasItr != datas_.end(); ++datasItr) {

			std::string stageName = "Stage" + std::to_string(stageCount);

			if (stageCount < 10) {
				stageName = "Stage0" + std::to_string(stageCount);
			}

			if (ImGui::BeginTabItem(stageName.c_str())) {

				std::string stageName = datasItr->first;
				EnemyEditorGroup& group = datasItr->second;

				// マルチエネミーの追加
				ImGui::SeparatorText("MultiEnemyAdd");

				ImGui::DragFloat3("AddPosition", &addMultieEnemyData_.position.x, imGuiSpeed);
				ImGui::DragFloat("AddDistance", &addMultieEnemyData_.distance, imGuiSpeed, 0);
				ImGui::DragInt("AddEnemyMaxCount", &addMultieEnemyData_.enemyMaxCount, imGuiSpeed, 0);
				ImGui::DragInt("AddMultiEnemyNum", &addMultiEnemyNum_, 0.1f, 0);
				ImGui::DragFloat("AddRotateSpeed", &addMultieEnemyData_.rotateSpeed, imGuiSpeed, 0);
				ImGui::DragInt("AddPadding", &addMultieEnemyData_.padding, 0.1f, 0);

				if (addMultieEnemyData_.enemyMaxCount < 0) {
					addMultieEnemyData_.enemyMaxCount = 0;
				}
				if (addMultiEnemyNum_ < 0) {
					addMultiEnemyNum_ = 0;
				}

				std::string nameTerrainAdd = "MultiEnemyAdd" + std::to_string(stageCount);

				if (ImGui::Button(nameTerrainAdd.c_str())) {
					// キー
					std::string key = "EnemyEmitter:" + std::to_string(addMultiEnemyNum_);

					if (addMultiEnemyNum_ < 10) {
						key = "EnemyEmitter:0" + std::to_string(addMultiEnemyNum_);
					}

					// 追加
					SetValue(stageName, key, addMultieEnemyData_);
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
					std::string key = "EnemyEmitter:" + std::to_string(deleteMultiEnemyNum_);

					if (deleteMultiEnemyNum_ < 10) {
						key = "EnemyEmitter:0" + std::to_string(deleteMultiEnemyNum_);
					}

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
						std::string nameRotateSpeed = stageName + name + "RotateSpeed";
						std::string namePadding = stageName + name + "Padding";

						ImGui::DragFloat3(namePosition.c_str(), &item.position.x, imGuiSpeed);
						ImGui::DragFloat(nameDistance.c_str(), &item.distance, imGuiSpeed, 0);
						ImGui::DragInt(nameEnemyMaxCount.c_str(), &item.enemyMaxCount, imGuiSpeed, 0);
						ImGui::DragFloat(nameRotateSpeed.c_str(), &item.rotateSpeed, imGuiSpeed, 0);
						ImGui::DragInt(namePadding.c_str(), &item.padding, imGuiSpeed, 0);

						if (item.enemyMaxCount < 0) {
							item.enemyMaxCount = 0;
						}

					}
				}

				ImGui::EndTabItem();

			}

			stageCount++;

		}

		// データが入ってない分
		for (; stageCount < StageNumberManager::kStageMax; ++stageCount) {

			std::string stageName = "Stage" + std::to_string(stageCount);

			if (stageCount < 10) {
				stageName = "Stage0" + std::to_string(stageCount);
			}

			if (ImGui::BeginTabItem(stageName.c_str())) {

				// マルチエネミーの追加
				ImGui::SeparatorText("MultiEnemyAdd");

				ImGui::DragFloat3("AddPosition", &addMultieEnemyData_.position.x, imGuiSpeed);
				ImGui::DragFloat("AddDistance", &addMultieEnemyData_.distance, imGuiSpeed, 0);
				ImGui::DragInt("AddEnemyMaxCount", &addMultieEnemyData_.enemyMaxCount, imGuiSpeed, 0);
				ImGui::DragInt("AddMultiEnemyNum", &addMultiEnemyNum_, 0.1f, 0);
				ImGui::DragFloat("AddRotateSpeed", &addMultieEnemyData_.rotateSpeed, imGuiSpeed, 0);
				ImGui::DragInt("AddPadding", &addMultieEnemyData_.padding, 0.1f, 0);

				if (addMultieEnemyData_.enemyMaxCount < 0) {
					addMultieEnemyData_.enemyMaxCount = 0;
				}
				if (addMultiEnemyNum_ < 0) {
					addMultiEnemyNum_ = 0;
				}

				std::string nameTerrainAdd = "MultiEnemyAdd" + std::to_string(stageCount);

				if (ImGui::Button(nameTerrainAdd.c_str())) {
					// キー
					std::string key = "EnemyEmitter:" + std::to_string(addMultiEnemyNum_);

					if (addMultiEnemyNum_ < 10) {
						key = "EnemyEmitter:0" + std::to_string(addMultiEnemyNum_);
					}

					// 追加
					SetValue(stageName, key, addMultieEnemyData_);
					addMultiEnemyNum_++;
				}

				ImGui::EndTabItem();

			}

		}

		// タブバーを終了
		ImGui::EndTabBar();

	}

}

void EnemyEditor::ImGuiChaseEnemy()
{
	const float imGuiSpeed = 0.1f;

	// ステージの数だけ回す
	uint32_t stageCount = 0;

	if (ImGui::BeginTabBar("StageNum")) {

		for (std::map<std::string, EnemyEditorGroup>::iterator datasItr = datas_.begin();
			datasItr != datas_.end(); ++datasItr) {

			std::string stageName = "Stage" + std::to_string(stageCount);

			if (stageCount < 10) {
				stageName = "Stage0" + std::to_string(stageCount);
			}

			if (ImGui::BeginTabItem(stageName.c_str())) {

				std::string stageName = datasItr->first;
				EnemyEditorGroup& group = datasItr->second;

				// シングルエネミーの追加
				ImGui::SeparatorText("ChaseEnemyAdd");

				ImGui::DragFloat3("AddPosition", &addChaseEnemyData_.position.x, imGuiSpeed);
				ImGui::DragFloat("AddSearchLength", &addChaseEnemyData_.searchLength, imGuiSpeed, 0);
				ImGui::DragInt("AddChaseEnemyNum", &addChaseEnemyNum_, 0.1f, 0);

				if (addChaseEnemyNum_ < 0) {
					addChaseEnemyNum_ = 0;
				}

				std::string nameChaseAdd = "ChaseEnemyAdd" + std::to_string(stageCount);

				if (ImGui::Button(nameChaseAdd.c_str())) {
					// キー
					std::string key = "ChaseEnemy" + std::to_string(addChaseEnemyNum_);

					if (addChaseEnemyNum_ < 10) {
						key = "ChaseEnemy0" + std::to_string(addChaseEnemyNum_);
					}

					// 追加
					SetValue(stageName, key, addChaseEnemyData_);
					addChaseEnemyNum_++;
				}

				// シングルエネミーの削除
				ImGui::SeparatorText("ChaseEnemyDelete");

				ImGui::DragInt("DeleteChaseEnemyNum", &deleteChaseEnemyNum_, 0.1f, 0);

				if (deleteChaseEnemyNum_ < 0) {
					deleteChaseEnemyNum_ = 0;
				}

				std::string nameSingleDelete = "ChaseEnemyDelete" + std::to_string(stageCount);

				if (ImGui::Button(nameSingleDelete.c_str())) {
					// キー
					std::string key = "ChaseEnemy" + std::to_string(deleteChaseEnemyNum_);

					if (deleteChaseEnemyNum_ < 10) {
						key = "ChaseEnemy0" + std::to_string(deleteChaseEnemyNum_);
					}


					// 指定グループに指定キーが存在するか
					if (datasItr->second.find(key) != datasItr->second.end()) {
						// 指定グループから指定のキーの値を取得
						datasItr->second.erase(key);
					}

				}

				// シングルエネミーの値の修正
				ImGui::SeparatorText("ChaseEnemyEdit");

				for (std::map<std::string, EnemyEditorItem>::iterator groupItr = group.begin();
					groupItr != group.end(); ++groupItr) {

					if (std::holds_alternative<ChaseEnemyData>(groupItr->second)) {

						ChaseEnemyData& item = std::get<ChaseEnemyData>(groupItr->second);

						std::string name = groupItr->first;
						ImGui::SeparatorText(name.c_str());

						std::string namePosition = stageName + name + "Position";
						std::string nameSearchLength = stageName + name + "SearchLength";

						ImGui::DragFloat3(namePosition.c_str(), &item.position.x, imGuiSpeed);
						ImGui::DragFloat(nameSearchLength.c_str(), &item.searchLength, imGuiSpeed);

					}

				}

				ImGui::EndTabItem();

			}

			stageCount++;

		}

		// データが入ってない分
		for (; stageCount < StageNumberManager::kStageMax; ++stageCount) {

			std::string stageName = "Stage" + std::to_string(stageCount);

			if (stageCount < 10) {
				stageName = "Stage0" + std::to_string(stageCount);
			}

			if (ImGui::BeginTabItem(stageName.c_str())) {

				// シングルエネミーの追加
				ImGui::SeparatorText("ChaseEnemyAdd");

				ImGui::DragFloat3("AddPosition", &addChaseEnemyData_.position.x, imGuiSpeed);
				ImGui::DragFloat("AddSearchLength", &addChaseEnemyData_.searchLength, imGuiSpeed, 0);
				ImGui::DragInt("AddChaseEnemyNum", &addChaseEnemyNum_, 0.1f, 0);

				if (addChaseEnemyNum_ < 0) {
					addChaseEnemyNum_ = 0;
				}

				std::string nameChaseAdd = "ChaseEnemyAdd" + std::to_string(stageCount);

				if (ImGui::Button(nameChaseAdd.c_str())) {
					// キー
					std::string key = "ChaseEnemy" + std::to_string(addChaseEnemyNum_);

					if (addChaseEnemyNum_ < 10) {
						key = "ChaseEnemy0" + std::to_string(addChaseEnemyNum_);
					}

					// 追加
					SetValue(stageName, key, addChaseEnemyData_);
					addChaseEnemyNum_++;
				}

				ImGui::EndTabItem();

			}
		
		}

		// タブバーを終了
		ImGui::EndTabBar();

	}
}
