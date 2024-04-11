#include "FreeFallState.h"
#include "../../Weapon/Weapon.h"
#include "../../GameUtility/MathUtility.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Math/DeltaTime.h"

void FreeFallState::Initialize()
{

	// ステート更新
	SetNowState(this);
	// 
	//weapon_->worldtransform_.usedDirection_ = false;
	//weapon_->worldtransform_.transform_.rotate.x = 1.57f;

	float valueX = 8.5f;
	weapon_->velocity_.x = weapon_->invDirect_.x * valueX;
	weapon_->velocity_.y = 40.0f;
}

void FreeFallState::Update()
{

	//weapon_->worldtransform_.transform_.rotate.z += 0.15f;

	//weapon_->worldtransform_.rotateMatrix_

	weapon_->velocity_.x = MathUtility::Lerp(weapon_->velocity_.x, 0, 0.01f);
	weapon_->velocity_.y += (kGravity * 8.0f) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

	weapon_->worldtransform_.direction_ = rotateVector(weapon_->worldtransform_.direction_, (3.14f / 18.0f));
	weapon_->worldtransform_.transform_.translate += weapon_->velocity_ * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

}

Vector3 FreeFallState::rotateVector(const Vector3& direct, float theta)
{
	float cosTheta = std::cosf(theta);
	float sinTheta = std::sinf(theta);

	Vector3 result = {};
	result.x = direct.x * cosTheta - direct.y * sinTheta;
	result.y = direct.x * sinTheta + direct.y * cosTheta;

	return result;
}
