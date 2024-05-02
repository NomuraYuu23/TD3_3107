#pragma once
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

class StageData
{
public: // 静的
	static StageData* GetInstance() {
		static StageData instance;
		return &instance;
	}
	static int32_t LoadNumber;
public: // サブクラス
    struct HierarchicalName
    {
        std::string kGroup;
        std::string kSection;
    };

private:
    StageData() = default;
	~StageData() = default;
    StageData(const StageData& variable) = default;
    StageData& operator=(const StageData& variable);

public:
    /// <summary>
    /// ファイルロード
    /// </summary>
    void LoadFiles();

    void LoadFile(const std::string& groupName);

    void SaveData(const std::string& groupName);

    /// <summary>
    /// グループ作成
    /// </summary>
    /// <param name="names"></param>
    void CreateGroup(const HierarchicalName names) {
        gameDatas_[names.kGroup][names.kSection];
    }

    /// <summary>
    /// データクリア
    /// </summary>
    void ClearGroup() {
        gameDatas_.clear();
    }

private:
	/// 項目
	using Item = std::variant<int32_t, uint32_t, float, Vector2, Vector3, std::string>;
	using Section = std::map<std::string, Item>;
	using Group = std::map<std::string, Section>;

    // 全データ
    std::map<std::string, Group> gameDatas_;

    // ImGuiの最大値
    const float kFabsValue_f = 2000.0f;
    const int kFabsValue_i = 2000;

    // 保存先のファイルパス
    const std::string kDirectoryPath = "Resources/StageData/";

public: // アクセッサ
#pragma region 設定
    template<typename T>
    void SetValue(const HierarchicalName& names, const std::string& key, T value) {
        // グループの参照を取得
        Section& section = gameDatas_[names.kGroup][names.kSection];
        // 新しい項目のデータを設定
        Item newItem = value;
        // 設定した項目をstd::mapに追加
        section[key] = newItem;
    }
#pragma endregion

#pragma region 追加
    template<typename T>
    void AddItem(const HierarchicalName& names, const std::string& key, T value) {
        // 項目が未登録なら
        if (gameDatas_[names.kGroup][names.kSection].find(key) ==
            gameDatas_[names.kGroup][names.kSection].end()) {
            SetValue(names, key, value);
        }
    }
#pragma endregion

#pragma region 取得
    template<typename T>
    T GetValue(const HierarchicalName& names, const std::string& key) {
        // 指定グループが存在するか
        assert(gameDatas_.find(names.kGroup) != gameDatas_.end());
        // セクション探し
        assert(gameDatas_[names.kGroup].find(names.kSection) != gameDatas_[names.kGroup].end());
        // セクションの参照を取得
        Section& section = gameDatas_[names.kGroup][names.kSection];

        // 指定グループに指定キーが存在するか
        assert(section.find(key) != section.end());
        // 指定グループから指定のキーの値を取得
        return std::get<T>(section[key]);
    }
#pragma endregion

};
