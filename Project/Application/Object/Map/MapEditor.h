#pragma once
#include <string>
#include <variant>
#include "MapBlockData.h"
class MapEditor
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
	/// 値のセット
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, MapBlockData value);

	/// <summary>
	/// 値の取得
	/// </summary>
	/// <returns></returns>
	MapBlockData GetValue(const std::string& groupName, const std::string& key);

	/// <summary>
	/// データ保存
	/// </summary>
	void SaveData(const std::string& groupName);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

private: // 変数

	//項目
	using Item = MapBlockData;
	using Group = std::map<std::string, Item>; // ブロック番号, アイテム
	std::map<std::string, Group> datas_; // ステージ番号、Group

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/Map/";

	// ステージ数
	uint32_t maxStages_;

	// 追加用
	MapBlockData addMapBlockData_;
	int32_t stageAddNum_;

	int32_t addMapBlockNum_;

	// 削除用
	int32_t stageDeleteNum_;
	int32_t deleteMapBlockNum_;


};

