#include "MapManager.h"

#include "../IObject.h"
#include "../ObjectList.h"
#include "../GameUtility/MathUtility.h"
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
	// リストの最大値
	int size = (int)objects_.size();
	ImGui::InputInt("maxBlockSize", &size);

	ImGui::Separator();
	// ブロック追加
	if (ImGui::Button("RegisterBlock")) {
		RegisterBlock();
	}

	//ImGui::Separator();

	//// ブロック達のImGui
	//for (std::list<OneOfManyObjects*>::iterator it = objects_.begin();
	//	it != objects_.end(); ++it) {
	//	static_cast<Terrain*>((*it))->ImGuiDraw();
	//}

	ImGui::End();

}

void MapManager::CollisionRegister(Collision2DManager* collisionManager, const BaseCamera& camera)
{
	for (std::list<OneOfManyObjects*>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		float range = 100.0f;
		if (!MathUtility::CheckOutScreen((*it)->GetWorldPosition(), range, camera)) {
			collisionManager->ListRegister(&static_cast<Terrain*>((*it))->boxCollider_);
		}

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

	float blockSize = 2.0f;

#pragma region 床ブロック

	for (int i = 0; i < 30; ++i) {
		// 床
		RegisterBlock({ (float)i * blockSize ,-4.0f,0 });
	}

#pragma endregion

#pragma region 初期値から左方向の部分と右の壁

	for (int i = 0; i < 20; ++i) {
		// 左の壁
		RegisterBlock({ 0.0f,-4.0f + ((float)i * blockSize),0 });
	}

	for (int i = 0; i < 14; ++i) {
		// 右の壁
		RegisterBlock({ 30.0f * blockSize,-4.0f + ((float)i * blockSize),0 });
	}

	// 壁じゃん用
	for (int i = 0; i < 8; ++i) {
		RegisterBlock({ 24.0f * blockSize, 8.0f + ((float)i * blockSize),0 });
	}

	// 壁上
	for (int i = 0; i < 35; ++i) {
		RegisterBlock({ 30.0f * blockSize +((float)i * blockSize), 8.0f + (7 * blockSize),0});
	}

	// 右端の壁
	for (int i = 0; i < 20; ++i) {
		RegisterBlock({ 64.0f * blockSize, ((12.0f + (float)i) * blockSize),0 });
	}

	const int kMaxXNum = 64;
	const int kMaxYNum = 31;


	// 上noY座標 8.0f + (7 * 2.0f)
	// 端noX座標 65.0f * 2.0f

	// 上の段差
	for (int i = 0; i < 4; ++i) {
		RegisterBlock({ 40.0f * blockSize + ((float)i * blockSize), 8.0f + (8 * blockSize),0 });
	}

	for (int i = 0; i < 4; ++i) {
		RegisterBlock({ 50.0f * blockSize + ((float)i * blockSize), 8.0f + (8 * blockSize),0 });
	}

#pragma endregion


}

void MapManager::InitializeBossMap()
{
	float blockSize = 2.0f;

}
