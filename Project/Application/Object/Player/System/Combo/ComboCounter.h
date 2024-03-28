#pragma once

class ComboCounter
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Reset();

	/// <summary>
	/// 加算
	/// </summary>
	void Add();

	int GetCount() { return comboCount_; }

private:
	int comboCount_;

};
