#pragma once
#include <list>
#include "Terrain.h"
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Collider2D/Box.h"
#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../../../Engine/3D/LargeNumberOfObjects.h"

class MapManager : public LargeNumberOfObjects
{

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGUi
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// コライダーに登録する処理
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionRegister(Collision2DManager* collisionManager, const BaseCamera& camera);

private:
	/// <summary>
	/// ブロックの追加
	/// </summary>
	void RegisterBlock(const Vector3& position);
	void RegisterBlock(const Vector3& position, const Vector2 scale);

	/// <summary>
	/// カメラに影響を与えるブロック登録
	/// </summary>
	/// <param name="position"></param>
	void RegisterTerrainBlock(const Vector3& position);
	/// <summary>
	/// カメラに影響を与えないブロック登録
	/// </summary>
	/// <param name="position"></param>
	void RegisterObstacleBlock(const Vector3& position);

	/// <summary>
	/// 壁
	/// </summary>
	/// <param name="position"></param>
	void RegisterWallBlock(const Vector3& position);

	/// <summary>
	/// とりあえずのマップ
	/// </summary>
	void InitializePlacement();

	/// <summary>
	/// ボス戦用のマップ配置
	/// </summary>
	void InitializeBossMap();

	/// <summary>
	/// マップチップ使わない法
	/// </summary>
	void InitializeLongPatternMap();


	void LoadMapData(const std::string& filePath);

private:
	// マップのオブジェクト登録用の関数ポインタ
	void(MapManager::* registerFuncs[static_cast<uint32_t>(Terrain::BlockType::kMaxSize)])(const Vector3&) = {
		&MapManager::RegisterBlock,
		&MapManager::RegisterTerrainBlock,
		&MapManager::RegisterObstacleBlock,
		&MapManager::RegisterWallBlock
	};

};

