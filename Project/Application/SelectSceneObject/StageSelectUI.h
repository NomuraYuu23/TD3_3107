#pragma once
#include <cstdint>
#include "../../Engine/Math/Vector2.h"
#include "../../Engine/2D/Sprite.h"
class StageSelectUI
{

public: // サブクラス

	/// <summary>
	/// スプライト種類
	/// </summary>
	enum SpriteIndex {
		kSpriteIndexLeftArrow, // 左矢印
		kSpriteIndexRightArrow, // 右矢印
		kSpriteIndexStage, // 「ステージ」
		kSpriteIndexStageNumber, // ステージ番号(連番)
		kSpriteIndexOperation, // 操作
		kSpriteIndexOfCount // 数える用。使わない
	};

	/// <summary>
	/// スプライト構造体
	/// </summary>
	struct SpriteStruct
	{
		// テクスチャハンドル
		uint32_t textureHandle_;
		// 位置
		Vector2 postion_;
		// サイズ
		Vector2 size_;
		// スプライト
		std::unique_ptr<Sprite> sprite_;

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="textureHandle">テクスチャハンドル</param>
		/// <param name="postion">位置</param>
		/// <param name="size">サイズ</param>
		/// <param name="color">色</param>
		void Initialize(
			uint32_t textureHandle, 
			const Vector2& postion, 
			const Vector2& size, 
			const Vector4& color);

	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandles"></param>
	void Initialize(const std::array<uint32_t, SpriteIndex::kSpriteIndexOfCount> textureHandles);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(bool notMove);

	/// <summary>
	/// 設定
	/// </summary>
	void Setting();

	/// <summary>
	/// ステージナンバーセット
	/// </summary>
	/// <param name="stageNum"></param>
	void SetStageNum(uint32_t stageNum);

private: //メンバ関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

private: // 定数

	// ステージ番号の大きさ
	static const Vector2 kStageNumSize;

private: // 変数

	// スプライトデータ
	std::array<SpriteStruct, SpriteIndex::kSpriteIndexOfCount> spriteDatas_;

	// ステージナンバー
	uint32_t stageNum_;

	// 動きの変数
	float waveAnimationT_;
	Vector2 waveVelocity_ = {};

};

