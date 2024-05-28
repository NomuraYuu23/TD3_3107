#include "EnemyManager.h"
#include "../ObjectList.h"
#include "../GameUtility/MathUtility.h"
#include "State/EnemyStateList.h"
#include "SingleEnemyRegister.h"
#include "../../AllSceneObject/StageNumberManager.h"

void EnemyManager::Initialize(Model* model, std::vector<UINT>* texture)
{
	model_ = model;

	SingleEnemyRegister::ResetSerialNumber();

	CreateSingleEnemy(texture);
	////CreateEmitter({ {-5.0f,10.0f},7.0f,5 });
	//CreateEmitter({ {0.0f,10.0f},7.0f,5 });
	//CreateEmitter({ {-20.0f,10.0f},7.0f,3 });

	enemyEditor_ = std::make_unique<EnemyEditor>();
	enemyEditor_->LoadFiles();
	LoadEnemyData();
	RegisterChaseEnemy({ {10,10,0},30.0f }, "Chase1");

}

void EnemyManager::Update()
{

#ifdef _DEBUG
	LoadEnemyData();
#endif // _DEBUG

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

	enemyEditor_->ImGuiDraw();

}

void EnemyManager::CollisionRegister(Collision2DManager* collisionManager, const BaseCamera& camera)
{
	for (std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator itParent = enemyEmitters_.begin();
		itParent != enemyEmitters_.end(); ++itParent) {

		for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = (*itParent)->GetObjects()->begin();
			it != (*itParent)->GetObjects()->end(); ++it) {
			Enemy* obj = static_cast<Enemy*>((it->get()));
			collisionManager->ListRegister(&obj->boxCollider_);
			// チェイスなら
			bool stateCheck = std::holds_alternative<ChaseEnemyState*>(obj->GetState());
			if (stateCheck) {
				ChaseEnemyState* state = static_cast<ChaseEnemyState*>(obj->GetNowState());
				collisionManager->ListRegister(&(state->sensorRay_.directRay_));
			}

		}

	}
}

void EnemyManager::CreateEmitter(const MultiEnemyData& data)
{
	std::unique_ptr<LargeNumberOfObjects> obj = std::make_unique<IEnemyEmitter>();
	// 初期化
	obj->Initialize(model_);
	// エミッターの設定
	static_cast<IEnemyEmitter*>(obj.get())->InitializeEmitter(data.rotateSpeed);
	// 敵生成
	static_cast<IEnemyEmitter*>(obj.get())->CreateEnemy(data.position, data.distance, data.enemyMaxCount);
	// リストに
	enemyEmitters_.push_back(std::move(obj));

}

void EnemyManager::CreateEmitter(const MultiEnemyData& data, const std::string& name)
{
	std::unique_ptr<LargeNumberOfObjects> obj = std::make_unique<IEnemyEmitter>();
	// 初期化
	static_cast<IEnemyEmitter*>(obj.get())->Initialize(model_, name);
	// エミッターの設定
	static_cast<IEnemyEmitter*>(obj.get())->InitializeEmitter(data.rotateSpeed);
	// 敵生成
	static_cast<IEnemyEmitter*>(obj.get())->CreateEnemy(data.position, data.distance, data.enemyMaxCount);
	// リストに
	enemyEmitters_.push_back(std::move(obj));

}

void EnemyManager::LoadEnemyData()
{

	// マップデータ
	std::map<std::string, EnemyEditor::EnemyEditorGroup>* mapDatas = enemyEditor_->GetDatas();

	// ステージ
	std::string stageName = "Stage" + std::to_string(StageNumberManager::stageNum_);

	if (StageNumberManager::stageNum_ < 10) {
		stageName = "Stage0" + std::to_string(StageNumberManager::stageNum_);
	}

	// シングル用
	std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator enemySingle0 = enemyEmitters_.begin();
	enemySingle0->get()->GetObjects()->clear();

	for (std::map<std::string, EnemyEditor::EnemyEditorGroup>::iterator stageItr = mapDatas->begin();
		stageItr != mapDatas->end(); ++stageItr) {

		// ステージ番号が違う
		if (stageItr->first != stageName) {
			continue;
		}

		std::vector<std::string> enemyNames;
		std::vector<std::string> enemySingleNames;

		for (EnemyEditor::EnemyEditorGroup::iterator enemyEmitterItr = stageItr->second.begin();
			enemyEmitterItr != stageItr->second.end(); ++enemyEmitterItr) {

			std::string enemyName = enemyEmitterItr->first;
			EnemyEditor::EnemyEditorItem enemyData = enemyEmitterItr->second;
			bool edited = false;

			if (std::holds_alternative<MultiEnemyData>(enemyData)) {
		
				// 編集
				for (std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator it = enemyEmitters_.begin();
					it != enemyEmitters_.end(); ++it) {

					// ブロックの番号が違う
					if (enemyName != static_cast<IEnemyEmitter*>(it->get())->GetName()) {
						continue;
					}

					it->get()->GetObjects()->clear();
					
					MultiEnemyData data = std::get<MultiEnemyData>(enemyData);

					static_cast<IEnemyEmitter*>(it->get())->CreateEnemy(data.position, data.distance, data.enemyMaxCount);
					
					edited = true;

					break;

				}

				// 追加
				if (!edited) {
					CreateEmitter(std::get<MultiEnemyData>(enemyData), enemyName);
				}

				// 削除用に名前登録
				enemyNames.push_back(enemyName);

			}
			
			// シングル
			else if (std::holds_alternative<SingleEnemyData>(enemyData)) {
				RegisterEnemy(std::get<SingleEnemyData>(enemyData), enemyName);

				enemySingleNames.push_back(enemyName);
			}

		}

		// 削除
		enemySingle0->get()->GetObjects()->remove_if([=](std::unique_ptr<OneOfManyObjects>& enemy) {

			for (uint32_t i = 0; i < enemySingleNames.size(); ++i) {
				std::string name = static_cast<Enemy*>(enemy.get())->GetName();
				if (enemySingleNames[i] == name) {
					return false;
				}
			}
			return true;


			});

		enemyEmitters_.remove_if([=](std::unique_ptr<LargeNumberOfObjects>& enemyEmitter) {

			if ("EnemySingle:0" == static_cast<IEnemyEmitter*>(enemyEmitter.get())->GetName()) {
				return false;
			}

			for (uint32_t i = 0; i < enemyNames.size(); ++i) {
				std::string name = static_cast<IEnemyEmitter*>(enemyEmitter.get())->GetName();
				if (enemyNames[i] == name) {
					return false;
				}
			}
			return true;

			});

	}

}

void EnemyManager::RegisterEnemy(const SingleEnemyData& data)
{

	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
	obj->Initialize();
	obj->transform_.translate = data.position;
	// 初期化
	static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<SingleEnemyState>(), data.typeNum);
	// 単体のやつ専用
	Vector3 end = obj->GetWorldPosition() + data.endPosition;
	static_cast<SingleEnemyState*>(static_cast<Enemy*>(obj.get())->GetNowState())->SettingMoveInfo(end);
	static_cast<Enemy*>(obj.get())->SetPlayer(player_);
	// 追加
	enemyEmitters_.begin()->get()->GetObjects()->push_back(std::move(obj));

}

void EnemyManager::RegisterEnemy(const SingleEnemyData& data, const std::string& name)
{

	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
	static_cast<Enemy*>(obj.get())->Initialize(name);
	obj->transform_.translate = data.position;
	// 初期化
	static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<SingleEnemyState>(), data.typeNum);
	// 単体のやつ専用
	Vector3 end = obj->GetWorldPosition() + data.endPosition;
	static_cast<SingleEnemyState*>(static_cast<Enemy*>(obj.get())->GetNowState())->SettingMoveInfo(end);
	static_cast<Enemy*>(obj.get())->SetPlayer(player_);
	// 追加
	enemyEmitters_.begin()->get()->GetObjects()->push_back(std::move(obj));

}

void EnemyManager::RegisterChaseEnemy(const ChaseEnemyData& data, const std::string& name)
{
	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
	static_cast<Enemy*>(obj.get())->Initialize(name);
	obj->transform_.translate = data.position;
	// 初期化
	static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<ChaseEnemyState>(), 0);
	// プレイヤーセット
	static_cast<Enemy*>(obj.get())->SetPlayer(player_);
	// 索敵設定
	static_cast<ChaseEnemyState*>(static_cast<Enemy*>(obj.get())->GetNowState())->SetChaseSetting(data.searchLength);
	// 追加
	enemyEmitters_.begin()->get()->GetObjects()->push_back(std::move(obj));
}

void EnemyManager::CreateSingleEnemy(std::vector<UINT>* texture)
{
	// 単体管理用変数に
	singleEnemys_ = std::make_unique<SingleEnemyRegister>();
	singleEnemys_->Initialize(model_);
	// テクスチャの設定
	static_cast<SingleEnemyRegister*>(singleEnemys_.get())->SetTexture(texture);


	// Largeの奴でまとめるためにリストにプッシュ
	enemyEmitters_.push_back(std::move(singleEnemys_));

	//RegisterChaseEnemy({ {30,10,0},30.0f }, "Chase1");
	//RegisterSingleEnemy({ {10,10,0},0 }, "na");
	///---ここに敵単体ごとに登録する---//
	//RegisterEnemy({ {10,10,0},0 });
}
