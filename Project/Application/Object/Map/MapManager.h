#pragma once
#include <list>
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Collider2D/Box.h"
#include "../../../Engine/Collision2D/Collision2DManager.h"


class IObject;

class MapManager
{

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const BaseCamera& camera);
	/// <summary>
	/// ImGUi
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// コライダーに登録する処理
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionRegister(Collision2DManager* collisionManager);

private:
	/// <summary>
	/// ブロックの追加
	/// </summary>
	void RegisterBlock();
	void RegisterBlock(const Vector3& position);
	void RegisterBlock(const Vector3& position, const Vector2 scale);

	void InitializePlacement();

private:
	// 地形ブロック
	Model* terrainModel_;
	// ブロックのリスト
	std::list<std::unique_ptr<IObject>> blocks_;

};

