#pragma once
/// <summary>
/// 急遽追加する計算関数
/// </summary>
class MathUtility
{
public:

	static float Lerp(float start, float end, float t) {
		return start + (end - start) * t;
	}

};

