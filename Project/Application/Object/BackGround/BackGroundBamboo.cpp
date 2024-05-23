#include "BackGroundBamboo.h"

void BackGroundBamboo::Initialize()
{
	// 基底クラスの初期化
	OneOfManyObjects::Initialize();
}

void BackGroundBamboo::Initialize(const std::string& name)
{
	// 本クラスの初期化を行う
	BackGroundBamboo::Initialize();
	// 名前設定
	name_ = name;
}

void BackGroundBamboo::Update()
{
	// 基底クラスの更新処理
	OneOfManyObjects::Update();
}
