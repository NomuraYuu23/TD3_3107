#include "EnemyManager.h"
#include "../ObjectList.h"
#include "../GameUtility/MathUtility.h"
#include "State/EnemyStateList.h"
#include "SingleEnemyRegister.h"

void EnemyManager::Initialize(Model* model)
{
	model_ = model;
	//CreateEmitter({ {-5.0f,10.0f},7.0f,5 });
	CreateEmitter({ {0.0f,10.0f},7.0f,5 });
	CreateEmitter({ {-20.0f,10.0f},7.0f,3 });

	CreateSingleEnemy();
}

void EnemyManager::Update()
{
	// 更新をLargeのやつごとに
	for (std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator it = enemyEmitters_.begin();
		it != enemyEmitters_.end(); ++it) {
		(*it)->Update();
	}
}

void EnemyManager::Draw(BaseCamera& camera, std::vector<UINT>* textureHnadles)
{
	// ドローコールをLargeのやつごとに
	for (std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator it = enemyEmitters_.begin();
		it != enemyEmitters_.end(); ++it) {
		(*it)->Draw(camera, textureHnadles);
	}
}

void EnemyManager::ImGuiDraw()
{
	ImGui::Begin("EnemyManager");

	//ImGui::DragFloat3("pos", &resPoint_.x, 0.01f, -100.0f, 100.0f);

	//int size = (int)enemyEmitters_.size();
	//ImGui::InputInt("Size", &size);

	//ImGui::Separator();
	int maxSize = 0;
	for (std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator itParent = enemyEmitters_.begin();
		itParent != enemyEmitters_.end(); ++itParent) {

		for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = (*itParent)->GetObjects()->begin();
			it != (*itParent)->GetObjects()->end(); ++it) {
			maxSize++;
		}

	}

	ImGui::InputInt("MaxEnemy", &maxSize);

	ImGui::SeparatorText("Emittes");

	for (std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator itr = enemyEmitters_.begin();
		itr != enemyEmitters_.end(); ++itr) {
		(*itr)->ImGuiDraw();
		ImGui::Separator();
	}


	ImGui::End();
}

void EnemyManager::CollisionRegister(Collision2DManager* collisionManager, const BaseCamera& camera)
{
	for (std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator itParent = enemyEmitters_.begin();
		itParent != enemyEmitters_.end(); ++itParent) {

		for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = (*itParent)->GetObjects()->begin();
			it != (*itParent)->GetObjects()->end(); ++it) {
			collisionManager->ListRegister(&static_cast<Enemy*>((it->get()))->boxCollider_);
		}

	}
}

void EnemyManager::CreateEmitter(const MultiEnemyData& data)
{
	std::unique_ptr<LargeNumberOfObjects> obj = std::make_unique<IEnemyEmitter>();
	// 初期化
	obj->Initialize(model_);
	// 敵生成
	static_cast<IEnemyEmitter*>(obj.get())->CreateEnemy(data.position, data.distance, data.enemyMaxCount);
	// エミッターの設定
	static_cast<IEnemyEmitter*>(obj.get())->InitializeEmitter(90.0f);
	// リストに
	enemyEmitters_.push_back(std::move(obj));

}

void EnemyManager::RegisterEnemy(const SingleEnemyData& data)
{

	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
	obj->Initialize();
	obj->transform_.translate = data.position;
	// 初期化
	static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<EnemyGroundState>(), data.typeNum);
	
	// 追加
	singleEnemys_->GetObjects()->push_back(std::move(obj));

}

void EnemyManager::CreateSingleEnemy()
{
	// 単体管理用変数に
	singleEnemys_ = std::make_unique<SingleEnemyRegister>();
	singleEnemys_->Initialize(model_);

	///---ここに敵単体ごとに登録する---//
	RegisterEnemy({ {10,10,0},0 });

	// Largeの奴でまとめるためにリストにプッシュ
	enemyEmitters_.push_back(std::move(singleEnemys_));
}
