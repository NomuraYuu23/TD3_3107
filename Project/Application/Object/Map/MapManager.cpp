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

void MapManager::RegisterBlock()
{
	//IObject* newBlock =
	std::unique_ptr<IObject> newBlock;
	newBlock = std::make_unique<Terrain>();
	newBlock->Initialize(terrainModel_);
	// 追加
	blocks_.push_back(std::move(newBlock));
}