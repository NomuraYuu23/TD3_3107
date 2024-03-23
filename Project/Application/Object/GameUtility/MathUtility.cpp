#include "MathUtility.h"
#include <cmath>
#include <numbers>

float MathUtility::CalcAngle(const Vector2& position, const Vector2& direction)
{

    float obMag = Vector2::Length(position);
    float diMag = Vector2::Length(direction);

    float dot = Vector2::Dot(position, direction);

    float cos = dot / (obMag * diMag);

    float radians = std::acosf(cos);

    return radians * (180.0f / (float)std::numbers::pi);
}
