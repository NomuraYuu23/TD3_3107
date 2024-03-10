#pragma once
class TimerLib
{
public:
	TimerLib() {};
	~TimerLib() = default;

public:
	/// <summary>
	/// 初期化関数
	/// </summary>
	void StartSetting();

	/// <summary>
	/// タイマー設定関数
	/// </summary>
	/// <param name="endTime"></param>
	void Start(float endTime);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // アクセッサ

	bool IsEnd() { return isEnd_; }
	bool IsActive() { return isActive_; }

private:
	// 終了フレーム
	float endFrame_;
	// 現在のフレーム
	float nowFrame_;
	// 終了フラグ
	bool isEnd_;
	// 有効フラグ
	bool isActive_;

};

