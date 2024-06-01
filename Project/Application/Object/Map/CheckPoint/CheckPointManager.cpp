#include "CheckPointManager.h"
#include "../../ObjectList.h"

void CheckPointManager::Initialize(Player* player, Model* checkPointModel, Model* passedModel, GameAudioManager* gam)
{
	// プレイヤー
	player_ = player;
	checkPointModel_ = checkPointModel;
	passedModel_ = passedModel;
	audioManager_ = gam;

	// モデルの設定以外まとめてる場所
	Setting();
}

void CheckPointManager::Update()
{

#ifdef _DEMO
	EditorCheckPointLoad();
#endif // _DEBUG


	// 更新
	for (std::list<std::unique_ptr<CheckPointObject>>::iterator it = checkPoints_.begin();
		it != checkPoints_.end(); ++it) {
		(*it)->Update();
	}
}

void CheckPointManager::Draw(BaseCamera& camera)
{
	//	描画
	for (std::list<std::unique_ptr<CheckPointObject>>::iterator it = checkPoints_.begin();
		it != checkPoints_.end(); ++it) {
		(*it)->Draw(camera);
	}
}

void CheckPointManager::ImGuiDraw()
{
	uint32_t num = 1;
	for (std::list<std::unique_ptr<CheckPointObject>>::iterator it = checkPoints_.begin();
		it != checkPoints_.end(); ++it) {
		std::string name = "CheckPoint:" + std::to_string(num);
		ImGui::DragFloat3(name.c_str(), &(*it)->worldtransform_.transform_.translate.x);
		num++;
	}
	int prev = prevCheckPointNum_;
	int now = checkPointNum_;
	ImGui::DragInt("PrevCheck", &prev);
	ImGui::DragInt("NowCheck", &now);
	ImGui::DragFloat3("ResPos", &respawnPosition_.x);
}

void CheckPointManager::Setting()
{
	// チェックポイントの番号初期
	prevCheckPointNum_ = 0;
	checkPointNum_ = prevCheckPointNum_;

	// 初期座標
	defaulResPosition_ = { 4.0f,3.0f,0 };
	// 更新されていくリス座標
	respawnPosition_ = defaulResPosition_;

	checkPointEditor_ = std::make_unique<CheckPointEditor>();
	checkPointEditor_->LoadFiles();

	EditorCheckPointLoad();
}

void CheckPointManager::GenerateCheckPoint(const Vector3& position, int32_t checkNumber)
{
	std::unique_ptr<CheckPointObject> obj = std::make_unique<CheckPointObject>();
	obj->Initialize(checkPointModel_);
	obj->SetManager(this);
	obj->Setting({ position, checkNumber });
	obj->SetPassedModel(passedModel_);
	checkPoints_.push_back(std::move(obj));
	obj.reset(nullptr);
}

void CheckPointManager::CheckPointJudge(const Vector3& position, uint32_t num)
{
	// 前のチェックポイント設定
	prevCheckPointNum_ = checkPointNum_;
	// 前のチェックポイントよりも先なら更新
	uint32_t newNum = num + 1;
	if (prevCheckPointNum_ < newNum) {
		checkPointNum_ = newNum;
		respawnPosition_ = position;
	}

}

void CheckPointManager::CollisionRegister(Collision2DManager* collisionManager)
{
	// コライダーに設定
	for (std::list<std::unique_ptr<CheckPointObject>>::iterator it = checkPoints_.begin();
		it != checkPoints_.end(); ++it) {
		collisionManager->ListRegister(&(*it)->boxCollider_);
	}
}

void CheckPointManager::EditorCheckPointLoad()
{

	// マップデータ
	std::map<std::string, std::map<std::string, CheckPointData>>* checkPoints = checkPointEditor_->GetDatas();

	// ステージ
	std::string stageName = "CheckPoint" + std::to_string(StageNumberManager::stageNum_);

	if (StageNumberManager::stageNum_ < 10) {
		stageName = "CheckPoint0" + std::to_string(StageNumberManager::stageNum_);
	}

	for (std::map<std::string, std::map<std::string, CheckPointData>>::iterator stageItr = checkPoints->begin();
		stageItr != checkPoints->end(); ++stageItr) {

		// ステージ番号が違う
		if (stageItr->first != stageName) {
			continue;
		}

		std::vector<uint32_t> usedNums_;

		// 編集、追加
		for (std::map<std::string, CheckPointData>::iterator terrainItr = stageItr->second.begin();
			terrainItr != stageItr->second.end(); ++terrainItr) {

			bool edited = false;

			for (uint32_t i = 0; i < checkPoints_.size(); ++i) {
				if (std::next(checkPoints_.begin(), i)->get()->GetCheckNum() == terrainItr->second.checkPointNumber) {
					std::next(checkPoints_.begin(), i)->get()->SetPosition(terrainItr->second.position);
					edited = true;
					usedNums_.push_back(terrainItr->second.checkPointNumber);
					break;
				}
			}

			if (!edited) {
				GenerateCheckPoint(terrainItr->second.position, terrainItr->second.checkPointNumber);
				usedNums_.push_back(terrainItr->second.checkPointNumber);
			}

		}

		// 名前確認してないやつ消す
			// フラグによる死亡処理
		checkPoints_.remove_if([=](std::unique_ptr<CheckPointObject>& checkPoint) {
			
			for (uint32_t i = 0; i < usedNums_.size(); ++i) {
				if (usedNums_[i] == checkPoint->GetCheckNum()) {
					return false;
				}
			}
			checkPoint.reset();
			return true;
			});

		break;

	}

}
