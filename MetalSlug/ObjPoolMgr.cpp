#include "ObjPoolMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Manager.h"
#include "AbstractFactory.h"
#include "Bullets.h"
#include "Items.h"
#include "Block.h"
#include "Enemys.h"
#include "Npc.h"

ObjPoolMgr* ObjPoolMgr::pInstance = nullptr;

ObjPoolMgr::ObjPoolMgr()
{
}

ObjPoolMgr::~ObjPoolMgr()
{
	Release();
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
			if ((*iter)->Get_Dead())
			{
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
	CCollisionMgr::Collision_Rect(onScreen[OBJ::PLAYER], onScreen[OBJ::ENEMY]);
	CCollisionMgr::Collision_Rect(onScreen[OBJ::ENEMY], onScreen[OBJ::BULLET]);
	CCollisionMgr::Collision_Rect(onScreen[OBJ::PLAYER], onScreen[OBJ::PROP]);
	CCollisionMgr::Collision_Rect(onScreen[OBJ::PLAYER], onScreen[OBJ::NPC]);
	CCollisionMgr::Collision_Rect(onScreen[OBJ::BULLET], onScreen[OBJ::PROP]);
	CCollisionMgr::Collision_Rect(onScreen[OBJ::BULLET], onScreen[OBJ::BLOCK]);
	CCollisionMgr::Collision_Rect(onScreen[OBJ::BULLET], onScreen[OBJ::NPC]);
	CCollisionMgr::Collision_RectPush(onScreen[OBJ::PLAYER],onScreen[OBJ::ENEMY]);
	CCollisionMgr::Collision_RectPush(onScreen[OBJ::PLAYER], onScreen[OBJ::BLOCK]);

	for (int i = 0; i < OBJ::END; ++i)
	{
		for (auto& iter : onScreen[i])
		{
			iter->Late_Update();
		}
	}
}

void ObjPoolMgr::Render(HDC _hdc)
{
	for (int i = 0; i < OBJ::END; ++i)
	{
		for (auto& iter : onScreen[i])
		{
			iter->Render(_hdc);
		}
	}
}

void ObjPoolMgr::Release()
{
	for (int i = 0; i < OBJ::END; ++i)
	{
		onScreen[i].clear();
	}

	for (int i = 0; i < ENEMY::END; ++i)
	{
		for_each(enemy[i].begin(), enemy[i].end(), CDeleteObj());
		enemy[i].clear();
	}

	for (int i = 0; i < BULLET::END; ++i)
	{
		for_each(bullet[i].begin(), bullet[i].end(), CDeleteObj());
		bullet[i].clear();
	}
	for (int i = 0; i < ITEM::END; ++i)
	{
		for_each(item[i].begin(), item[i].end(), CDeleteObj());
		item[i].clear();
	}
	for (int i = 0; i < NPC::END; ++i)
	{
		for_each(npc[i].begin(), npc[i].end(), CDeleteObj());
		npc[i].clear();
	}

	for (int i = 0; i < VEHICLE::END; ++i)
	{
		for_each(vehicle[i].begin(), vehicle[i].end(), CDeleteObj());
		vehicle[i].clear();
	}
	
	for_each(block.begin(),block.end(),CDeleteObj());
	block.clear(); 

	SAFE_DELETE(player);
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

void ObjPoolMgr::Delete_Object(OBJ::ID _id, Obj* _obj)
{
	if (find(onScreen[_id].begin(), onScreen[_id].end(), _obj) != onScreen[_id].end())
	{
		onScreen[_id].erase(find(onScreen[_id].begin(), onScreen[_id].end(), _obj));
	}
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

void ObjPoolMgr::Spawn_Enemy(ENEMY::ID _enemy, float _X, float _Y, DIR::ID _dir)
{
	if (!enemy[_enemy].empty())
	{
		sort(enemy[_enemy].begin(), enemy[_enemy].end(), CompareDead<Obj*>);

		auto& temp = enemy[_enemy].front(); 
		if(temp->Get_Dead())
		{
			temp->Initialize();
			temp->Set_Pos(_X, _Y);
			temp->Set_Dir(_dir);
			temp->Update_Rect();
			temp->Set_Dead(false);
			Add_Object(OBJ::ENEMY, temp);
			return;
		}
	}
	
	switch (_enemy)
	{
	case ENEMY::SOLDIER:
		enemy[_enemy].push_back(CAbstractFactory<Soldier>::Create(_X, _Y, _dir));
		break;
	case ENEMY::ARABIAN:
		enemy[_enemy].push_back(CAbstractFactory<Arabian>::Create(_X,_Y,_dir));
		break;
	case ENEMY::THREEHEAD:
		enemy[_enemy].push_back(CAbstractFactory<ThreeHead>::Create(_X, _Y));
		break;
	case ENEMY::MASKNELL:
		enemy[_enemy].push_back(CAbstractFactory<Masknell>::Create(_X, _Y));
		break;
	case ENEMY::SOLDAE:
		enemy[_enemy].push_back(CAbstractFactory<SolDae>::Create(_X, _Y));
		break;
	default:
		return;
	}

	Add_Object(OBJ::ENEMY, enemy[_enemy].back());
}

void ObjPoolMgr::Spawn_Bullet(BULLET::ID _bullet, float _X, float _Y, DIR::ID _dir, OBJ::ID _parent)
{
	if (!bullet[_bullet].empty())
	{
		sort(bullet[_bullet].begin(), bullet[_bullet].end(), CompareDead<Obj*>);

		auto& temp = bullet[_bullet].front();

		if (temp->Get_Dead())
		{
			temp->Initialize();
			static_cast<Bullet*>(temp)->Set_ParentID(_parent);
			temp->Set_Pos(_X, _Y);
			temp->Set_Dir(_dir);
			temp->Update_Rect();
			temp->Set_Dead(false);
			Add_Object(OBJ::BULLET, temp);
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
		bullet[_bullet].push_back(CAbstractFactory<HeavyBullet>::Create(_X, _Y, _dir));
	}
		break;
	case BULLET::SHOTGUN:
	{
		temp = CAbstractFactory<Bullet>::Create(_X, _Y, _dir);
		static_cast<Bullet*>(temp)->Set_ParentID(_parent);
		bullet[_bullet].push_back(CAbstractFactory<ShotgunBullet>::Create(_X, _Y, _dir));
	}
	break;
	default:
		return;
	}

	Add_Object(OBJ::BULLET, bullet[_bullet].back());
}

void ObjPoolMgr::Spawn_Item(ITEM::ID _item, float _X, float _Y, WEAPON::ID _wep)
{
	if (!item[_item].empty())
	{
		sort(item[_item].begin(), item[_item].end(), CompareDead<Obj*>);
		auto& temp = item[_item].front();
		if (temp->Get_Dead())
		{
			temp->Initialize();
			if (_item == ITEM::WEAPON)
				static_cast<WepItem*>(temp)->Set_WepID(_wep);
			temp->Set_Pos(_X, _Y);
			temp->Update_Rect();
			temp->Set_Dead(false);
			Add_Object(OBJ::PROP, temp);
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
		case WEAPON::SHOTGUN:
			temp = CAbstractFactory<WepItem>::Create(_X, _Y);
			static_cast<WepItem*>(temp)->Set_WepID(WEAPON::SHOTGUN);
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
	if (!block.empty())
	{
		sort(block.begin(), block.end(), CompareDead<Obj*>);
		auto& temp = block.front();
		if(temp->Get_Dead())
		{
			temp->Set_Pos(_X, _Y);
			temp->Set_Size(_cx, _cy);
			temp->Initialize();
			temp->Update_Rect();
			temp->Set_Dead(false);
			Add_Object(OBJ::BLOCK, temp);
			return;
		}
	}
	
	block.push_back(CAbstractFactory<Block>::Create(_X,_Y,_cx,_cy));

	Add_Object(OBJ::BLOCK, block.back());
}

void ObjPoolMgr::Spawn_Npc(NPC::ID _npc, float _X, float _Y)
{
	

	if(!npc[_npc].empty())
	{
		sort(npc[_npc].begin(), npc[_npc].end(), CompareDead<Obj*>);
		auto& temp = npc[_npc].front();
		if (temp->Get_Dead())
		{
			temp->Set_Pos(_X, _Y);
			temp->Initialize();
			temp->Update_Rect();
			temp->Set_Dead(false);
			Add_Object(OBJ::NPC, temp);
			return;
		}
	}

	switch (_npc)
	{
	case NPC::SLAVE:
		npc[_npc].push_back(CAbstractFactory<Npc>::Create(_X, _Y, DIR::LEFT));
		break;
	case NPC::TRAVELER:
		break;
	case NPC::END:
		break;
	default:
		return;
	}
	
	Add_Object(OBJ::NPC, npc[_npc].back());
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



