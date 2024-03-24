#pragma once
#include "../../../Engine/Math/Vector2.h"
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/Camera/BaseCamera.h"

/// <summary>
/// 急遽追加する計算関数
/// </summary>
class MathUtility
{
public:
	static float Lerp(float start, float end, float t) {
		return start + (end - start) * t;
	}

	template <typename T>
	static T LerpT(T start, T end, float t) {
		return start + (end - start) * t;
	}

	static float CalcAngle(const Vector2& position, const Vector2& direction);

	static float CalcAngle(const Vector2& direction);

	static Vector2 WorldToScreen(const Vector3& position, BaseCamera* camera);

};

