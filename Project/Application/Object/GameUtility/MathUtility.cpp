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

float MathUtility::Ratio(float min, float max, float rate)
{
    return min + (max - min) * rate;
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

bool MathUtility::CheckOutScreen(const Vector3& worldPosition, const Vector2& offset, const BaseCamera& camera)
{
    // スクリーン座標
    Vector2 screenPosition = MathUtility::WorldToScreen(worldPosition, &const_cast<BaseCamera&>(camera));
    Vector2 deadZone = { (float)WinApp::kWindowWidth + std::fabsf(offset.x), (float)WinApp::kWindowHeight + std::fabsf(offset.y) };
    if ((screenPosition.x > deadZone.x || screenPosition.x < -std::fabsf(offset.x)) || (screenPosition.y > deadZone.y || screenPosition.y < -std::fabsf(offset.y))) {
        return true;
    }
    return false;
}

bool MathUtility::CheckOutScreen(const Vector3& worldPosition, float offset, const BaseCamera& camera)
{
    // スクリーン座標
    Vector2 screenPosition = MathUtility::WorldToScreen(worldPosition, &const_cast<BaseCamera&>(camera));
    Vector2 deadZone = { (float)WinApp::kWindowWidth + std::fabsf(offset), (float)WinApp::kWindowHeight + std::fabsf(offset) };
    if ((screenPosition.x > deadZone.x || screenPosition.x < -std::fabsf(offset)) || (screenPosition.y > deadZone.y || screenPosition.y < -std::fabsf(offset))) {
        return true;
    }
    return false;
}

Vector3 MathUtility::RotateVector(const Vector3& direct, float theta)
{
    float cosTheta = std::cosf(theta);
    float sinTheta = std::sinf(theta);

    Vector3 result = {};
    result.x = direct.x * cosTheta - direct.y * sinTheta;
    result.y = direct.x * sinTheta + direct.y * cosTheta;

    return result;
}

Vector2 MathUtility::ScreenPositionRatio(const Vector3& worldPosition, BaseCamera* camera)
{
    Vector2 screenPosition = WorldToScreen(worldPosition, camera);
    Vector2 result = { screenPosition.x / (float)WinApp::kWindowWidth,screenPosition.y / (float)WinApp::kWindowHeight };
    return result;
}
