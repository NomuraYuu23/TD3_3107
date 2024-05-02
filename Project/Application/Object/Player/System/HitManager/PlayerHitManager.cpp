#include "PlayerHitManager.h"
#include "../../Player.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void PlayerHitManager::Initialize(Player* player)
{
	player_ = player;

	hp_.max = (uint32_t)GlobalVariables::GetInstance()->GetIntValue("Player", "HitPoint");
	hp_.current = hp_.max;
	hp_.decreValue_ = 1;

	invMaxFrame_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "HitInvisibleFrame");

}

void PlayerHitManager::Update()
{
	if (!invisibleTimer_.IsActive()) {
		return;
	}

	invisibleTimer_.Update();
}

void PlayerHitManager::OnHit()
{
	if (!invisibleTimer_.IsActive()) {
		// 無敵開始
		invisibleTimer_.Start(invMaxFrame_);
		// HP削る
		hp_.current -= hp_.decreValue_;
		// 死亡処理
		if (hp_.current <= 0) {
			player_->SetIsDead(true);
		}
	}

}

void PlayerHitManager::OnHit(uint32_t decrement)
{
	if (!invisibleTimer_.IsActive()) {
		// 無敵開始
		invisibleTimer_.Start(invMaxFrame_);
		// HP削る
		hp_.current -= decrement;
		// 死亡処理
		if (hp_.current <= 0) {
			player_->SetIsDead(true);
		}
	}
}

void PlayerHitManager::ImGuiDraw()
{
	int hp = hp_.current;
	ImGui::DragInt("currentHP", &hp);

}
