#include "EnemyEditor.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../externals/nlohmann/json.hpp"
#include <fstream>
#include <cassert>

//名前空間
using namespace nlohmann;

void EnemyEditor::ImGuiDraw()
{
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

void EnemyEditor::SetValue(const std::string& groupName, const std::string& key, MultiEnemyData value)
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

SingleEnemyData EnemyEditor::GetSingleEnemyDataValue(const std::string& groupName, const std::string& key)
{

	// 指定グループが存在するか
	assert(datas_.find(groupName) != datas_.end());
	//  グループの参照を取得
	Group& group = datas_[groupName];
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
	Group& group = datas_[groupName];
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
