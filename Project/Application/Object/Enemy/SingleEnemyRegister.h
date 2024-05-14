#pragma once
#include "../../../Engine/3D/LargeNumberOfObjects.h"

class SingleEnemyRegister : public LargeNumberOfObjects
{
private:
	// 共通の番号
	static uint32_t sSerialNumber_;

	// シリアルナンバー
	uint32_t serialNum_ = 0;

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
	/// ImGuiの描画
	/// </summary>
	void ImGuiDraw() override;
	//std::list<std::unique_ptr<OneOfManyObjects>>* GetList() { return &objects_; }

private:
	// 
	std::string name_;
};
