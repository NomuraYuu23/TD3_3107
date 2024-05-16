#include "SpearAnimManager.h"
#include "../Weapon.h"

void SpearAnimManager::Init(Weapon* weapon)
{
	// 武器取得
	weapon_ = weapon;

	// アニメーション取得と初期化
	anim_.Initialize(
		weapon_->model_->GetNodeAnimationData(),
		weapon_->localMatrixManager_->GetInitTransform(),
		weapon_->localMatrixManager_->GetNodeNames());

	// 全アニメーション停止
	StopAnimationAll();

	// アニメーション開始
	anim_.StartAnimation(0, true);
}

void SpearAnimManager::Update()
{
	// アニメーションの更新
	weapon_->localMatrixManager_->SetNodeLocalMatrix(anim_.AnimationUpdate());
	weapon_->localMatrixManager_->Map();
}

void SpearAnimManager::PlayAnimation(int32_t animNum, bool isLoop, bool isFinish)
{
	// アニメーション停止トリガーで分岐
	if (isFinish) {
		// 全アニメーション停止
		StopAnimationAll();
	}

	// 指定した番号のアニメーション再生
	anim_.StartAnimation(animNum, isLoop);
}

void SpearAnimManager::StopAnimationAll()
{
	// 全アニメーション分ループ
	for (int i = 0; i < SpearAnim::SpearBounce; i++) {
		anim_.StopAnimation(i);
	}
}
