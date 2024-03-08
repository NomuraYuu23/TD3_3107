#pragma once
#include "../../../Engine/Camera/BaseCamera.h"

class GameBasicCamera : public BaseCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

public:
	void ImGuiDraw();

};

