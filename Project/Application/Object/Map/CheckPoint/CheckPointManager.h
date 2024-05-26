#pragma once
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Camera/BaseCamera.h"
#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../../../AllSceneObject/StageNumberManager.h"
#include "CheckPointData.h"
#include <vector>
#include <memory>

class Player;
class CheckPointObject;

class CheckPointManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	/// <param name="checkPointModel"></param>
	void Initialize(Player* player, Model* checkPointModel);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(BaseCamera& camera);

	void ImGuiDraw();

	/// <summary>
	/// チェックポイント生成関数
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="checkNumber">チェックポイントの番号</param>
	void GenerateCheckPoint(const Vector3& position, uint32_t checkNumber);
	/// <summary>
	/// チェックポイント更新処理
	/// </summary>
	/// <param name="num"></param>
	void CheckPointJudge(uint32_t num);
	/// <summary>
	/// 当たり判定の設定
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionRegister(Collision2DManager* collisionManager);

public: // アクセッサ
	/// <summary>
	/// 今のチェックポイント番号
	/// </summary>
	/// <returns></returns>
	uint32_t GetCheckNumber() { return checkPointNum_; }

	void SetCheckNumber(uint32_t num) { checkPointNum_ = num; }


private:
	// プレイヤー
	Player* player_ = nullptr;
	// モデル
	Model* checkPointModel_ = nullptr;

	/// <summary>
	/// オブジェクトリスト
	/// </summary>
	std::vector<std::unique_ptr<CheckPointObject>> checkPoints_;

	// 現在のチェックポイント番号
	uint32_t checkPointNum_ = 0u;
	// 前のチェックポイント番号
	uint32_t prevCheckPointNum_ = 0u;

private: // 

	std::array<std::vector<CheckPointData>, StageNumberManager::kStageMax> checkPointDatas_;

};