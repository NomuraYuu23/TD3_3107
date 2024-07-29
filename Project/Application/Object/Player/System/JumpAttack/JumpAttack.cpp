#include "JumpAttack.h"
#include "../../../GameSystem/GameSystemManager.h"

void JumpAttack::StartAccept(float invincibleFrame)
{
	// タイマーの開始
	timer_.Start(invincibleFrame);
}

void JumpAttack::Update()
{
	// タイマー更新
	timer_.Update(GameSystemManager::sGameSpeed);
}

void JumpAttack::Cancel()
{
	// タイマーの終了
	timer_.End();
}
