#include "SpearAnimManager.h"
#include "../Weapon.h"

void SpearAnimManager::Init(Weapon* weapon)
{
	// 武器取得
	weapon_ = weapon;

	// アニメーション取得と初期化
	spearAnim_.Initialize(
		weapon_->model_->GetNodeAnimationData(),
		weapon_->localMatrixManager_->GetInitTransform(),
		weapon_->localMatrixManager_->GetNodeNames());

	// 全アニメーション停止
	StopSpearAnimationAll();

	// アニメーション開始
	spearAnim_.StartAnimation(0, true);
}

void SpearAnimManager::Update()
{
	// アニメーションの更新
	weapon_->localMatrixManager_->SetNodeLocalMatrix(spearAnim_.AnimationUpdate());
	weapon_->localMatrixManager_->Map();

	// セットアップ済みであれば
	if (isSetUpRingAnim_) {
		// リングアニメーション再生
		weapon_->GetRingLocalMatrix()->SetNodeLocalMatrix(ringAnim_.AnimationUpdate());
		weapon_->GetRingLocalMatrix()->Map();
	}
}

void SpearAnimManager::PlaySpearAnimation(int32_t animNum, bool isLoop, bool isFinish)
{
	// アニメーション停止トリガーで分岐
	if (isFinish) {
		// 全アニメーション停止
		StopSpearAnimationAll();
	}

	// 指定した番号のアニメーション再生
	spearAnim_.StartAnimation(animNum, isLoop);
}

void SpearAnimManager::PlayRingAnimation(int32_t animNum, bool isLoop, bool isFinish)
{
	// アニメーション停止トリガーで分岐
	if (isFinish) {
		// 全アニメーション停止
		StopRingAnimationAll();
	}

	// 指定した番号のアニメーション再生
	ringAnim_.StartAnimation(animNum, isLoop);
}

void SpearAnimManager::SetUpRingAnim()
{
	// リングアニメーション取得と初期化
	ringAnim_.Initialize(
		weapon_->GetRingModel()->GetNodeAnimationData(),
		weapon_->GetRingLocalMatrix()->GetInitTransform(),
		weapon_->GetRingLocalMatrix()->GetNodeNames());

	// 全アニメーション停止
	StopRingAnimationAll();

	// アニメーション開始
	ringAnim_.StartAnimation(RingIdle, true);

	// セットアップ済み
	isSetUpRingAnim_ = true;
}

void SpearAnimManager::StopSpearAnimationAll()
{
	// 全アニメーション分ループ
	for (int i = 0; i < SpearAnim::SpearAnimCount; i++) {
		spearAnim_.StopAnimation(i);
	}
}

void SpearAnimManager::StopRingAnimationAll()
{
	// 全アニメーション分ループ
	for (int i = 0; i < RingAnim::RingAnimCount; i++) {
		ringAnim_.StopAnimation(i);
	}
}
