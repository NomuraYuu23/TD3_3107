#pragma once
#include "../../../Engine/3D/LargeNumberOfObjects.h"
#include "../../../Engine/Animation/Animation.h"
#include "SingleEnemyData.h"
#include "ChaseEnemyData.h"

class Player;

class SingleEnemyRegister : public LargeNumberOfObjects
{
private:
	// 共通の番号
	static uint32_t sSerialNumber_;

	// シリアルナンバー
	uint32_t serialNum_ = 0;

public: 
	
	static void ResetSerialNumber() { sSerialNumber_ = 0; }

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="textureHnadles">テクスチャ</param>
	void Draw(BaseCamera& camera, std::vector<UINT>* textureHnadles) override;
	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void ImGuiDraw() override;

	/// <summary>
	/// 編集
	/// </summary>
	/// <param name="name"></param>
	/// <param name="singleEnemyData"></param>
	/// <param name="player"></param>
	void Edit(const std::string& name, const SingleEnemyData& singleEnemyData, Player* player);
	void Edit(const std::string& name, const ChaseEnemyData& chaseEnemyData, Player* player);
	//std::list<std::unique_ptr<OneOfManyObjects>>* GetList() { return &objects_; }

	void SetTexture(std::vector<UINT>* texture) { 
		texture_ = texture;
	}

	void Setting(Player* player, BaseCamera* camera) {
		player_ = player;
		camera_ = camera;
	}

private:
	// 
	std::string name_;

	// アニメーション本体
	Animation anim_;

	// テクスチャ
	std::vector<UINT>* texture_;
	UINT singleTexture_ = 0;

	Player* player_ = nullptr;
	BaseCamera* camera_ = nullptr;

};
