#pragma once
#include <cstdint>
#include "../../Engine/Input/Input.h"
#include "StagePhot.h"
#include "StageSelectUI.h"
#include "StageMax.h"

class SelectSystem
{

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="stagePhotTextureHandles">ステージの写真ハンドル</param>
	/// <param name="stageUITextureHandles">UIハンドル</param>
	void Initialize(
		const std::array<uint32_t, kStageMax>& stagePhotTextureHandles, 
		const std::array<uint32_t, StageSelectUI::SpriteIndex::kSpriteIndexOfCount>& stageUITextureHandles);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ関数

	/// <summary>
	/// 右移動
	/// </summary>
	void MoveRight();

	/// <summary>
	/// 左移動
	/// </summary>
	void MoveLeft();

public: // アクセッサ

	uint32_t GetStageNum() { return stageNum_; }

	bool GetGotoGameScene() { return gotoGameScene_; }

	bool GetGotoTitleScene() { return gotoTitleScene_; }

private: // 変数
	
	// 入力
	Input* input_ = nullptr;

	// 右に動いているか
	bool isMoveRight_;
	// 左に動いているか
	bool isMoveLeft_;

	// イージングタイマー
	float easeTimer_;
	// イージングスピード
	float easeSpeed_;
	
	// ステージ番号
	uint32_t stageNum_;

	// ゲームシーンフラグ
	bool gotoGameScene_ = false;
	// タイトルシーン
	bool gotoTitleScene_ = false;

	// ステージ写真
	std::unique_ptr<StagePhot> stagePhot_ = nullptr;
	// UI
	std::unique_ptr<StageSelectUI> stageSelectUI_ = nullptr;

};

