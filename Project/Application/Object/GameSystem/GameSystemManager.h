#pragma once
#include "../IObject.h"
#include "../../../Engine/Collision2D/Collision2DManager.h"

class GoalObject;

/// <summary>
/// ゲームシステム関係をまとめたクラス
/// </summary>
class GameSystemManager
{
public:
	// 現在のステージ番号
	static uint32_t sNowStageNum;
	// ゲームの速度
	static float sGameSpeed;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="goalModel"></param>
	void Initialize(Model* goalModel);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(BaseCamera& camera);
	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// コライダー登録
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionRegister(Collision2DManager* collisionManager);
private:
	/// <summary>
	/// ゴール生成関数
	/// </summary>
	/// <param name="position"></param>
	void GenarateGoal(const Vector3& position);


private:
	// ゴール
	std::unique_ptr<GoalObject> goal_;

	Model* goalModel_ = nullptr;
	Model* checkPointMode_ = nullptr;

};