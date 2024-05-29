#pragma once
#include <vector>

#include "../../Engine/Math/Vector2.h"
#include "../../Engine/2D/Sprite.h"
#include "../AllSceneObject/StageNumberManager.h"

class StagePhot
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandles"></param>
	void Initialize(const std::array<uint32_t, StageNumberManager::kStageMax>& stagePhotTextureHandles);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 設定
	/// </summary>
	void Setting();

	/// <summary>
	/// 右移動
	/// </summary>
	/// <param name="t"></param>
	void MoveRight(float t);

	/// <summary>
	/// 左移動
	/// </summary>
	/// <param name="t"></param>
	void MoveLeft(float t);

	/// <summary>
	/// テクスチャ変更
	/// </summary>
	void TextureHandleChange();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	Vector2* GetCenterPhot() { return &positions_[2]; }

private: //メンバ関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

private: // 定数

	// スプライトの数
	static const uint32_t kSpriteNum = 5;

private:

	// テクスチャハンドル
	std::array<uint32_t, StageNumberManager::kStageMax> textureHandles_;

	// スプライト5つ
	std::array<std::unique_ptr<Sprite>, kSpriteNum> sprite_;

	// スプライト固定座標
	std::array<Vector2, kSpriteNum> positions_;

	// サイズ
	std::array<Vector2, kSpriteNum> sizes_;

	// 動きの変数
	float waveAnimation_t_ = 0.0f;
	Vector2 waveVelocity_ = {};

	// 振幅
	float amplitude_ = 0.3f;
	// フレーム
	float fre_ = 5.0f;

	// センターベースポジション
	Vector2 centerPositions_;

};

