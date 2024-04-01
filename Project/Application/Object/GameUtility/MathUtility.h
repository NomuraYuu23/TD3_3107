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

	/// <summary>
	/// ワールド座標からスクリーン座標へ
	/// </summary>
	/// <param name="position"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static Vector2 WorldToScreen(const Vector3& position, BaseCamera* camera);
	/// <summary>
	/// 画面内かチェックする関数
	/// </summary>
	/// <param name="worldPosition"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static bool CheckOutScreen(const Vector3& worldPosition, const Vector2& offset, const BaseCamera& camera);
	static bool CheckOutScreen(const Vector3& worldPosition, float offset, const BaseCamera& camera);

};

