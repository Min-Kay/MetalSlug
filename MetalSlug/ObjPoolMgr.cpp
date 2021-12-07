#include "ObjPoolMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Manager.h"
#include "AbstractFactory.h"
#include "Bullet.h"
#include "HeavyBullet.h"
#include "Soldier.h"
#include "Items.h"
#include "Block.h"

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
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	for (int i = 0; i < OBJ::END; ++i)
	{
		list<Obj*>::iterator iter = onScreen[i].begin();
		for ( ; iter != onScreen[i].end();)
		{
			if ((*iter)->Get_Dead() || (!onScreen[OBJ::PLAYER].empty() && ((*iter)->Get_Info().x - onScreen[OBJ::PLAYER].front()->Get_Info().x > WINCX - 100.f - scrollX)))
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
	CCollisionMgr::Collision_Rect(onScreen[OBJ::PLAYER], onScreen[OBJ::BULLET]);
	CCollisionMgr::Collision_Rect(onScreen[OBJ::ENEMY], onScreen[OBJ::BULLET]);
	CCollisionMgr::Collision_Rect(onScreen[OBJ::PLAYER], onScreen[OBJ::PROP]);
	CCollisionMgr::Collision_Rect(onScreen[OBJ::BULLET], onScreen[OBJ::PROP]);
	CCollisionMgr::Collision_Rect(onScreen[OBJ::BULLET], onScreen[OBJ::BLOCK]);
	CCollisionMgr::Collision_RectPush(onScreen[OBJ::PLAYER],onScreen[OBJ::ENEMY]);
	CCollisionMgr::Collision_RectPush(onScreen[OBJ::PLAYER], onScreen[OBJ::BLOCK]);

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

void ObjPoolMgr::Spawn_Enemy(ENEMY::ID _enemy, float _X, float _Y, DIR::ID _dir, SOLDIER::CLASS _class)
{
	sort(enemy[_enemy].begin(), enemy[_enemy].end(), CompareDead<Obj*>);
	for (auto& i : enemy[_enemy])
	{
		if (i->Get_Dead())
		{
			i->Initialize();
			if (_enemy == ENEMY::SOLDIER)
				static_cast<Soldier*>(i)->Set_Class(_class);
			i->Set_Pos(_X, _Y);
			i->Set_Dir(_dir);
			i->Update_Rect();
			i->Set_Dead(false);
			Add_Object(OBJ::ENEMY, i);
			return;
		}
	}

	switch (_enemy)
	{
	case ENEMY::SOLDIER:
	{
		Obj* temp = CAbstractFactory<Soldier>::Create(_X, _Y, _dir);
		static_cast<Soldier*>(temp)->Set_Class(_class);
		enemy[_enemy].push_back(temp);
	}
		break;
	default:
		return;
	}

	Add_Object(OBJ::ENEMY, enemy[_enemy].back());
}

void ObjPoolMgr::Spawn_Bullet(BULLET::ID _bullet, float _X, float _Y, DIR::ID _dir, float _angle, OBJ::ID _parent)
{
	sort(bullet[_bullet].begin(), bullet[_bullet].end(), CompareDead<Obj*>);

	for (auto& i : bullet[_bullet])
	{
		if (i->Get_Dead())
		{
			i->Initialize();
			static_cast<Bullet*>(i)->Set_ParentID(_parent);
			i->Set_Pos(_X, _Y);
			i->Set_Dir(_dir);
			i->Set_Angle(_angle);
			i->Update_Rect();
			i->Set_Dead(false);
			Add_Object(OBJ::BULLET, i);
			return;
		}
	}

	Obj* temp = nullptr;
	switch (_bullet)
	{
	case BULLET::PISTOL:
	{
		temp = CAbstractFactory<Bullet>::Create(_X, _Y, _dir);
		static_cast<Bullet*>(temp)->Set_ParentID(_parent);
		bullet[_bullet].push_back(temp);
	}
		break;
	case BULLET::HEAVY:
	{
		temp = CAbstractFactory<Bullet>::Create(_X, _Y, _dir);
		static_cast<Bullet*>(temp)->Set_ParentID(_parent);
		bullet[_bullet].push_back(CAbstractFactory<HeavyBullet>::Create(_X, _Y, _dir, _angle));
	}
		break;
	case BULLET::ROCKET:
		//bullet[_bullet].push_back(CAbstractFactory<CFlowerBullet>::Create(_X, _Y, _dir));
		break;
	default:
		return;
	}

	Add_Object(OBJ::BULLET, bullet[_bullet].back());
}

void ObjPoolMgr::Spawn_Item(ITEM::ID _item, float _X, float _Y, WEAPON::ID _wep)
{
	sort(item[_item].begin(), item[_item].end(), CompareDead<Obj*>);

	for (auto& i : item[_item])
	{
		if (i->Get_Dead())
		{
			i->Initialize();
			if (_item == ITEM::WEAPON)
				static_cast<WepItem*>(i)->Set_WepID(_wep);
			i->Set_Pos(_X, _Y);
			i->Update_Rect();
			i->Set_Dead(false);
			Add_Object(OBJ::PROP, i);
			return;
		}
	}

	Obj* temp;
	switch (_item)
	{
	case ITEM::AMMO:
		item[_item].push_back(CAbstractFactory<AmmoBox>::Create(_X, _Y));
		break;
	case ITEM::GRENADE:
		item[_item].push_back(CAbstractFactory<GrenadeBox>::Create(_X, _Y));
		break;
	case ITEM::LIFE:
		item[_item].push_back(CAbstractFactory<LifeStone>::Create(_X, _Y));
		break;
	case ITEM::WEAPON:
	{
		switch (_wep)
		{
		case WEAPON::PISTOL:
			return;
		case WEAPON::HEAVY:
			temp = CAbstractFactory<WepItem>::Create(_X, _Y);
			static_cast<WepItem*>(temp)->Set_WepID(WEAPON::HEAVY);
			item[_item].push_back(temp);
			break;
		case WEAPON::ROCKET:
			temp = CAbstractFactory<WepItem>::Create(_X, _Y);
			static_cast<WepItem*>(temp)->Set_WepID(WEAPON::ROCKET);
			item[_item].push_back(temp);
			break;
		default:
			break;
		}
	}
		break;
	case ITEM::ITEMBOX:
		item[_item].push_back(CAbstractFactory<ItemBox>::Create(_X, _Y));
		break;
	default:
		return;
	}

	Add_Object(OBJ::PROP, item[_item].back());
}

void ObjPoolMgr::Spawn_Block(float _cx, float _cy, float _X, float _Y, bool _Grav)
{
	sort(block.begin(), block.end(), CompareDead<Obj*>);

	for (auto& i : block)
	{
		if (i->Get_Dead())
		{
			i->Set_Pos(_X, _Y);
			i->Set_Size(_cx,_cy);
			i->Initialize();
			i->Update_Rect();
			i->Set_Dead(false);
			Add_Object(OBJ::BLOCK, i);
			return;
		}
	}
	
	block.push_back(CAbstractFactory<Block>::Create(_X,_Y,_cx,_cy));

	Add_Object(OBJ::BLOCK, block.back());
}

void ObjPoolMgr::Set_Player_Wep(Weapon* _wep)
{
	if (onScreen[OBJ::PLAYER].empty())
		return;

	static_cast<Player*>(onScreen[OBJ::PLAYER].front())->Set_Weapon(_wep);
}

const INFO& ObjPoolMgr::Get_Player_Info() const
{
	INFO info{};
	if (onScreen[OBJ::PLAYER].empty())
		return info;


	return onScreen[OBJ::PLAYER].front()->Get_Info();
}

const RECT& ObjPoolMgr::Get_Player_Rect() const
{
	RECT rect{};
	if (onScreen[OBJ::PLAYER].empty())
		return rect;


	return onScreen[OBJ::PLAYER].front()->Get_Rect();
}



