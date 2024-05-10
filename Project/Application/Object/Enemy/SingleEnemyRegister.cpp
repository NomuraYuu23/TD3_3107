#include "SingleEnemyRegister.h"

void SingleEnemyRegister::Initialize(Model* model)
{
	LargeNumberOfObjects::Initialize(model);

}

void SingleEnemyRegister::Update()
{

	LargeNumberOfObjects::Update();

	// フラグによる死亡処理
	objects_.remove_if([this](std::unique_ptr<OneOfManyObjects>& enemy) {
		if (enemy->IsDead()) {
			enemy.reset();
			return true;
		}
		return false;
		});

}
