#pragma once
#include <list>
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Collider2D/Box.h"
#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../../../Engine/3D/LargeNumberOfObjects.h"

class EnemyManager
{
private:
	//enum class EnemyType : uint32_t
	//{
	//	eProximity,	// 近接タイプ
	//	eRemote,	// 遠隔タイプ
	//};

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

	void Draw(BaseCamera& camera, std::vector<UINT>* textureHnadles);

	/// <summary>
	/// ImGUi
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// コライダーに登録する処理
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionRegister(Collision2DManager* collisionManager, const BaseCamera& camera);

	std::list<std::unique_ptr<LargeNumberOfObjects>>* GetEmitterLists() { return &enemyEmitters_; }

private:
	/// <summary>
	/// 敵の追加
	/// </summary>
	void RegisterEnemy(const Vector3& position, uint32_t typeNum);

	void CreateSingleEnemy();

	void CreateEmitter(const Vector3& position, float distance, uint32_t enemyMaxCount);

	Vector3 resPoint_ = {};
	
	// エミッター単位で敵を管理
	std::list<std::unique_ptr<LargeNumberOfObjects>> enemyEmitters_;

	// 単体管理のエネミーたちをまとめる
	std::unique_ptr<LargeNumberOfObjects> singleEnemys_;

	// 全体のモデル
	Model* model_;
};
