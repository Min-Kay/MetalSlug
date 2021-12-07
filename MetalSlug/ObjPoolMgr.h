#pragma once
#include "Include.h"

class Obj;
class Weapon;

class ObjPoolMgr
{

private:
	ObjPoolMgr();
	~ObjPoolMgr();

public:
	void	Initialize();
	void	Update();
	void	Late_Update();
	void	Render(HDC _hdc);
	void	Release();

public:
	void	DisableObj();

public:
	void	Add_Object(OBJ::ID _id, Obj* _obj);
	float	Check_Distance(Obj* _target);

public:
	void	Set_Player_Dead(bool _bool) { player_Dead = _bool; }
	void	Set_Player_Wep(Weapon* _wep);

public:
	const bool& Get_Player_Dead() const { return player_Dead; }
	const	INFO& Get_Player_Info() const;
	const	RECT& Get_Player_Rect() const;

public:
	void	Spawn_Player(float _X, float _Y);
	void	Spawn_Enemy(ENEMY::ID _enemy, float _X, float _Y, DIR::ID _dir = DIR::LEFT, SOLDIER::CLASS _class = SOLDIER::PRIVATE);
	void	Spawn_Bullet(BULLET::ID _bullet, float _X, float _Y, DIR::ID _dir, float _angle = 0, OBJ::ID parent = OBJ::PLAYER);
	void	Spawn_Item(ITEM::ID _item, float _X, float _Y, WEAPON::ID _wep = WEAPON::HEAVY);
	void	Spawn_Block(float _cx, float _cy, float _X, float _Y, bool _Grav = true);

private:
	list<Obj*> onScreen[OBJ::END];
	list<Obj*> render[RENDER::END];

	Obj* player; 
	vector<Obj*> enemy[ENEMY::END];
	vector<Obj*> bullet[BULLET::END];
	vector<Obj*> item[ITEM::END];
	vector<Obj*> npc[NPC::END];
	vector<Obj*> block;
	vector<Obj*> vehicle[VEHICLE::END];

	bool	player_Dead;

public:
	static ObjPoolMgr* Get_Instance()
	{
		if (!pInstance)
			pInstance = new ObjPoolMgr;

		return pInstance;
	}

	static void	Destroy_Instance()
	{
		if (pInstance)
		{
			delete pInstance;
			pInstance = nullptr; 
		}
	}

private:
	static	ObjPoolMgr* pInstance;

};

