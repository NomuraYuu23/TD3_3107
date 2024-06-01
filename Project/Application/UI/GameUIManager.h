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
/// ゲーム内UIマネージャー
/// </summary>
class GameUIManager
{
private: // プライベートなサブクラス

	/// <summary>
	/// テクスチャインデックス管理用
	/// </summary>
	enum TextureIndex
	{
		White2x2Tex,
		LeftStickNoneTex,
		LeftStickPressTex,
		AButtonNoneTex,
		AButtonPressTex,
		LeftThumbNoneTex,
		LeftThumbPressTex,
		RightThumbNoneTex,
		RightThumbPressTex,
		RightStickNoneTex,
		RightStickPressTex,
		JoyStickBackTex,
		DashImageTex,
		JumpImageTex,
		AimImageTex,
		ThrowTextTex,
		ReturnTextTex,
		HPGageTex,
		HPGageFrameTex,
		indexCount,	// インデックス数管理用
	};

	/// <summary>
	/// スプライトインデックス管理用
	/// </summary>
	enum SpriteIndex 
	{
		LeftStickBackSprite, // 左スティック背景用
		LeftStickSprite, // 左スティック用
		DashSprite,		// ダッシュ画像
		LBButtonSprite,  // LBボタン用
		JumpSprite,		// ジャンプ画像
		RightStickBackSprite, // 右スティック背景用
		RightStickSprite, // 右スティック用
		AimSprite,		// エイム画像
		RBButtonSprite, // RBボタン用
		TextSprite,		// テキスト用
		HPGageSprite,	 // ゲージ本体
		HPGageFrameSprite, // ゲージ背景
		ClearBackFrameSprite, // クリア背景
		spriteCount, // スプライト数用
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
	/// 接地状態セッター
	/// </summary>
	/// <param name="isGround">接地状態</param>
	void SetIsGround(bool& isGround) { isGrounded_ = isGround; }

	/// <summary>
	/// クリア状態セッター
	/// </summary>
	/// <param name="isClear">クリア状態</param>
	void SetISClear(const bool isClear) { isClear_ = isClear; }

	/// <summary>
	/// クリア演出終了状態ゲッター
	/// </summary>
	/// <returns>クリア演出終了状態</returns>
	bool GetClearAppear() { return isClearAppear_; }

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
	/// 左スティック関連更新関数
	/// </summary>
	void LeftStickUIUpdate();

	/// <summary>
	/// ジャンプに使用するボタンの更新関数
	/// </summary>
	void JumpButtonUIUpdate();

	/// <summary>
	/// 左スティック関連更新関数
	/// </summary>
	void RightStickUIUpdate();

	/// <summary>
	/// 槍投げに使用するボタンの更新関数
	/// </summary>
	void ThrowButtonUIUpdate();

	/// <summary>
	/// HPに関するUIの更新関数
	/// </summary>
	void HPUIUpdate();

	/// <summary>
	/// クリア時UIの更新関数
	/// </summary>
	void ClearUIUpdate();

private: // メンバ変数

	// DirectX汎用クラスのインスタンス
	DirectXCommon* dxCommon_ = nullptr;
	// テクスチャハンドルのマネージャー
	ITextureHandleManager* texHandleManager_ = nullptr;
	
	// 入力クラス
	Input* input_ = nullptr;

	// テクスチャ番号管理配列
	std::map<TextureIndex, uint32_t> texHandles_;

	/// UIスプライト配列
	std::vector<std::unique_ptr<Sprite>> uiSprites_;

	// プレイヤー
	Player* player_ = nullptr;

	// ImGui 調整用変数群
	bool isEditing_ = false; // 編集中フラグ
	int32_t editingIndex_ = 0; // 編集中インデックス番号
	Vector2 forImGuiPosition_; // 位置座標用
	Vector2 forImGuiSize_;	   // スケール用

private: // 左スティック用変数群

	// 左スティックUIの固定座標
	Vector2 stickUIPos_L_;
	// オフセット分
	float stickUIOffset_ = 10.0f;

private: // ジャンプボタン用変数群

	// 接地判定取得用
	bool isGrounded_ = false;
	// ジャンプUI色
	Vector4 jumpUIColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

private: // 右スティック用変数群

	// 左スティックUIの固定座標
	Vector2 stickUIPos_R_;

	// 槍投げUI色
	Vector4 rStickUIColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 throwUIColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

private: // hp用変数群


private: // クリア演出用変数群

	// クリアフラグ
	bool isClear_ = false;

	// 文字が出現し終わったか
	bool isClearAppear_ = false;

	// クリア背景演出用
	float currentBackAppearTime_ = 0.0f; // 現在時間
	float backAppearTime_		 = 1.5f; // 演出時間

	// クリア文字演出用
	float currentClearAppearTime_ = 0.0f; // 現在時間
	float clearAppearTime_		  = 1.0f; // 演出時間

};

