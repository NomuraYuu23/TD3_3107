#pragma once

#include "../../../Engine/3D/LargeNumberOfObjects.h"

class ArrowManager :
    public LargeNumberOfObjects
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

private:

	/// <summary>
	/// 登録
	/// </summary>
	/// <param name="transform"></param>
	/// <param name="name"></param>
	void Register(
		const EulerTransform& transform,
		const std::string& name);

private:

	// 追加用トランスフォーム
	EulerTransform addTransform_;

	// 追加用名前
	int32_t addNameNumber_;

};

