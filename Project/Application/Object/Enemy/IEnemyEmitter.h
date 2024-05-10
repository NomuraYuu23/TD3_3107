#pragma once
#include "../../../Engine/3D/LargeNumberOfObjects.h"

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
	/// 敵のリスト取得
	/// </summary>
	/// <returns></returns>
	//std::list<std::unique_ptr<OneOfManyObjects>>* GetList() { return &objects_; }
	WorldTransform* GetWorldTransform() { return &worldTransform_; };

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
	float nowAngle_;
	// フレーム単位の回転速度
	float rotation_;
};
