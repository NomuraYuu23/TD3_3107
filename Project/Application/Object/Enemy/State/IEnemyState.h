#pragma once

class Enemy;

class IEnemyState
{
public:
	/// <summary>
	/// 前初期化
	/// </summary>
	void PreInitialize(Enemy* enemy);
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

protected:
	// 親のエネミーポインタ
	Enemy* enemy_ = nullptr;
	// 落下フラグ
	bool isFall_ = false;

private:

};
