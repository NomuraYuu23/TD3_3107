#include "EnemyManager.h"
#include "../ObjectList.h"
#include "../GameUtility/MathUtility.h"
#include "State/EnemyStateList.h"

void EnemyManager::Initialize(Model* model)
{
	LargeNumberOfObjects::Initialize(model);


}

void EnemyManager::Update()
{

	LargeNumberOfObjects::Update();

}

void EnemyManager::ImGuiDraw()
{
	ImGui::Begin("EnemyManager");

	if (ImGui::Button("RegisterMeleeBlock")) {
		//RegisterBlock();
		RegisterEnemy(resPoint_, 0);
	}
	if (ImGui::Button("RegisterRangedBlock")) {
		//RegisterBlock();
		RegisterEnemy(resPoint_, 1);
	}

	ImGui::DragFloat3("pos", &resPoint_.x, 0.01f, -100.0f, 100.0f);

	ImGui::Separator();

	// ブロック達のImGui
	for (std::list<OneOfManyObjects*>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		static_cast<Enemy*>((*it))->ImGuiDraw();
	}

	ImGui::End();
}

void EnemyManager::CollisionRegister(Collision2DManager* collisionManager, const BaseCamera& camera)
{
	for (std::list<OneOfManyObjects*>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		//float range = 100.0f;
		//if (!MathUtility::CheckOutScreen((*it)->GetWorldPosition(), range, camera)) {
			collisionManager->ListRegister(&static_cast<Enemy*>((*it))->boxCollider_);
		//}

	}
}

void EnemyManager::RegisterEnemy(const Vector3& position, uint32_t typeNum)
{
	OneOfManyObjects* obj = new Enemy();
	obj->Initialize();
	obj->transform_.translate = position;
	// 初期化
	static_cast<Enemy*>(obj)->StateInitialize(std::make_unique<EnemyGroundState>(), typeNum);
	
	// 追加
	objects_.push_back(std::move(obj));


}
