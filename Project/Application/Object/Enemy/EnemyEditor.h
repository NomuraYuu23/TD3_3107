#pragma once
#include <string>
#include <variant>
#include "SingleEnemyData.h"
#include "MultiEnemyData.h"

class EnemyEditor
{

public: // メンバ変数(編集)

	void ImGuiDraw();

public: // メンバ関数(読み込みなど)

	/// <summary>
	/// マップ読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// マップ読み込み
	/// </summary>
	void LoadFile(const std::string& groupName);

	/// <summary>
	/// 値のセットSingleEnemyData
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, SingleEnemyData value);

	/// <summary>
	/// 値のセットMultiEnemyData
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, MultiEnemyData value);

	/// <summary>
	/// 値の取得
	/// </summary>
	/// <returns></returns>
	SingleEnemyData GetSingleEnemyDataValue(const std::string& groupName, const std::string& key);

	/// <summary>
	/// 値の取得
	/// </summary>
	/// <returns></returns>
	MultiEnemyData GetMultiEnemyDataValue(const std::string& groupName, const std::string& key);

	/// <summary>
	/// データ保存
	/// </summary>
	void SaveData(const std::string& groupName);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	std::map<std::string, std::map<std::string, std::vector<SingleEnemyData, MultiEnemyData>>>* GetDatas() { return &datas_; }

private: // 変数

	//項目
	using Item = std::variant<SingleEnemyData, MultiEnemyData>;
	using Group = std::map<std::string, Item>; // ブロック番号, アイテム
	std::map<std::string, Group> datas_; // ステージ番号、Group

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/Enemy/";

	// ステージ数
	uint32_t maxStages_;
	// ステージ追加
	int32_t stageAddNum_;
	// ステージ削除
	int32_t stageDeleteNum_;

	// シングル追加データ
	SingleEnemyData addSingleEnemyData_;
	// シングル追加番号
	int32_t addSingleEnemyNum_;
	// シングル削除番号
	int32_t deleteSingleEnemyNum_;

};
