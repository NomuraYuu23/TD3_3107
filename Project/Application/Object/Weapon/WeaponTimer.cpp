//#include "WeaponTimer.h"
//
//void WeaponTimer::StartTimer(int endFrame)
//{
//	// カウント用変数
//	coolTimer_ = 0;
//	// 終了フレーム
//	endFrame_ = endFrame;
//	// 開始フラグ
//	isStart_ = true;
//}
//
//void WeaponTimer::UpdateTimer()
//{
//	if (isStart_) {
//		coolTimer_ += (1.0f / endFrame_);
//	}
//
//	if (coolTimer_ >= 1.0f) {
//		isEnd_ = true;
//	}
//}