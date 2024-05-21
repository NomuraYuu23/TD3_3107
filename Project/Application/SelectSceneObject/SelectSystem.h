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
	/// 
	/// </summary>
	void Initialize(
		const std::array<uint32_t, kStageMax>& stagePhotTextureHandles, 
		const std::array<uint32_t, StageSelectUI::SpriteIndex::kSpriteIndexOfCount>& stageUITextureHandles);

	/// <summary>
	/// 
	/// </summary>
	void Update();

	/// <summary>
	/// 
	/// </summary>
	void Draw();

private: // メンバ関数

	// 右移動
	void MoveRight();

	// 左移動
	void MoveLeft();

public: // アクセッサ

	StageSelectUI* GetStageSelectUI() { return stageSelectUI_.get(); }

private: // 変数
	
	// 入力
	Input* input_ = nullptr;

	// 動いているか
	bool isMoveRight_;
	bool isMoveLeft_;

	// イージング
	float easeTimer_;
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

