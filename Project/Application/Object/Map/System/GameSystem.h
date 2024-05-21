#pragma once
#include "../../../Engine/3D/Model.h"

class GameSystem
{
public:
	GameSystem() {};
	~GameSystem() = default;

public:
	void Intiailize(Model* goalModel);

private:
	Model* goalModel_ = nullptr;

};