#pragma once
#include "IActionState.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Vector2.h"

class SpearAerialState : public IActionState
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


private:
	float decreeSpeed_ = 2.0f;

	float gravity_ = 0;

	Vector2 moveDirection_ = {};

private:
	float CreateNewSpeed(float direct) {
		const char* groupName = "Player";
		float power = GlobalVariables::GetInstance()->GetFloatValue(groupName, "HorizontalPower");
		return direct * power;
	}

public:
	void InitializeDirection(const Vector2& direct);

};

