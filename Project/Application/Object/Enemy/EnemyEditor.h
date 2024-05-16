#pragma once
#include <string>
#include <variant>
#include "SingleEnemyData.h"
#include "MultiEnemyData.h"

class EnemyEditor
{

public:

	//項目
	using EnemyEditorItem = std::variant<SingleEnemyData, MultiEnemyData>;
	using EnemyEditorGroup = std::map<std::string, EnemyEditorItem>; // ブロック番号, アイテム

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

	std::map<std::string, EnemyEditorGroup>* GetDatas() { return &datas_; }

private: // 関数

	/// <summary>
	/// シングル用
	/// </summary>
	void ImGuiSingleEnemy();

	/// <summary>
	/// マルチ用
	/// </summary>
	void ImGuiMultiEnemy();

private: // 変数

	std::map<std::string, EnemyEditorGroup> datas_; // ステージ番号、Group

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

	// マルチ追加データ
	MultiEnemyData addMultieEnemyData_;
	// マルチ追加番号
	int32_t addMultiEnemyNum_;
	// マルチ削除番号
	int32_t deleteMultiEnemyNum_;

	// モード
	int32_t mode_;

};
