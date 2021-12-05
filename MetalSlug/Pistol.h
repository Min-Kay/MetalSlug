#pragma once
#include "Weapon.h"
class Pistol :
	public Weapon
{
public:
	virtual void Initialize() override;
	virtual bool Fire(float _x, float _y, DIR::ID _dir) override;

private:
	float angle;
	bool isFire; 
};

