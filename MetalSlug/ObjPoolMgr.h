#pragma once
#include "Include.h"

class Obj;

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

public:
	void	Spawn_Enemy(ENEMY::ID _enemy, float _X, float _Y, DIR::ID _dir = DIR::LEFT);
	void	Spawn_Bullet(BULLET::ID _bullet, float _X, float _Y, DIR::ID _dir = DIR::RIGHT);
	void	Spawn_Item(ITEM::ID _item, float _X, float _Y, DIR::ID _dir = DIR::RIGHT);

private:
	list<Obj*> onScreen[OBJ::END];
	list<Obj*> render[RENDER::END];

	vector<Obj*> enemy[ENEMY::END];
	vector<Obj*> bullet[BULLET::END];
	vector<Obj*> item[ITEM::END];
	vector<Obj*> npc[NPC::END];
	vector<Obj*> vehicle[VEHICLE::END];

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

