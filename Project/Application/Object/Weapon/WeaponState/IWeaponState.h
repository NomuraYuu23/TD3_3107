#pragma once

class Weapon;

class IWeaponState
{
public:
	virtual ~IWeaponState() = default;

public:

	void PreInitialize(Weapon* weapon);
	virtual void Initialize() = 0;
	virtual void Update() = 0;

protected:
	Weapon* weapon_ = nullptr;

};

