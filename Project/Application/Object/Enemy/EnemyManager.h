#pragma once
#include <list>
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Collider2D/Box.h"
#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../../../Engine/3D/LargeNumberOfObjects.h"
#include "../../../Engine/Animation/Animation.h"
#include "SingleEnemyData.h"
#include "MultiEnemyData.h"
#include "ChaseEnemyData.h"
#include "EnemyEditor.h"

class Player;

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
	void Initialize(Model* model, std::vector<UINT>* texture, std::vector<UINT>* chaseTexture);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="textureHnadles"></param>
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

	/// <summary>
	/// LargeNumberOfObjectsのリスト
	/// </summary>
	/// <returns></returns>
	std::list<std::unique_ptr<LargeNumberOfObjects>>* GetEmitterLists() { return &enemyEmitters_; }

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

public:
	/// <summary>
	/// ロードエネミーデータ
	/// </summary>
	void LoadEnemyData();

private:
	/// <summary>
	/// 敵の追加
	/// </summary>
	void RegisterEnemy(const SingleEnemyData& data);
	void RegisterEnemy(const SingleEnemyData& data, const std::string& name);


	void RegisterChaseEnemy(const ChaseEnemyData& data, const std::string& name);

	/// <summary>
	/// 単体の敵を生成している場所
	/// </summary>
	void CreateSingleEnemy(std::vector<UINT>* texture, std::vector<UINT>* chaseTexture);
	/// <summary>
	/// エミッター生成関数
	/// </summary>
	/// <param name="position">エミッターの座標</param>
	/// <param name="distance">各敵との距離</param>
	/// <param name="enemyMaxCount">敵の数</param>
	void CreateEmitter(const MultiEnemyData& data);
	void CreateEmitter(const MultiEnemyData& data, const std::string& name);

	Vector3 resPoint_ = {};

	// エミッター単位で敵を管理
	std::list<std::unique_ptr<LargeNumberOfObjects>> enemyEmitters_;

	// 単体管理のエネミーたちをまとめる
	std::unique_ptr<LargeNumberOfObjects> singleEnemys_;

	// 追尾管理
	std::unique_ptr<LargeNumberOfObjects> chaseEnemys_;

	// 全体のモデル
	Model* model_;

	Player* player_ = nullptr;

	// エネミーエディタ
	std::unique_ptr<EnemyEditor> enemyEditor_;

};
