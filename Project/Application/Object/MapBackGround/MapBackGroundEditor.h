#pragma once
#include <string>
#include <variant>
#include "MapBackGroundData.h"
class MapBackGroundEditor
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
	void SetValue(const std::string& groupName, const std::string& key, MapBackGroundData value);

	/// <summary>
	/// 値の取得
	/// </summary>
	/// <returns></returns>
	MapBackGroundData GetValue(const std::string& groupName, const std::string& key);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	std::map<std::string, std::map<std::string, MapBackGroundData>>* GetDatas() { return &datas_; }

private: // 変数

	//項目
	using Item = MapBackGroundData;
	using Group = std::map<std::string, Item>; // ブロック番号, アイテム
	std::map<std::string, Group> datas_; // ステージ番号、Group

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/MapBackGround/";

	// 追加用
	MapBackGroundData addMapBlockData_;
	int32_t addMapBlockNum_;

	// 削除用
	int32_t deleteMapBlockNum_;


};

