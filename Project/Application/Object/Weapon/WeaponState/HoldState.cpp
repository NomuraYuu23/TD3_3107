#include "HoldState.h"
#include "../Weapon.h"

void HoldState::Initialize()
{
	// 親子付け
	weapon_->SettingParent();

}
