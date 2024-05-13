#pragma once
#include <list>
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Collider2D/Box.h"
#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../../../Engine/3D/LargeNumberOfObjects.h"

class EnemyManager : public LargeNumberOfObjects
{
private:
	enum class EnemyType : uint32_t
	{
		eProximity,	// 近接タイプ
		eRemote,	// 遠隔タイプ
	};

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

	//void Draw(BaseCamera& camera, std::vector<UINT>* textureHnadles);

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
	/// 敵の追加
	/// </summary>
	void RegisterEnemy(const Vector3& position, uint32_t typeNum);
	void RegisterEnemy(const Vector3& offset, uint32_t typeNum, WorldTransform* parent);

	struct EnemyEmitter {
		WorldTransform worldTransform;
		float distance;
		uint32_t maxCount;
	};
	std::list<EnemyEmitter> enemyEmitters_;
	WorldTransform testParent;

	WorldTransform emitters_[3];

	std::list<WorldTransform> enemyEmitterTest_;

	Vector3 resPoint_ = {};

};
