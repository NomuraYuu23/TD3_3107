#include "SingleEnemyRegister.h"
#include "../../../Engine/2D/ImguiManager.h"

uint32_t SingleEnemyRegister::sSerialNumber_ = 0;

void SingleEnemyRegister::Initialize(Model* model)
{
	LargeNumberOfObjects::Initialize(model);

	// 型番
	serialNum_ = sSerialNumber_;
	// 全体番号
	sSerialNumber_++;
	name_ = "EnemySingle:" + std::to_string(serialNum_);

}

void SingleEnemyRegister::Update()
{

	LargeNumberOfObjects::Update();

	// フラグによる死亡処理
	objects_.remove_if([this](std::unique_ptr<OneOfManyObjects>& enemy) {
		if (enemy->IsDead()) {
			enemy.reset();
			return true;
		}
		return false;
		});

}

void SingleEnemyRegister::ImGuiDraw()
{
	//ImGui::Begin(name_.c_str());
	ImGui::Text(name_.c_str());
	//ImGui::End();
}
