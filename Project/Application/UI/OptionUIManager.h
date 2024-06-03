#pragma once
#include <memory>
#include <cstdint>
#include <vector>
#include <map>
#include "../../Engine/base/TextureManager.h"
#include "../../Engine/2D/Sprite.h"
#include "../../../Engine/Input/Input.h"

// クラスの前方宣言
class Player;

/// <summary>
/// オプション画面のUIマネージャー
/// </summary>
class OptionUIManager
{
private: // プライベートなサブクラス

	/// <summary>
	/// オプション用テクスチャインデックス管理用
	/// </summary>
	enum OTextureIndex {
		OptionTextTex, // オプションテキスト
		CursorTex, // カーソル
		ControlTextTex, // 操作テキスト
		AimAssistTextTex, // エイムアシスト
		ChangeControlTypeTex, // 操作タイプ切り替えテクスチャ
		RightArrowTex, // 右矢印テクスチャ
		LeftArrowTex, // 左矢印テクスチャ
		ThrowControlsTex, // 投げ操作に関する連番テクスチャ
		AimStrengthTex, // エイムアシスト操作に関する連番テクスチャ
		OTextureCount,
	};

	/// <summary>
	/// オプション用スプライトインデックス管理用
	/// </summary>
	enum OSpriteIndex {
		OptionTextSprite,
		OptionCategoryCursorSprite, // カーソル(カテゴリ用)
		OptionControlTextSprite, // 操作カテゴリスプライト
		OptionAimAssistSprite, // エイムアシストカテゴリスプライト
		OptionSelectedCategoryUI, // 選択中カテゴリUI
		Control_RightArrowSprite, // 右矢印UI
		Control_LeftArrowSprite, // 左矢印UI
		OSpriteCount,
	};

	/// <summary>
	/// 選択中カテゴリスプライト
	/// </summary>
	enum CategorySpriteIndex {
		Category_Control, // 操作系
		Category_AimAssist, // エイムアシスト
	};

	/// <summary>
	/// 操作オプション選択値
	/// </summary>
	enum ControlType {
		Control_RBPressThrow,
		Control_RBReleaseThrow,
		Control_RStickReleaseThrow,
	};

	/// <summary>
	/// エイムアシストオプション選択値
	/// </summary>
	enum AssistType {
		Assist_Low,
		Assist_Medium,
		Assist_High,
	};

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="texHandleManager">テクスチャハンドルマネージャーのインスタンス</param>
	void Initialze(ITextureHandleManager* texHandleManager);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGui描画関数
	/// </summary>
	void DisplayImGui();

public: // アクセッサ等

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 描画フラグゲッター
	/// </summary>
	/// <returns>描画を行うか</returns>
	bool GetIsDraw() { return isDraw_; }
	/// <summary>
	/// 描画フラグセッター
	/// </summary>
	/// <param name="isDraw">描画を行うか</param>
	void SetIsDraw(const bool isDraw);

private: // プライベートなメンバ関数

	/// <summary>
	/// テクスチャロード関数
	/// </summary>
	void LoadTexture();

	/// <summary>
	/// スプライト生成関数
	/// </summary>
	void CreateSprite();

	/// <summary>
	/// カテゴリ関連更新関数
	/// </summary>
	void CategoryUpdate();

	/// <summary>
	/// 操作カテゴリ更新
	/// </summary>
	void ControlCategoryUpdate();

	/// <summary>
	/// エイムアシストカテゴリ更新
	/// </summary>
	void AimAssistCategoryUpdate();

private: // メンバ変数

	// DirectX汎用クラスのインスタンス
	DirectXCommon* dxCommon_ = nullptr;
	// テクスチャハンドルのマネージャー
	ITextureHandleManager* texHandleManager_ = nullptr;

	// 入力クラス
	Input* input_ = nullptr;

	// テクスチャ番号管理配列
	std::map<OTextureIndex, uint32_t> texHandles_;

	/// UIスプライト配列
	std::vector<std::unique_ptr<Sprite>> uiSprites_;

	// プレイヤー
	Player* player_ = nullptr;

	// オプション描画フラグ
	bool isDraw_ = false;

	// ImGui 調整用変数群
	bool isEditing_ = false; // 編集中フラグ
	int32_t editingIndex_ = 0; // 編集中インデックス番号
	Vector2 forImGuiPosition_; // 位置座標用
	Vector2 forImGuiSize_;	   // スケール用

private: // カテゴリ関連変数群

	// 選択中カテゴリ
	int SelectedCategory_ = Category_Control;
	// 選択している状態か
	bool isCategorySelected_ = false;

	// カーソル移動クールタイム
	float currentCategoryCursorCoolTime_ = 1.0f;
	float categoryCursorCoolTime_ = 0.25f;

	// カテゴリスプライト透明度
	float categoryAlpha_ = 1.0f;

	// 操作カテゴリ座標
	Vector2 categoryTextPos_;
	// エイムアシストカテゴリ座標
	Vector2 aimAssistTextPos_;

	// 操作タイプ
	int SelectedControlType_ = Control_RBPressThrow;

	// エイムアシストタイプ
	int SelectedAimAssistType_ = Assist_Medium;

};

