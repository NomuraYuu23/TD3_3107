#include "ReturnWaitState.h"

void ReturnWaitState::Initialize()
{

	// ステート更新
	SetNowState(this);

	endTimer_.Start(150.0f);
}

void ReturnWaitState::Update()
{

	// タイマー更新
	endTimer_.Update();

}

void ReturnWaitState::ImGuiUpdate()
{



}
