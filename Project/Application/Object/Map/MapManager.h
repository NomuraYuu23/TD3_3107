#pragma once
#include <list>
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Collider2D/Box.h"

class IObject;

class MapManager
{

public:
	void Initialize(Model* model);
	void Update();
	void Draw(BaseCamera camera);
	void ImGuiDraw();

private:
	void RegisterBlock();

private:
	// 地形ブロック
	Model* terrainModel_;
	// ブロックのリスト
	std::list<std::unique_ptr<IObject>> blocks_;

};

