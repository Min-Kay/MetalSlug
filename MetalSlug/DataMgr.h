#pragma once

#include "Include.h"
#include "Weapon.h"
#include "Weapons.h"

class DataMgr
{
private:
	DataMgr() {};
	virtual ~DataMgr() {};

private:
	static DataMgr* instance;
	
public:
	static DataMgr* Get_Instance()
	{
		if (!instance)
			instance = new DataMgr;
		
		return instance;
	}

	static void	Destroy_Instance()
	{
		if (instance)
		{
			delete instance; 
			instance = nullptr;
		}
	}

public:
	void	Initialize();

private:
	int	score;
	int life;
	int grenade;
	Weapon* weapon;

public:
	void	Set_Score(int _score) { score = _score; }
	void	Set_Life(int _life) { life = _life; }
	void	Set_Grenade(int _gre) { grenade = _gre; }
	void	Set_Weapon(Weapon* _wep) { weapon = _wep; }

public:
	void	Add_Score(int _score) { score += _score; }
	void	Add_Life(int _life) { life += _life; }
	void	Add_Grenade(int _gre) { grenade += _gre;  }
	void	Add_Ammo(int _ammo) { if (weapon && weapon->Get_WepID() == WEAPON::PISTOL) return; weapon->Add_Ammo(_ammo); }

public:
	const Weapon* Get_Weapon() const { if (weapon) return weapon; return nullptr; }
	const int& Get_Score() const { return score; }
	const int& Get_Life() const { return life; }
	const int& Get_Grenade() const { return grenade; }
	const int& Get_Ammo() const { if (weapon) return weapon->Get_Ammo(); return 0; }
};

