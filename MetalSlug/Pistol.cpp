#include "Pistol.h"
#include "Manager.h"

void Pistol::Initialize()
{
	wep_id = WEAPON::PISTOL;
	ammo = -1;
	isFire = false; 
	coolTime = 200.f;
	timer = GetTickCount();
}

bool Pistol::Fire(float _x, float _y, DIR::ID _dir)
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_ATTACK) && coolTime + timer < GetTickCount())
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WEAPON);
		CSoundMgr::Get_Instance()->PlaySound(L"Pistol_Shoot.wav", CSoundMgr::WEAPON, 2.0f);

		ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::PISTOL, _x, _y, _dir);
		timer = GetTickCount();
		isFire = true;
		return true;
	}
	else if (isFire && coolTime + timer > GetTickCount())
	{
		return true;
	}
	else if (isFire && coolTime + timer < GetTickCount())
	{
		timer = GetTickCount();
		isFire = false; 
		return false;
	}

	return false;
}
		

