#pragma once

#include "Include.h"
#include "Weapon.h"

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
	void	Add_Score(int _score) { score += _score; }
	void	Add_Life(int _life) { life += _life; }
	void	Add_Grenade(int _gre) { grenade += _gre;  }

public:
	const int& Get_Score() const { return score; }
	const int& Get_Life() const { return life; }
	const int& Get_Grenade() const { return grenade; }
	const int& Get_Ammo() const { if (weapon) return weapon->Get_Ammo();  return 0; }
};

