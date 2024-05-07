#include "IEnemyEmitter.h"

uint32_t IEnemyEmitter::sSerialNumber_ = 0;

void IEnemyEmitter::Initialize(Model* model)
{
	LargeNumberOfObjects::Initialize(model);

	worldTransform_.Initialize();
}

void IEnemyEmitter::Update()
{
	// リストの更新処理
	LargeNumberOfObjects::Update();
}

void IEnemyEmitter::CreateEnemy(const Vector3& transformPosition, float distance, uint32_t enemyCount)
{

	worldTransform_.transform_.translate = transformPosition;
	distance_ = distance;
	maxCount_ = enemyCount;

}
