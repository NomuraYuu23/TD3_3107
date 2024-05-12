#pragma once
#include "../../../Engine/3D/LargeNumberOfObjects.h"
#include "../GameUtility/TimerLib.h"

class IEnemyEmitter : public LargeNumberOfObjects
{
private:
	// 共通の番号
	static uint32_t sSerialNumber_;

	// シリアルナンバー
	uint32_t serialNum_ = 0;

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
	/// 回転速度などエミッター単体の設定（ここ変更加えそう
	/// </summary>
	/// <param name="spinSpeed"></param>
	void InitializeEmitter(float spinSpeed);
	/// <summary>
	/// 敵生成
	/// </summary>
	/// <param name="transformPosition">エミッターの座標</param>
	/// <param name="distance">エミッターからの距離</param>
	/// <param name="enemyCount">生成したい敵の数</param>
	void CreateEnemy(const Vector3& transformPosition, float distance, uint32_t enemyCount);

	/// <summary>
	/// ワールドトランスフォームの取得
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return &worldTransform_; };

	void ImGuiDraw() override;

	bool IsRotateReturn() { return isRotateReturn_; }

	float GetNowAngle() { return nowAngle_; }

private:
	// 
	std::string name_;

private:
	// 親
	WorldTransform worldTransform_;
	// 距離
	float distance_ = 0.0f;
	// 最大数
	uint32_t maxCount_ = 0u;
	// 今の角度
	float nowAngle_ = 0.0f;
	// フレーム単位の回転速度
	float rotation_ = 0.0f;

	// 一周したか？
	bool isRotateReturn_ = false;

	TimerLib interval_;
};
