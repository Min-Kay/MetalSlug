#include "ObjPoolMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Manager.h"
#include "AbstractFactory.h"
#include "Bullet.h"
#include "HeavyBullet.h"

ObjPoolMgr* ObjPoolMgr::pInstance = nullptr;

ObjPoolMgr::ObjPoolMgr()
{
}

ObjPoolMgr::~ObjPoolMgr()
{
}

void ObjPoolMgr::Initialize()
{
	player = nullptr; 
	player_Dead = false;
}

void ObjPoolMgr::Update()
{
	for (int i = 0; i < OBJ::END; ++i)
	{
		list<Obj*>::iterator iter = onScreen[i].begin();
		for ( ; iter != onScreen[i].end();)
		{
			if ((*iter)->Get_Dead() || ((*iter)->Get_Info().x - onScreen[OBJ::PLAYER].front()->Get_Info().x > WINCX - 100.f))
			{
				(*iter)->Set_Dead(true);
				iter = onScreen[i].erase(iter);
				continue;
			}
			else
			{
				(*iter)->Update();
				++iter; 
			}
		}
	}
}

void ObjPoolMgr::Late_Update()
{
	for (int i = 0; i < OBJ::END; ++i)
	{
		for (auto& iter : onScreen[i])
		{
			iter->Late_Update();
			render[iter->Get_RenderId()].push_back(iter);
		}
	}
}

void ObjPoolMgr::Render(HDC _hdc)
{
	for (int i = 0; i < RENDER::END; ++i)
	{
		render[i].sort(CompareY<Obj*>);

		for (auto& iter : render[i])
		{
			iter->Render(_hdc);
		}
		render[i].clear(); 
	}
}

void ObjPoolMgr::Release()
{

}

void ObjPoolMgr::DisableObj()
{
	for (int i = 0; i < BULLET::END; ++i)
	{
		for_each(bullet[i].begin(), bullet[i].end(), KillObj());
	}

	for (int i = 0; i < OBJ::END; ++i)
	{
		onScreen[i].erase(onScreen[i].begin(), onScreen[i].end());
		onScreen[i].clear(); 
	}	
}

void ObjPoolMgr::Add_Object(OBJ::ID _id, Obj* _obj)
{
	onScreen[_id].push_back(_obj);
}

float ObjPoolMgr::Check_Distance(Obj* _target)
{
	if (onScreen[OBJ::PLAYER].empty())
		return 10000;

	float x = _target->Get_Info().x - onScreen[OBJ::PLAYER].front()->Get_Info().x;
	float y = _target->Get_Info().y - onScreen[OBJ::PLAYER].front()->Get_Info().y;

	return sqrtf(x * x + y * y);
}

void ObjPoolMgr::Spawn_Player(float _X, float _Y)
{
	if (player)
	{
		player->Set_Pos(_X,_Y);
		player->Initialize(); 
	}
	else
	{
		player = new Player;
		player->Set_Pos(_X, _Y);
		player->Initialize();
	}
	Add_Object(OBJ::PLAYER,player);
}

void ObjPoolMgr::Spawn_Bullet(BULLET::ID _bullet, float _X, float _Y, DIR::ID _dir, float _angle)
{
	sort(bullet[_bullet].begin(), bullet[_bullet].end(), CompareDead<Obj*>);

	for (auto& i : bullet[_bullet])
	{
		if (i->Get_Dead())
		{
			i->Initialize();
			i->Set_Pos(_X, _Y);
			i->Set_Dir(_dir);
			i->Set_Angle(_angle);
			i->Update_Rect();
			i->Set_Dead(false);
			Add_Object(OBJ::BULLET, i);
			return;
		}
		else
			break;
	}

	switch (_bullet)
	{
	case BULLET::PISTOL:
		bullet[_bullet].push_back(CAbstractFactory<Bullet>::Create(_X, _Y, _dir));
		break;
	case BULLET::HEAVY:
		bullet[_bullet].push_back(CAbstractFactory<HeavyBullet>::Create(_X, _Y, _dir,_angle));
		break;
	case BULLET::ROCKET:
		//bullet[_bullet].push_back(CAbstractFactory<CFlowerBullet>::Create(_X, _Y, _dir));
		break;
	default:
		return;
	}

	Add_Object(OBJ::BULLET, bullet[_bullet].back());
}

const INFO& ObjPoolMgr::Get_Player_Info() const
{
	INFO info{};
	if (onScreen[OBJ::PLAYER].empty())
		return info;


	return onScreen[OBJ::PLAYER].front()->Get_Info();
}



