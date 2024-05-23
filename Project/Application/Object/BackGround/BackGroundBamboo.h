#pragma once
#include "../../../Engine/3D/OneOfManyObjects.h"
#include <string>

/// <summary>
/// 背景用の竹１つ１つ
/// </summary>
class BackGroundBamboo : public OneOfManyObjects
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">初期化時の名前</param>
	void Initialize(const std::string& name);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

private: // メンバ変数

	// 竹名
	std::string name_;
	
};

