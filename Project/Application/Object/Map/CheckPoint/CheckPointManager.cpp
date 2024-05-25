#include "CheckPointManager.h"
#include "../../ObjectList.h"

void CheckPointManager::Initialize(Player* player, Model* checkPointModel)
{
	// プレイヤー
	player_ = player;
	checkPointModel_ = checkPointModel;

	prevCheckPointNum_ = 0;
}

void CheckPointManager::Update()
{
	// 更新
	for (std::vector<std::unique_ptr<CheckPointObject>>::iterator it = checkPoints_.begin();
		it != checkPoints_.end(); ++it) {
		(*it)->Update();
	}
}

void CheckPointManager::Draw(BaseCamera& camera)
{
	//	描画
	for (std::vector<std::unique_ptr<CheckPointObject>>::iterator it = checkPoints_.begin();
		it != checkPoints_.end(); ++it) {
		(*it)->Draw(camera);
	}
}

void CheckPointManager::ImGuiDraw()
{
	uint32_t num = 1;
	for (std::vector<std::unique_ptr<CheckPointObject>>::iterator it = checkPoints_.begin();
		it != checkPoints_.end(); ++it) {
		std::string name = "CheckPoint:" + std::to_string(num);
		ImGui::DragFloat3(name.c_str(), &(*it)->worldtransform_.transform_.translate.x);
		num++;
	}
	int prev = prevCheckPointNum_;
	int now = checkPointNum_;
	ImGui::DragInt("PrevCheck", &prev);
	ImGui::DragInt("NowCheck", &now);
}

void CheckPointManager::GenerateCheckPoint(const Vector3& position, uint32_t checkNumber)
{
	std::unique_ptr<CheckPointObject> obj = std::make_unique<CheckPointObject>();
	obj->Initialize(checkPointModel_);
	obj->SetManager(this);
	obj->Setting({ position,checkNumber });
	checkPoints_.push_back(std::move(obj));
}

void CheckPointManager::CheckPointJudge(uint32_t num)
{
	// 前のチェックポイント設定
	prevCheckPointNum_ = checkPointNum_;
	// 前のチェックポイントよりも先なら更新
	if (prevCheckPointNum_ < num) {
		checkPointNum_ = num;
	}

}

void CheckPointManager::CollisionRegister(Collision2DManager* collisionManager)
{
	// コライダーに設定
	for (std::vector<std::unique_ptr<CheckPointObject>>::iterator it = checkPoints_.begin();
		it != checkPoints_.end(); ++it) {
		collisionManager->ListRegister(&(*it)->boxCollider_);
	}
}
