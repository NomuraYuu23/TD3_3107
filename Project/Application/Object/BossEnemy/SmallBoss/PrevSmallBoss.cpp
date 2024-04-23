#include "PrevSmallBoss.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "State/SmallBossStateList.h"
#include "Weapon/WaveAttack.h"

void PrevSmallBoss::Initialize(Model* model)
{

	IBoss::Initialize(model);
	worldtransform_.transform_.translate = { 50.0f,20.0f,0 };
	worldtransform_.UpdateMatrix();

	// ステート変更
	ChangeState(std::make_unique<WaveAttackState>());
}

void PrevSmallBoss::Update()
{
	// 状態の更新
	if (state_) {
		state_->Update();
	}

	for (std::list<std::unique_ptr<IAttackObject>>::iterator it = attackLists_.begin(); it != attackLists_.end(); ++it) {
		// 更新
		(*it)->Update();
	}

	// 更新処理
	IBoss::Update();
	BoxColliderUpdate();
}

void PrevSmallBoss::Draw(const BaseCamera& camera)
{
	// 描画
	ModelDraw::AnimObjectDesc desc;
	desc.camera = &const_cast<BaseCamera&>(camera);
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.textureHandles;
	desc.worldTransform;
	ModelDraw::AnimObjectDraw(desc);

	for (std::list<std::unique_ptr<IAttackObject>>::iterator it = attackLists_.begin(); it != attackLists_.end(); ++it) {
		// 更新
		(*it)->Draw(camera);
	}

}

void PrevSmallBoss::ImGuiDraw()
{

	ImGui::Begin("SmallBoss");

	// 状態の名前取得
	std::string name = typeid(*state_).name();
	ImGui::Text(name.c_str());
	int size = (int)attackLists_.size();
	ImGui::InputInt("listSize", &size);
	for (std::list<std::unique_ptr<IAttackObject>>::iterator it = attackLists_.begin(); it != attackLists_.end(); ++it) {
		// 更新
		uint32_t num = (*it)->serialNum_;
		std::string name = "AttackObj" + std::to_string(num);
		ImGui::Text(name.c_str());
	}


	ImGui::End();

}

void PrevSmallBoss::OnCollision(ColliderParentObject2D target)
{
	target;
}
