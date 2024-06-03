#pragma once
#include <string>
#include <variant>
#include "SingleEnemyData.h"
#include "MultiEnemyData.h"
#include "ChaseEnemyData.h"

class EnemyEditor
{

public:

	//項目
	using EnemyEditorItem = std::variant<SingleEnemyData, MultiEnemyData, ChaseEnemyData>;
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
	/// 値のセットChaseEnemyData
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, ChaseEnemyData value);

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
	/// 値の取得
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	ChaseEnemyData GetChaseEnemyDataValue(const std::string& groupName, const std::string& key);

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
	/// データの追加
	/// </summary>
	/// <param name="chackName">確認用の名前</param>
	/// <param name="stageCount">ステージカウント</param>
	/// <param name="imGuiSpeed">imGUiの速度</param>
	void DataAddSingleEnemy(const std::string& chackName, uint32_t stageCount, float imGuiSpeed);


	/// <summary>
	/// マルチ用
	/// </summary>
	void ImGuiMultiEnemy();

	/// <summary>
	/// チェイス用
	/// </summary>
	void ImGuiChaseEnemy();

private: // 変数

	std::map<std::string, EnemyEditorGroup> datas_; // ステージ番号、Group

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/Enemy/";

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

	// チェイス追加データ
	ChaseEnemyData addChaseEnemyData_;
	int32_t addChaseEnemyNum_;
	int32_t deleteChaseEnemyNum_;

	// モード
	int32_t mode_;

};
