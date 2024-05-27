#include "MapManager.h"
#include <fstream>

#include "../IObject.h"
#include "../ObjectList.h"
#include "../GameUtility/MathUtility.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../AllSceneObject/StageNumberManager.h"

void MapManager::Initialize(Model* model)
{

	LargeNumberOfObjects::Initialize(model);

	//InitializePlacement();
	//InitializeBossMap();
	//InitializeLongPatternMap();

	mapEditor_ = std::make_unique<MapEditor>();
	mapEditor_->LoadFiles();

}

void MapManager::Update()
{

	LargeNumberOfObjects::Update();

	EditorMapLoad();

}

void MapManager::ImGuiDraw()
{

	//ImGui::Begin("BlockManager");
	//// リストの最大値
	//int size = (int)objects_.size();
	//ImGui::InputInt("maxBlockSize", &size);

	//ImGui::Separator();

	//// ブロック達のImGui
	//for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = objects_.begin();
	//	it != objects_.end(); ++it) {
	//	static_cast<Terrain*>(it->get())->ImGuiDraw();
	//}

	//ImGui::End();

	mapEditor_->ImGuiDraw();



}

void MapManager::CollisionRegister(Collision2DManager* collisionManager, const BaseCamera& camera)
{
	for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		float range = 100.0f;


		//if (!MathUtility::CheckOutScreen((*it)->GetWorldPosition(), {100.0f,500.0f}, camera)) {
			if (static_cast<Terrain*>(it->get())->typeNumber_ != Terrain::BlockType::kNone) {
				collisionManager->ListRegister(&static_cast<Terrain*>((it->get()))->boxCollider_);
			}
		//}

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

void MapManager::RegisterBlock(const Vector3& position, const Vector2& scale , const std::string& name)
{

	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Terrain>();
	obj->Initialize();
	obj->transform_.translate = position;
	obj->transform_.scale = { scale.x,scale.y,1.0f };
	// サイズの設定
	static_cast<Terrain*>(obj.get())->scale2D_ = { scale.x * 2.0f, scale.y * 2.0f };
	// タイプの設定
	static_cast<Terrain*>(obj.get())->typeNumber_ = Terrain::BlockType::kTerrain;
	// マテリアル更新（サイズの変更後に合わせて）
	static_cast<Terrain*>(obj.get())->MaterialUpdate();

	// 名前
	static_cast<Terrain*>(obj.get())->name_ = name;

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

void MapManager::RegisterWallBlock(const Vector3& position)
{
	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Terrain>();
	obj->Initialize();
	obj->transform_.translate = position;
	// タイプの設定
	static_cast<Terrain*>(obj.get())->typeNumber_ = Terrain::BlockType::kWall;
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
	typeNum = static_cast<uint32_t>(Terrain::BlockType::kWall);
	for (int i = 0; i < 32; ++i) {
		// 左
		position = Vector3({ 0.0f,-4.0f + ((float)i * blockSize),0 });
		(this->*registerFuncs[typeNum])(position);
		// 右
		position = Vector3({ (float)kMaxXNum * blockSize, -4.0f + ((float)i * blockSize),0 });
		(this->*registerFuncs[typeNum])(position);
	}


	// 最初の段差
	typeNum = static_cast<uint32_t>(Terrain::BlockType::kTerrain);
	for (int y = 0; y < 2; ++y) {
		for (int x = 0; x < 44; ++x) {
			position = Vector3({ (float)(kMaxXNum - 44) * blockSize + (float)x * blockSize, -2.0f + ((float)y * blockSize),0 });
			// 当たり判定に追加（左端
			if (x == 0) {
				(this->*registerFuncs[static_cast<uint32_t>(Terrain::BlockType::kTerrain)])(position);
			}
			// 下の段は登録しない
			else if (y != 0) {
				(this->*registerFuncs[static_cast<uint32_t>(Terrain::BlockType::kTerrain)])(position);
			}
			// 上の段は登録
			else {
				(this->*registerFuncs[static_cast<uint32_t>(Terrain::BlockType::kNone)])(position);
			}
		}
	}

	// 次の段差
	//for (int y = 0; y < 2; ++y) {
	//	for (int x = 0; x < 30; ++x) {
	//		position = Vector3({ (float)(kMaxXNum - 30) * blockSize + (float)x * blockSize, 2.0f + ((float)y * blockSize),0 });
	//		
	//		if (x == 0) {
	//			(this->*registerFuncs[static_cast<uint32_t>(Terrain::BlockType::kTerrain)])(position);
	//		}
	//		else if (y == 0) {
	//			(this->*registerFuncs[static_cast<uint32_t>(Terrain::BlockType::kNone)])(position);
	//		}
	//		else {
	//			(this->*registerFuncs[static_cast<uint32_t>(Terrain::BlockType::kTerrain)])(position);
	//		}
	//	}
	//}

#pragma endregion

//#pragma region リスの周辺
//	// 上床
//	for (int i = 0; i < 57; ++i) {
//		position = { blockSize * 4.0f + (float)i * blockSize,6.0f + (17 * blockSize),0 };
//		(this->*registerFuncs[typeNum])(position);
//	}
//
//	typeNum = static_cast<uint32_t>(Terrain::BlockType::kWall);
//	// 壁じゃん
//	for (int i = 0; i < 18; ++i) {
//		position = { blockSize * 4.0f,6.0f + ((float)i * blockSize),0 };
//		(this->*registerFuncs[typeNum])(position);
//	}
//	// 壁じゃん
//	for (int i = 0; i < 12; ++i) {
//		position = { blockSize * 4.0f + blockSize * 56 ,16.0f + ((float)i * blockSize),0 };
//		(this->*registerFuncs[typeNum])(position);
//	}
//
//#pragma endregion

#pragma region 空中の障害物
	RegisterBlock({ 20.0f,4.0f,0 }, { 20.0f,1.0f }, "Terrain12");

	RegisterBlock({ 35.0f,8.0f,0 }, { 20.0f,1.0f }, "Terrain13");

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

void MapManager::InitializeLongPatternMap()
{
	// スケールの値が半径

	//// 床
	//RegisterBlock({ 20.0f,-4.0f,0 }, { 100.0f,1.0f });

	//// 縦壁
	////RegisterBlock({ 0.0f,30.0f - 4.0f,0 }, { 1.0f,30.0f });

	//RegisterBlock({ 120.0f - 1.0f,30.0f - 4.0f,0 }, { 1.0f,30.0f });

	// 床
	RegisterBlock({ 20.0f,-4.0f,0 }, { 100.0f,2.0f }, "Terrain0");

	// 真ん中の縦
	// 右
	RegisterBlock({ 50.0f,28.0f,0 }, { 2.0f,30.0f }, "Terrain1");
	// 左
	RegisterBlock({ 40.0f,40.0f,0 }, { 2.0f,30.0f }, "Terrain2");

	// 上の床
	RegisterBlock({ 87.0f,56.0f,0 }, { 35.0f,2.0f }, "Terrain3");


	// ブロック
	RegisterBlock({ 17.5f,12.5f,0 }, { 6.0f,1.0f }, "Terrain4");

	// 右下の奴ら
	RegisterBlock({ 90.0f,8.0f,0 }, { 6.0f,1.0f }, "Terrain5");
	RegisterBlock({ 110.0f,16.0f,0 }, { 6.0f,1.0f }, "Terrain6");

	RegisterBlock({ 90.0f,24.0f,0 }, { 6.0f,1.0f }, "Terrain7");
	RegisterBlock({ 110.0f,32.0f,0 }, { 6.0f,1.0f }, "Terrain8");


	// 左端縦
	RegisterBlock({ -78.0f,50.0f - 2.0f,0 }, { 2.0f,50.0f }, "Terrain9");
	// 右端壁
	RegisterBlock({ 120.0f - 1.0f,50.0f - 2.0f,0 }, { 1.0f,50.0f }, "Terrain10");

	// 天井
	RegisterBlock({ 20.0f,98.0f,0 }, { 100.0f,2.0f }, "Terrain11");




}

void MapManager::EditorMapLoad()
{

	// マップデータ
	std::map<std::string, std::map<std::string, MapBlockData>>* mapDatas = mapEditor_->GetDatas();
	
	// ステージ
	std::string stageName = "Stage" + std::to_string(StageNumberManager::stageNum_);

	if (StageNumberManager::stageNum_ < 10) {
		stageName = "Stage0" + std::to_string(StageNumberManager::stageNum_);
	}

	for (std::map<std::string, std::map<std::string, MapBlockData>>::iterator stageItr = mapDatas->begin();
		stageItr != mapDatas->end(); ++stageItr) {

		// ステージ番号が違う
		if (stageItr->first != stageName) {
			continue;
		}

		std::vector<std::string> terrainNames;

		for (std::map<std::string, MapBlockData>::iterator terrainItr = stageItr->second.begin();
			terrainItr != stageItr->second.end(); ++terrainItr) {

			std::string terrainName = terrainItr->first;
			MapBlockData terrainData = terrainItr->second;
			bool edited = false;

			// 編集
			for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = objects_.begin();
				it != objects_.end(); ++it) {

				// ブロックの番号が違う
				if (terrainName != static_cast<Terrain*>(it->get())->name_) {
					continue;
				}

				it->get()->transform_.translate = { terrainData.position.x, terrainData.position.y, 0.0f };
				it->get()->transform_.scale = { terrainData.size.x,terrainData.size.y,1.0f };
				// サイズの設定
				static_cast<Terrain*>(it->get())->scale2D_ = { terrainData.size.x * 2.0f, terrainData.size.y * 2.0f };
				// マテリアル更新（サイズの変更後に合わせて）
				static_cast<Terrain*>(it->get())->MaterialUpdate();

				edited = true;

				break;
			
			}

			// 追加
			if (!edited) {
				RegisterBlock({ terrainData.position.x, terrainData.position.y, 0.0f }, terrainData.size, terrainName);
			}

			// 削除用に名前登録
			terrainNames.push_back(terrainName);

		}

		// 削除
		objects_.remove_if([=](std::unique_ptr<OneOfManyObjects>& terrain) {
			
			for (uint32_t i = 0; i < terrainNames.size(); ++i) {
				if (terrainNames[i] == static_cast<Terrain*>(terrain.get())->name_) {
					return false;
				}
			}
			return true;

			});

	}

}
