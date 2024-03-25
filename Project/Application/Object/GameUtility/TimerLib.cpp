#include "TimerLib.h"

void TimerLib::StartSetting()
{
	// リセット
	isEnd_ = false;
	nowFrame_ = 0;
	// 開始
	isActive_ = true;
}

void TimerLib::Start(float endFrame)
{
	// 初期化
	StartSetting();

	// 終了フレーム
	endFrame_ = endFrame;

}

void TimerLib::End()
{
	isEnd_ = true;
	isActive_ = false;
}

void TimerLib::Update()
{

	if (isActive_) {

		if (nowFrame_ >= 1.0f) {
			//isEnd_ = true;
			//isActive_ = false;
			End();
			return;
		}

		this->nowFrame_ += (1.0f / endFrame_);

	}
}
