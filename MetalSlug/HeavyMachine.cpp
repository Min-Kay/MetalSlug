#include "HeavyMachine.h"
#include "Manager.h"

void HeavyMachine::Initialize()
{
	isFire = false;
	wep_id = WEAPON::HEAVY;
	ammo = 200;
	currCount = 0;
	fireCount = 5;
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
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WEAPON);
			CSoundMgr::Get_Instance()->PlaySound(L"Pistol_Shoot.wav", CSoundMgr::WEAPON, 2.0f);

		ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::HEAVY, _x, _y, _dir);
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
