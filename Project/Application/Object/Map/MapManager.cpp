#include "MapManager.h"

#include "../IObject.h"
#include "../ObjectList.h"
#include "../../../Engine/2D/ImguiManager.h"

void MapManager::Initialize(Model* model)
{

	LargeNumberOfObjects::Initialize(model);

	InitializePlacement();
}

void MapManager::Update()
{

	LargeNumberOfObjects::Update();

}

void MapManager::ImGuiDraw()
{

	ImGui::Begin("BlockManager");

	if (ImGui::Button("RegisterBlock")) {
		RegisterBlock();
	}

	ImGui::Separator();

	// ブロック達のImGui
	for (std::list<OneOfManyObjects*>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		static_cast<Terrain*>((*it))->ImGuiDraw();
	}

	ImGui::End();

}

void MapManager::CollisionRegister(Collision2DManager* collisionManager)
{
	for (std::list<OneOfManyObjects*>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		collisionManager->ListRegister(&static_cast<Terrain*>((*it))->boxCollider_);
	}
}

void MapManager::RegisterBlock()
{
	//IObject* newBlock =
	OneOfManyObjects* obj = new Terrain();
	obj->Initialize();
	// 追加
	objects_.push_back(std::move(obj));
}

void MapManager::RegisterBlock(const Vector3& position)
{
	OneOfManyObjects* obj = new Terrain();
	obj->Initialize();
	obj->transform_.translate = position;
	// 追加
	objects_.push_back(std::move(obj));

}

void MapManager::RegisterBlock(const Vector3& position, const Vector2 scale)
{
	OneOfManyObjects* obj = new Terrain();
	obj->Initialize();
	obj->transform_.translate = position;
	obj->transform_.scale = { scale.x,scale.y,1.0f };
	static_cast<Terrain*>(obj)->scale2D_ = scale;
	// 追加
	objects_.push_back(std::move(obj));
}

void MapManager::InitializePlacement()
{
	//// 横（地面
	//for (int i = 0; i < 30; i++) {
	//	RegisterBlock({ 16.0f - 2.0f * (float)i,-4.0f,0 });
	//}
	//// 縦壁
	//for (int i = 0; i < 8; i++) {
	//	RegisterBlock({ 8.0f, 2.0f * (float)i, 0 });
	//}
	//for (int i = 0; i < 8; i++) {
	//	RegisterBlock({ 10.0f, 2.0f * (float)i, 0 });
	//}

	//// ２段目
	//for (int i = 0; i < 8; i++) {
	//	RegisterBlock({ 2.0f + 2.0f * (float)i,-2.0f,0 });
	//}

#pragma region 床ブロック
	for (int i = 0; i < 30; ++i) {
		RegisterBlock({ (float)i * 2.0f ,-4.0f,0 });
	}

	//RegisterBlock({ 0,-4.0f,0 }, { 40.0f,1.0f });

#pragma endregion

#pragma region 初期値から左方向の部分と右の壁

	for (int i = 0; i < 14; ++i) {
		// 左の壁
		RegisterBlock({ 0.0f,-4.0f + ((float)i * 2.0f),0 });
		// 右の壁
		RegisterBlock({ 30.0f * 2.0f,-4.0f + ((float)i * 2.0f),0 });
	}

	// 壁じゃん用
	for (int i = 0; i < 8; ++i) {
		RegisterBlock({ 24.0f * 2.0f, 8.0f + ((float)i * 2.0f),0 });
	}

	for (int i = 0; i < 6; ++i) {
		RegisterBlock({ 30.0f * 2.0f +((float)i * 2.0f), 8.0f + (7 * 2.0f),0});
	}

#pragma endregion


}
