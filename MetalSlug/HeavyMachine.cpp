#include "HeavyMachine.h"
#include "Manager.h"

void HeavyMachine::Initialize()
{
	isFire = false;
	wep_id = WEAPON::HEAVY;
	ammo = 200;
	currCount = 0;
	fireCount = 5;
	damage = 10;
	coolTime = 200.f;
	timer = GetTickCount(); 
	interval = GetTickCount();
	isFiring = false; 
}

bool HeavyMachine::Fire(float _x, float _y, DIR::ID _dir)
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_ATTACK) && !isFiring && timer + coolTime < GetTickCount())
	{
		isFiring = true; 
		timer = GetTickCount();
	}
	else if(!isFiring)
	{
		return false;
	}

	if (isFiring && interval + 75.f < GetTickCount())
	{
		ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::HEAVY, _x, _y, _dir,angle);
		--ammo;
		++currCount;
		interval = GetTickCount(); 

		if (currCount >= fireCount)
		{
			currCount = 0; 
			isFiring = false; 
		}
	}
	return true;
}
