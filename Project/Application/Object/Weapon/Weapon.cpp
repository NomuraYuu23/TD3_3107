#include "Weapon.h"

void Weapon::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);
}

void Weapon::Update()
{
	// 基底クラスの更新
	IObject::Update();
}

void Weapon::Draw(BaseCamera camera)
{

	camera;

}

void Weapon::ImGuiDraw()
{

}
