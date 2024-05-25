#include "SpearLandingAdjuster.h"
#include "../../../ObjectList.h"
#include "../../../GameSystem/GameSystemManager.h"
#include "../../../Engine/Math/Ease.h"

void SpearLandingAdjuster::Initialize(Player* player, Weapon* weapon)
{
	// ポインタの設定
	player_ = player;
	weapon_ = weapon;

}

void SpearLandingAdjuster::Update()
{

	compTimer_.Update(GameSystemManager::sGameSpeed);

	// 補正中なら早期
	if (compTimer_.IsActive()) {

		player_->worldtransform_.transform_.translate.x = Ease::Easing(Ease::EaseName::Lerp, startPosition_.x, weapon_->worldtransform_.GetWorldPosition().x, compTimer_.GetNowFrame());

		return;
	}
	//// 飛んでなければ早期
	//if (!std::holds_alternative<SpearAerialState*>(player_->GetNowState()) ||
	//	!std::holds_alternative<AerialState*>(player_->GetNowState())) {
	//	return;
	//}
	// 刺さってなければ早期
	if (!std::holds_alternative<ImpaledState*>(weapon_->GetNowState())) {
		return;
	}

	if (player_->velocity_.y > 0) {
		return;
	}
	else if(std::holds_alternative<AerialState*>(player_->GetNowState()) || std::holds_alternative<SpearAerialState*>(player_->GetNowState())){

		Vector3 direct = weapon_->worldtransform_.GetWorldPosition() - player_->worldtransform_.GetWorldPosition();

		if (direct.x > 0 && player_->velocity_.x < 0) {
			return;
		}
		else if (direct.x < 0 && player_->velocity_.x > 0) {
			return;
		}

		// 最小・最大値
		Vector2 plMin = { player_->worldtransform_.GetWorldPosition().x - player_->circleCollider_.radius_,
			player_->worldtransform_.GetWorldPosition().y - player_->circleCollider_.radius_ };

		Vector2 plMax = { player_->worldtransform_.GetWorldPosition().x + player_->circleCollider_.radius_,
			player_->worldtransform_.GetWorldPosition().y + player_->circleCollider_.radius_ };

		float maxRadius = 0;
		//float scaling = 0.5f;
		if (weapon_->boxCollider_.scale_.x > weapon_->boxCollider_.scale_.y) {
			maxRadius = weapon_->boxCollider_.scale_.x;
		}
		else {
			maxRadius = weapon_->boxCollider_.scale_.y;
		}

		maxSize_ = maxRadius * 0.75f + scaling_;

		Vector2 weaponMin = { weapon_->worldtransform_.GetWorldPosition().x - maxRadius,
			weapon_->worldtransform_.GetWorldPosition().y - maxRadius };

		Vector2 weaponMax = { weapon_->worldtransform_.GetWorldPosition().x + maxRadius,
			weapon_->worldtransform_.GetWorldPosition().y + maxRadius };


		IObject::FourTop player4Top = IObject::GenerateFourTop(plMin, plMax);
		IObject::FourTop weapon4Top = IObject::GenerateFourTop(weaponMin, weaponMax);

		IObject::CollisionType type = IObject::GetCollisionType(player4Top, weaponMin, weaponMax);

		// どこかが当たったらセットアップ関数を呼び出す（移動補正
		if (type != IObject::kNone) {
			SetUp();
		}


	}


}

void SpearLandingAdjuster::ImGuiDraw()
{
	ImGui::DragFloat("scaling", &scaling_, 0.01f);
	ImGui::DragFloat("maxSize", &maxSize_);
}

void SpearLandingAdjuster::SetUp()
{
	compTimer_.Start(10.0f);

	startPosition_ = player_->worldtransform_.GetWorldPosition();

}
