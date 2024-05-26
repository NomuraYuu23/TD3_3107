#pragma once
#include <map>
#include <string>
#include "CheckPointData.h"
class CheckPointEditor
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
	void SetValue(const std::string& groupName, const std::string& key, CheckPointData value);

	/// <summary>
	/// 値の取得
	/// </summary>
	/// <returns></returns>
	CheckPointData GetValue(const std::string& groupName, const std::string& key);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	std::map<std::string, std::map<std::string, CheckPointData>>* GetDatas() { return &datas_; }


private: // 変数

	//項目
	using Item = CheckPointData;
	using Group = std::map<std::string, Item>; // ブロック番号, アイテム
	std::map<std::string, Group> datas_; // ステージ番号、Group

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/Map/";

	// 追加用
	CheckPointData addCheckPointData_;
	int32_t addCheckPointNum_;

	// 削除用
	int32_t deleteCheckPointNum_;

};

