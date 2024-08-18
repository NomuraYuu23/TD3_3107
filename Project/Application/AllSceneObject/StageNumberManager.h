#pragma once
#include <cstdint>
class StageNumberManager
{

public : // 関数

	/// <summary>
	/// ImGui描画
	/// </summary>
	static void ImGuiDraw();

public: // 変数

	// ステージ番号
	static uint32_t stageNum_;

public: // 定数

	// 最大ステージ番号
	static const uint32_t kStageMax = 6;

};

