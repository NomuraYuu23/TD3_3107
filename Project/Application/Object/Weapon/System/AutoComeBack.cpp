#include "AutoComeBack.h"

void AutoComeBack::Initialize(Weapon* weapon)
{
	weapon_ = weapon;

}

void AutoComeBack::Update()
{
	if (!endTimer_.IsActive()) {
		return;
	}

	endTimer_.Update();
}

void AutoComeBack::Start(float endFrame)
{
	endTimer_.Start(endFrame);
}
