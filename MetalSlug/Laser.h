#pragma once
#include "Weapon.h"
class LaserBullet;
class Laser :
	public Weapon
{

public:
	Laser();
	virtual ~Laser();

public:
	virtual void Initialize() override;
	virtual bool Fire(float _x, float _y, DIR::ID _dir) override;

private:
	LaserBullet* bullet; 
	DWORD tickTimer;
};

