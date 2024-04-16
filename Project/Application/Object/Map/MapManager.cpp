#include "MapManager.h"

#include "../IObject.h"
#include "../ObjectList.h"
#include "../GameUtility/MathUtility.h"
#include "../../../Engine/2D/ImguiManager.h"

void MapManager::Initialize(Model* model)
{

	LargeNumberOfObjects::Initialize(model);

	InitializePlacement();
	//InitializeBossMap();
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
	for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		float range = 100.0f;


		if (!MathUtility::CheckOutScreen((*it)->GetWorldPosition(), {100.0f,500.0f}, camera)) {
			collisionManager->ListRegister(&static_cast<Terrain*>((it->get()))->boxCollider_);
		}

	}
}

void MapManager::RegisterBlock(const Vector3& position)
{

	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Terrain>();
	obj->Initialize();
	obj->transform_.translate = position;
	// 追加
	objects_.push_back(std::move(obj));

}

void MapManager::RegisterBlock(const Vector3& position, const Vector2 scale)
{

	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Terrain>();
	obj->Initialize();
	obj->transform_.translate = position;
	obj->transform_.scale = { scale.x,scale.y,1.0f };
	static_cast<Terrain*>(obj.get())->scale2D_ = scale;
	// 追加
	objects_.push_back(std::move(obj));
}

void MapManager::RegisterTerrainBlock(const Vector3& position)
{
	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Terrain>();
	obj->Initialize();
	obj->transform_.translate = position;
	// タイプの設定
	static_cast<Terrain*>(obj.get())->typeNumber_ = Terrain::BlockType::kTerrain;
	// 追加
	objects_.push_back(std::move(obj));

}

void MapManager::RegisterObstacleBlock(const Vector3& position)
{
	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Terrain>();
	obj->Initialize();
	obj->transform_.translate = position;
	// タイプの設定
	static_cast<Terrain*>(obj.get())->typeNumber_ = Terrain::BlockType::kObstacle;
	// 追加
	objects_.push_back(std::move(obj));
}

void MapManager::InitializePlacement()
{

	float blockSize = 2.0f;

	const int kMaxXNum = 64;
	const int kMaxYNum = 31;

	// 上noY座標 8.0f + (7 * 2.0f)
	// 端noX座標 65.0f * 2.0f

	uint32_t typeNum = static_cast<uint32_t>(Terrain::BlockType::kTerrain);
	Vector3 position = {};

#pragma region 周りのブロック
	// 床・天井
	for (int i = 0; i < 64; ++i) {
		// 床
		position = Vector3({ (float)i * blockSize,-4.0f,0 });
		(this->*registerFuncs[typeNum])(position);
		// 天
		position = Vector3({ (float)i * blockSize, 58.0f,0 });
		(this->*registerFuncs[typeNum])(position);
	}

	// 端壁
	for (int i = 0; i < 32; ++i) {
		// 左
		position = Vector3({ 0.0f,-4.0f + ((float)i * blockSize),0 });
		(this->*registerFuncs[typeNum])(position);
		// 右
		position = Vector3({ (float)kMaxXNum * blockSize, -4.0f + ((float)i * blockSize),0 });
		(this->*registerFuncs[typeNum])(position);
	}


	// 最初の段差
	for (int y = 0; y < 2; ++y) {
		for (int x = 0; x < 44; ++x) {
			position = Vector3({ (float)(kMaxXNum - 44) * blockSize + (float)x * blockSize, -2.0f + ((float)y * blockSize),0 });
			(this->*registerFuncs[typeNum])(position);
		}
	}

	// 次の段差
	for (int y = 0; y < 2; ++y) {
		for (int x = 0; x < 30; ++x) {
			position = Vector3({ (float)(kMaxXNum - 30) * blockSize + (float)x * blockSize, 2.0f + ((float)y * blockSize),0 });
			(this->*registerFuncs[typeNum])(position);
		}
	}

#pragma endregion

#pragma region リスの周辺
	// 壁じゃん
	for (int i = 0; i < 18; ++i) {
		position = { blockSize * 4.0f,6.0f + ((float)i * blockSize),0 };
		(this->*registerFuncs[typeNum])(position);
	}
	// 上床
	for (int i = 0; i < 57; ++i) {
		position = { blockSize * 4.0f + (float)i * blockSize,6.0f + (17 * blockSize),0 };
		(this->*registerFuncs[typeNum])(position);
	}

	// 壁じゃん
	for (int i = 0; i < 12; ++i) {
		position = { blockSize * 4.0f + blockSize * 56 ,16.0f + ((float)i * blockSize),0 };
		(this->*registerFuncs[typeNum])(position);
	}

#pragma endregion

#pragma region 空中の障害物
	typeNum = static_cast<uint32_t>(Terrain::BlockType::kObstacle);
	for (int i = 0; i < 4; ++i) {
		(this->*registerFuncs[typeNum])(Vector3({ (40 * blockSize) + (float)i * blockSize ,9 * blockSize,0 }));
		(this->*registerFuncs[typeNum])(Vector3({ (50 * blockSize) + (float)i * blockSize ,9 * blockSize,0 }));
	}
#pragma endregion


//#pragma region 最初の壁じゃんのとこいら
//
//	for (int i = 0; i < 14; ++i) {
//		// 右の壁
//		RegisterBlock({ 30.0f * blockSize,-4.0f + ((float)i * blockSize),0 });
//	}
//
//	// 壁じゃん用
//	for (int i = 0; i < 8; ++i) {
//		RegisterBlock({ 24.0f * blockSize, 8.0f + ((float)i * blockSize),0 });
//	}
//
//	// 壁上の床
//	for (int i = 0; i < 35; ++i) {
//		RegisterBlock({ 30.0f * blockSize +((float)i * blockSize), 8.0f + (7 * blockSize),0});
//	}
//
//	// 上の段差
//	for (int i = 0; i < 4; ++i) {
//		RegisterBlock({ 40.0f * blockSize + ((float)i * blockSize), 8.0f + (8 * blockSize),0 });
//	}
//
//	for (int i = 0; i < 4; ++i) {
//		RegisterBlock({ 50.0f * blockSize + ((float)i * blockSize), 8.0f + (8 * blockSize),0 });
//	}
//
//#pragma endregion
//

}

void MapManager::InitializeBossMap()
{

	float blockSize = 2.0f;

	const int kMaxXNum = 64;
	const int kMaxYNum = 31;

#pragma region 床ブロック

	//for (int i = 0; i < 10; ++i) {
	//	// 床
	//	RegisterBlock({ (float)i * blockSize ,-4.0f,0 });
	//}
	// 床・天井
	for (int i = 0; i < 64; ++i) {
		// 床
		RegisterBlock({ (float)i * blockSize,-4.0f,0 });
		// 天
		RegisterBlock({ (float)i * blockSize, 58.0f,0 });
	}

	// 端壁
	for (int i = 0; i < 32; ++i) {
		// 左
		RegisterBlock({ 0.0f,-4.0f + ((float)i * blockSize),0 });
		// 右
		RegisterBlock({ (float)kMaxXNum * blockSize, -4.0f + ((float)i * blockSize),0 });
	}

#pragma endregion

}
