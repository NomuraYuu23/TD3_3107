#include "PlayerHitManager.h"
#include "../../Player.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"

void PlayerHitManager::Initialize(Player* player)
{
	player_ = player;

	hp_.max = (uint32_t)GlobalVariables::GetInstance()->GetIntValue("Player", "HitPoint");
	hp_.current = hp_.max;
	hp_.decreValue_ = 1;

	invMaxFrame_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "HitInvisibleFrame");
	defaultData_ = { 3.0f,{4.0f,0},{-3.0f,0},{1.5f,-1.5f},false,0 };
}

void PlayerHitManager::Update()
{
	if (!invisibleTimer_.IsActive()) {
		return;
	}

	invisibleTimer_.Update();
	hitEffectTimer_.Update();
	if (hitEffectTimer_.IsActive()) {
		hitEffect_.time = Ease::Easing(Ease::EaseName::Lerp, 0, defaultData_.time, hitEffectTimer_.GetNowFrame());
		if (hitEffect_.nowFrame > 2.0f) {
			hitEffect_.isStop = true;
		}
		else {
			hitEffect_.isStop = false;
		}
	}
	else {
		hitEffect_ = {};
	}
	hitEffect_.nowFrame++;

}

void PlayerHitManager::OnHit()
{
	if (!invisibleTimer_.IsActive()) {
		// 無敵開始
		invisibleTimer_.Start(invMaxFrame_);

		// デバッグ以外の場合行う
		#ifndef _DEBUG
		// ジャンプ効果音を再生
		player_->gameAudioManager_->PlayWave(GameAudioNameIndex::kPlayerDamage);
		#endif // !_DEBUG

		hitEffectTimer_.Start(15.0f);
		hitEffect_.bShift = { 3.0f,0 };
		hitEffect_.gShift = { -3.0f,0 };
		hitEffect_.rShift = { 1.5f,1.5f };

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

		// デバッグ以外の場合行う
		#ifndef _DEBUG
		// ジャンプ効果音を再生
		player_->gameAudioManager_->PlayWave(GameAudioNameIndex::kPlayerDamage);
		#endif // !_DEBUG

		// エフェクトの仮
		hitEffectTimer_.Start(15.0f);
		//hitEffect_.bShift = { 3.0f,0 };
		//hitEffect_.gShift = { -3.0f,0 };
		//hitEffect_.rShift = { 1.5f,1.5f };

		hitEffect_.rShift = defaultData_.rShift;
		hitEffect_.gShift = defaultData_.gShift;
		hitEffect_.bShift = defaultData_.bShift;

		hitEffect_.nowFrame = 0;
		hitEffect_.isStop = true;

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

	ImGui::DragFloat2("r", &defaultData_.rShift.x, 0.01f);
	ImGui::DragFloat2("g", &defaultData_.gShift.x, 0.01f);
	ImGui::DragFloat2("b", &defaultData_.bShift.x, 0.01f);
	ImGui::DragFloat("t", &defaultData_.time, 0.01f);
}
