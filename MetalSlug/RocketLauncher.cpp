#include "RocketLauncher.h"
#include "Manager.h"

void RocketLauncher::Initialize()
{
	ammo = 100;
	wep_id = WEAPON::ROCKET;
	timer = GetTickCount();
	isFire = false;
}

bool RocketLauncher::Fire(float _x, float _y, DIR::ID _dir)
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_ATTACK) && timer + 200.f < GetTickCount())
	{

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WEAPON);
		CSoundMgr::Get_Instance()->PlaySound(L"Rocket_Shoot.wav",CSoundMgr::WEAPON,2.f);
		switch (_dir)
		{
		case DIR::UP:
			ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::ROCKET, _x, _y , _dir);
			break;
		case DIR::DOWN:
			ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::ROCKET, _x, _y , _dir);
			break;
		case DIR::RIGHT:
			ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::ROCKET, _x , _y, _dir);
			break;
		case DIR::LEFT:
			ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::ROCKET, _x , _y, _dir);
			break;
		}
		--ammo;
		timer = GetTickCount();
		isFire = true;
		return true;
	}
	else if (isFire && timer + 100.f > GetTickCount())
	{
		isFire = false;
		return true;
	}

	return false;
}
