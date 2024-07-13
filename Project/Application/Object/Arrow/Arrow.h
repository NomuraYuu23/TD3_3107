#pragma once
#include "../../../Engine/3D/OneOfManyObjects.h"
class Arrow :
    public OneOfManyObjects
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="transform">トランスフォーム</param>
	/// <param name="name">名前</param>
	void Initialize(
		const EulerTransform& transform,
		const std::string& name);

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

private: 

	// 名前
	std::string name_;

	

};

