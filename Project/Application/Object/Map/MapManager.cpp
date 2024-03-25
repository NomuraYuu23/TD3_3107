#include "MapManager.h"

#include "../IObject.h"
#include "../ObjectList.h"
#include "../../../Engine/2D/ImguiManager.h"

void MapManager::Initialize(Model* model)
{
	terrainModel_ = model;
	for (std::list<std::unique_ptr<IObject>>::iterator it = blocks_.begin();
		it != blocks_.end();++it) {
		(*it)->Initialize(terrainModel_);
	}
	InitializePlacement();
}

void MapManager::Update()
{
	for (std::list<std::unique_ptr<IObject>>::iterator it = blocks_.begin();
		it != blocks_.end(); ++it) {
		(*it)->Update();
	}

}

void MapManager::Draw(BaseCamera camera)
{
	for (std::list<std::unique_ptr<IObject>>::iterator it = blocks_.begin();
		it != blocks_.end(); ++it) {
		(*it)->Draw(camera);
	}
}

void MapManager::ImGuiDraw()
{

	ImGui::Begin("BlockManager");

	if (ImGui::Button("RegisterBlock")) {
		RegisterBlock();
	}

	ImGui::Separator();

	// ブロック達のImGui
	for (std::list<std::unique_ptr<IObject>>::iterator it = blocks_.begin();
		it != blocks_.end(); ++it) {
		(*it)->ImGuiDraw();
	}

	ImGui::End();

}

void MapManager::CollisionRegister(Collision2DManager* collisionManager)
{
	for (std::list<std::unique_ptr<IObject>>::iterator it = blocks_.begin();
		it != blocks_.end(); ++it) {
		collisionManager->ListRegister(&(*it)->boxCollider_);
	}
}

void MapManager::RegisterBlock()
{
	//IObject* newBlock =
	std::unique_ptr<IObject> newBlock;
	newBlock = std::make_unique<Terrain>();
	newBlock->Initialize(terrainModel_);
	// 追加
	blocks_.push_back(std::move(newBlock));
}

void MapManager::RegisterBlock(const Vector3& position)
{
	std::unique_ptr<IObject> newBlock;
	newBlock = std::make_unique<Terrain>();
	newBlock->Initialize(terrainModel_);
	newBlock->worldtransform_.transform_.translate = position;
	// 追加
	blocks_.push_back(std::move(newBlock));

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
	for (int i = 0; i < 60; ++i) {
		RegisterBlock({ (float)i * 2.0f ,-4.0f,0 });
	}
#pragma endregion

#pragma region 初期値から左方向の部分と右の壁
	RegisterBlock({ -2.0f,-4.0f,0 });

	for (int i = 0; i < 10; ++i) {
		RegisterBlock({ -4.0f,-4.0f + ((float)i * 2.0f),0 });
		RegisterBlock({ 60.0f * 2.0f,-4.0f + ((float)i * 2.0f),0 });
	}

#pragma endregion


}
