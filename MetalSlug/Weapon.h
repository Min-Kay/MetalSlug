#pragma once
#include "framework.h"
#include "Obj.h"
#include "Include.h"

class Weapon
{
public:
	Weapon() {};
	virtual ~Weapon() {};

public:
	virtual void Initialize()PURE;

public:
	virtual bool Fire(float _x , float _y , DIR::ID _dir)PURE;

public:
	void	Set_Ammo(int _ammo) { ammo = _ammo; }
	void	Add_Ammo(int _ammo) { ammo += _ammo; }

public:
	const int& Get_Ammo() const { return ammo; }
	const WEAPON::ID& Get_WepID() const { return wep_id; }

protected:
	bool isFire;

	WEAPON::ID wep_id;

	float coolTime;
	DWORD timer;

	int damage;
	int ammo;
	int speed;
};

