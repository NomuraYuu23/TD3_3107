#include "MathUtility.h"
#include "../../../Engine/Math/Matrix4x4.h"
#include "../../../Engine/base/WinApp.h"
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

float MathUtility::CalcAngle(const Vector2& direction)
{
    Vector2 direct = { 1 + direction.x,1 + direction.y };

    // 方向ベクトルの長さを計算
    float diMag = Vector2::Length(direct);

    // 位置ベクトルを考慮しないため、位置ベクトルの長さを0とします
    float obMag = Vector2::Length({ 1,1 });

    // 位置ベクトルと方向ベクトルの内積を計算
    float dot = Vector2::Dot({1,1}, direct);

    // cosθ を計算
    float cos = dot / (obMag * diMag);

    // アークコサインを使用してラジアンを計算
    float radians = std::acosf(cos);

    // ラジアンを度数に変換して返す
    return radians * (180.0f / (float)std::numbers::pi);
}

Vector2 MathUtility::WorldToScreen(const Vector3& position, BaseCamera* camera)
{
    // ビューポート行列
    Matrix4x4 matViewport =
        Matrix4x4::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
    // ビューポート行列
    Matrix4x4 matViewProjectionViewport = Matrix4x4::Multiply(camera->GetViewProjectionMatrix(), matViewport);
    // ワールド→スクリーン座標変換
    Vector3 positionReticle = Matrix4x4::Transform(position, matViewProjectionViewport);
   
    return Vector2(positionReticle.x, positionReticle.y);
}
