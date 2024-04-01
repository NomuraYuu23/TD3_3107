#pragma once
#include <list>
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
	void RegisterBlock();
	void RegisterBlock(const Vector3& position);
	void RegisterBlock(const Vector3& position, const Vector2 scale);

	void InitializePlacement();

};

