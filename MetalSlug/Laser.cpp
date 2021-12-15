#include "Laser.h"
#include "Manager.h"
#include "LaserBullet.h"

Laser::Laser()
{
	bullet = nullptr;
	tickTimer = GetTickCount();
}

Laser::~Laser()
{
	ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::BULLET,bullet);
	SAFE_DELETE(bullet);
}

void Laser::Initialize()
{
	wep_id = WEAPON::LASER;
	ammo = 100;
	bullet = new LaserBullet;
	bullet->Initialize();
	ObjPoolMgr::Get_Instance()->Add_Object(OBJ::BULLET,bullet);
}

bool Laser::Fire(float _x, float _y, DIR::ID _dir)
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_ATTACK))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WEAPON);
		CSoundMgr::Get_Instance()->PlaySound(L"Laser_Shoot.wav", CSoundMgr::WEAPON, 2.f);
		bullet->Set_Pos(_x,_y);
		bullet->Set_Dir(_dir);
		bullet->Pressing(true);
		
		if (tickTimer + LASER_TICK < GetTickCount())
		{
			--ammo;
			tickTimer = GetTickCount();
		}

		return true;
	}
	else 
	{
		bullet->Reset();
		bullet->Pressing(false);
	}

	return false;
}

void Laser::Set_Fire(bool _bool)
{
	if (bullet) 
		bullet->Pressing(false); 
	isFire = _bool;
}
