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
	// 補正タイマー
	compTimer_.Update(GameSystemManager::sGameSpeed);

	// 補正中なら早期
	if (compTimer_.IsActive()) {
		// 接地したらキャンセル
		if (std::holds_alternative<GroundState*>(player_->GetNowState())) {
			Cancel();
			return;
		}
		// プレイヤーが下になったらキャンセル
		else if (player_->worldtransform_.GetWorldPosition().y < weapon_->worldtransform_.GetWorldPosition().y) {
			Cancel();
			return;
		}
		// 補正処理
		player_->worldtransform_.transform_.translate.x = Ease::Easing(Ease::EaseName::Lerp, startPosition_.x, weapon_->worldtransform_.GetWorldPosition().x, compTimer_.GetNowFrame());
		//player_->worldtransform_.transform_.translate.y = Ease::Easing(Ease::EaseName::Lerp, startPosition_.y, weapon_->worldtransform_.GetWorldPosition().y + 0.5f, compTimer_.GetNowFrame());
		return;
	}

	// 刺さってなければ早期
	if (!std::holds_alternative<ImpaledState*>(weapon_->GetNowState())) {
		return;
	}

	// 上向きに進んでいる場合キャンセル
	if (player_->velocity_.y > 0) {
		return;
	}
	// 下向きかつ空中のステートの場合
	else if(std::holds_alternative<AerialState*>(player_->GetNowState()) || std::holds_alternative<SpearAerialState*>(player_->GetNowState())){
		// 方向
		Vector3 direct = weapon_->worldtransform_.GetWorldPosition() - player_->worldtransform_.GetWorldPosition();
		// 方向と逆向きに進んでいる場合
		if (direct.x > 0 && player_->velocity_.x < 0) {
			return;
		}
		else if (direct.x < 0 && player_->velocity_.x > 0) {
			return;
		}

		float maxRadius = 0;
		float minRadius = 0;
		// X軸の方が大きい場合
		if (weapon_->boxCollider_.scale_.x > weapon_->boxCollider_.scale_.y) {
			// 横に倒れているため判定を切るY座標をちょい上げ
			if (player_->worldtransform_.GetWorldPosition().y < weapon_->worldtransform_.GetWorldPosition().y + weapon_->boxCollider_.scale_.y) {
				return;
			}
			maxRadius = weapon_->boxCollider_.scale_.x;
		}
		// Y軸の方が大きい場合
		else {
			if (player_->worldtransform_.GetWorldPosition().y < weapon_->worldtransform_.GetWorldPosition().y) {
				return;
			}
			maxRadius = weapon_->boxCollider_.scale_.y;
		}

		// 武器の座標取得
		Vector3 weaponPosition = weapon_->worldtransform_.GetWorldPosition();
		// ここ外部
		float offset = 1.0f;
		// オフセット分上に動かしてる
		weaponPosition.y += offset;

		// 最小・最大値
		Vector2 plMin = { player_->worldtransform_.GetWorldPosition().x - player_->circleCollider_.radius_,
			player_->worldtransform_.GetWorldPosition().y - player_->circleCollider_.radius_ };

		Vector2 plMax = { player_->worldtransform_.GetWorldPosition().x + player_->circleCollider_.radius_,
			player_->worldtransform_.GetWorldPosition().y + player_->circleCollider_.radius_ };

		// MaxRadiusは武器の長い方のサイズを取得
		// ここ外部
		float xRatio = 1.0f;
		float yRatio = 0.5f;
		// X軸の半径サイズ
		maxSize_.x = maxRadius * xRatio;

		// Y軸の半径サイス　
		maxSize_.y = maxRadius * yRatio;

		// 武器の最大最小
		Vector2 weaponMin = { weaponPosition.x - maxSize_.x,
			weaponPosition.y - maxSize_.y };

		Vector2 weaponMax = { weaponPosition.x + maxSize_.x,
			weaponPosition.y + maxSize_.y };


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
	ImGui::DragFloat2("maxSize", &maxSize_.x);
}

void SpearLandingAdjuster::SetUp()
{
	// ここ外部
	float lerpFrame = 5.0f;
	compTimer_.Start(lerpFrame);
	// 
	isRunOnce_ = true;
	// 開始座標
	startPosition_ = player_->worldtransform_.GetWorldPosition();

}
