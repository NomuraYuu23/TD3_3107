#include "StageNumberManager.h"
#include "../../Engine/2D/ImguiManager.h"
// ステージ番号
uint32_t StageNumberManager::stageNum_ = 0;

void StageNumberManager::ImGuiDraw()
{

	ImGui::Begin("StageNumberManager");
	ImGui::DragInt("stageNum_", reinterpret_cast<int32_t*>(&stageNum_), 0.01f, 0, kStageMax - 1);
	ImGui::End();

}
