#pragma once
#include "Weapon.h"
class HeavyMachine :
	public Weapon
{
	virtual void Initialize() override;
	virtual bool Fire(float _x, float _y, DIR::ID _dir) override;

private:
	int fireCount = 0;
	int currCount = 0;
	bool isFiring = false;
	DWORD	interval = GetTickCount();
};

