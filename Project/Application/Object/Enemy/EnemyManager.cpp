#include "EnemyManager.h"
#include "../ObjectList.h"
#include "../GameUtility/MathUtility.h"
#include "State/EnemyStateList.h"

void EnemyManager::Initialize(Model* model)
{
	LargeNumberOfObjects::Initialize(model);

	RegisterEnemy({ 80.0f,59.0f,0 }, 0);
	RegisterEnemy({ 90.0f,59.0f,0 }, 0);
	RegisterEnemy({ 100.0f,59.0f,0 }, 0);

	//RegisterEnemy({ 10.0f,0,0 }, 0, { 20.0f,10.0f });
	//RegisterEnemy({ -10.0f,0,0 }, 0, { 20.0f,10.0f });
	//RegisterEnemy({ 0,10.0f,0 }, 0, { 20.0f,10.0f });
	//RegisterEnemy({ 0,-10.0f,0 }, 0, { 20.0f,10.0f });

	testParent.Initialize();
	testParent.transform_.translate = { -5.0f,15.0f };

	RegisterEnemy({ 10.0f,0,0 }, 0, &testParent);
	RegisterEnemy({ -10.0f,0,0 }, 0, &testParent);
	RegisterEnemy({ 0,10.0f,0 }, 0, &testParent);
	RegisterEnemy({ 0,-10.0f,0 }, 0, &testParent);

	for (int i = 0; i < 3; i++) {
		emitters_[i].Initialize();
	}
	emitters_[0].transform_.translate = { -40.0f, 17.5f };

	RegisterEnemy({ 12.5f,0,0 }, 0, &emitters_[0]);
	RegisterEnemy({ -12.5f,0,0 }, 0, &emitters_[0]);
	RegisterEnemy({ 0,12.5f,0 }, 0, &emitters_[0]);
	RegisterEnemy({ 0,-12.5f,0 }, 0, &emitters_[0]);

}

void EnemyManager::Update()
{
	// フラグによる死亡処理
	objects_.remove_if([this](std::unique_ptr<OneOfManyObjects>& enemy) {
		if (enemy->IsDead()) {
			enemy.reset();
			return true;
		}
		return false;
		});

	// 
	testParent.UpdateMatrix();
	testParent.transform_.rotate.z -= 0.005f;
	for (int i = 0; i < 3; i++) {
		emitters_[0].UpdateMatrix();
	}
	emitters_[0].transform_.rotate.z += 0.01f;
	// リストの更新処理
	LargeNumberOfObjects::Update();

}

void EnemyManager::ImGuiDraw()
{
	ImGui::Begin("EnemyManager");

	if (ImGui::Button("RegisterMeleeBlock")) {
		//RegisterBlock();
		RegisterEnemy(resPoint_, 0);
	}
	if (ImGui::Button("RegisterRangedBlock")) {
		//RegisterBlock();
		RegisterEnemy(resPoint_, 1);
	}

	ImGui::DragFloat3("Parent", &testParent.transform_.translate.x, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat3("PRot", &testParent.transform_.rotate.x, 0.01f, -100, 100);

	ImGui::DragFloat3("pos", &resPoint_.x, 0.01f, -100.0f, 100.0f);

	int size = (int)objects_.size();
	ImGui::InputInt("Size", &size);

	ImGui::Separator();

	// ブロック達のImGui

	for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		static_cast<Enemy*>((it->get()))->ImGuiDraw();
	}

	ImGui::End();
}

void EnemyManager::CollisionRegister(Collision2DManager* collisionManager, const BaseCamera& camera)
{

	for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		//float range = 100.0f;
		//if (!MathUtility::CheckOutScreen((*it)->GetWorldPosition(), range, camera)) {
			collisionManager->ListRegister(&static_cast<Enemy*>((it->get()))->boxCollider_);
		//}

	}
}

void EnemyManager::RegisterEnemy(const Vector3& position, uint32_t typeNum)
{

	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
	obj->Initialize();
	obj->transform_.translate = position;
	// 初期化
	static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<EnemyGroundState>(), typeNum);
	
	// 追加
	objects_.push_back(std::move(obj));


}

void EnemyManager::RegisterEnemy(const Vector3& offset, uint32_t typeNum, const Vector3& parent)
{

	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
	obj->Initialize();
	static_cast<Enemy*>(obj.get())->parentPosition_ = parent;
	obj->transform_.translate = Vector3::Add(parent, offset);
	// 初期化
	static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<EnemyGroundState>(), typeNum);

	// 追加
	objects_.push_back(std::move(obj));


}

void EnemyManager::RegisterEnemy(const Vector3& offset, uint32_t typeNum, WorldTransform* parent)
{

	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
	obj->Initialize();
	static_cast<Enemy*>(obj.get())->SetParent(parent);
	obj->transform_.translate = Vector3::Add(parent->GetWorldPosition(), offset);
	// 初期化
	static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<EnemyGroundState>(), typeNum);

	// 追加
	objects_.push_back(std::move(obj));

}
