#include "ObjPoolMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Manager.h"
#include "AbstractFactory.h"
#include "Bullets.h"
#include "Items.h"
#include "Blocks.h"
#include "Enemys.h"
#include "Npc.h"

ObjPoolMgr* ObjPoolMgr::pInstance = nullptr;

ObjPoolMgr::ObjPoolMgr()
{
	player = nullptr;
	player_Dead = false;
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
	SAFE_DELETE(player);

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
	
	for (int i = 0; i < BLOCK::END; ++i)
	{
		for_each(block[i].begin(), block[i].end(), CDeleteObj());
		block[i].clear();
	}
}

void ObjPoolMgr::DisableObj()
{
	for (int i = 0; i < OBJ::END; ++i)
	{
		onScreen[i].erase(onScreen[i].begin(), onScreen[i].end());
		onScreen[i].clear();
	}

	for (int i = 0; i < ENEMY::END; ++i)
	{
		for_each(enemy[i].begin(), enemy[i].end(), KillObj());
	}

	for (int i = 0; i < BULLET::END; ++i)
	{
		for_each(bullet[i].begin(), bullet[i].end(), KillObj());
	}
	for (int i = 0; i < ITEM::END; ++i)
	{
		for_each(item[i].begin(), item[i].end(), KillObj());
	}
	for (int i = 0; i < NPC::END; ++i)
	{
		for_each(npc[i].begin(), npc[i].end(), KillObj());
	}

	for (int i = 0; i < VEHICLE::END; ++i)
	{
		for_each(vehicle[i].begin(), vehicle[i].end(), KillObj());
	}

	for (int i = 0; i < BLOCK::END; ++i)
	{
		for_each(block[i].begin(), block[i].end(), KillObj());
	}

	SAFE_DELETE(player);
}

void ObjPoolMgr::Add_Object(OBJ::ID _id, Obj* _obj)
{
	onScreen[_id].push_back(_obj);
}

void ObjPoolMgr::Delete_Object(OBJ::ID _id, Obj* _obj)
{
	auto iter = find(onScreen[_id].begin(), onScreen[_id].end(), _obj); 
	if (iter != onScreen[_id].end())
	{
		BlockMgr::Get_Instance()->Delete_Block(*iter);
		onScreen[_id].erase(iter);
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
			BlockMgr::Get_Instance()->Add_Block(temp);
			Add_Object(OBJ::ENEMY, temp);
			return;
		}
	}
	
	Obj* temp = nullptr;
	switch (_enemy)
	{
	case ENEMY::SOLDIER:
		temp = CAbstractFactory<Soldier>::Create(_X, _Y, _dir);
		break;
	case ENEMY::ARABIAN:
		temp = CAbstractFactory<Arabian>::Create(_X, _Y, _dir);
		break;
	case ENEMY::THREEHEAD:
		temp = CAbstractFactory<ThreeHead>::Create(_X, _Y, _dir);
		break;
	case ENEMY::MASKNELL:
		temp = CAbstractFactory<Masknell>::Create(_X, _Y, _dir);
		break;
	case ENEMY::SOLDAE:
		temp = CAbstractFactory<SolDae>::Create(_X, _Y, _dir);
		break;
	case ENEMY::SARUBIA:
		temp = CAbstractFactory<Sarubia>::Create(_X, _Y, _dir);
		break;
	case ENEMY::BAZUKA:
		temp = CAbstractFactory<Bazuka>::Create(_X, _Y, _dir);
		break;
	case ENEMY::DICOKKA:
		temp = CAbstractFactory<DiCokka>::Create(_X, _Y, _dir);
		break;
	default:
		return;
	}
	BlockMgr::Get_Instance()->Add_Block(temp);
	enemy[_enemy].push_back(temp);
	Add_Object(OBJ::ENEMY, temp);
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
		temp = CAbstractFactory<Bullet>::Create(_X, _Y, _dir);
		break;
	case BULLET::HEAVY:
		temp = CAbstractFactory<HeavyBullet>::Create(_X, _Y, _dir);
		break;
	case BULLET::SHOTGUN:
		temp = CAbstractFactory<ShotgunBullet>::Create(_X, _Y, _dir);
		break;
	case BULLET::ROCKET:
		temp = CAbstractFactory<RocketBullet>::Create(_X, _Y, _dir);
		break;
	case BULLET::ENEMYBULLET:
		temp = CAbstractFactory<EnemyBullet>::Create(_X, _Y, _dir);
		break;
	case BULLET::SOLDAEROOT:
		temp = CAbstractFactory<SolDaeRoot>::Create(_X, _Y, _dir);
		break;
	case BULLET::SARUBIA:
		temp = CAbstractFactory<SarubiaBullet>::Create(_X, _Y, _dir);
		break;
	case BULLET::BAZUKA:
		temp = CAbstractFactory<BazukaBullet>::Create(_X, _Y, _dir);
		break;
	case BULLET::DICOKKA:
		temp = CAbstractFactory<DiCokkaBullet>::Create(_X, _Y, _dir);
		break;
	case BULLET::IRONLIZARD:
		temp = CAbstractFactory<IronlizardBullet>::Create(_X, _Y, _dir);
		break;
	default:
		return;
	}
	static_cast<Bullet*>(temp)->Set_ParentID(_parent);
	bullet[_bullet].push_back(temp);
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
		temp = CAbstractFactory<WepItem>::Create(_X, _Y);
		static_cast<WepItem*>(temp)->Set_WepID(_wep);
		item[_item].push_back(temp);
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

void ObjPoolMgr::Spawn_Block(BLOCK::ID _block, float _X, float _Y, bool _Grav)
{
	if (!block[_block].empty())
	{
		sort(block[_block].begin(), block[_block].end(), CompareDead<Obj*>);
		auto& temp = block[_block].front();
		if(temp->Get_Dead())
		{
			temp->Set_Pos(_X, _Y);
			temp->Initialize();
			temp->Update_Rect();
			static_cast<Block*>(temp)->Set_Gravity(_Grav);
			temp->Set_Dead(false);
			BlockMgr::Get_Instance()->Add_Block(temp);
			Add_Object(OBJ::BLOCK, temp);
			return;
		}
	}
	Obj* temp = nullptr; 
	switch (_block)
	{
	case BLOCK::CAR:
		temp = CAbstractFactory<Car>::Create(_X, _Y);
		break;
	default:
		return;
	}
	static_cast<Block*>(temp)->Set_Gravity(_Grav);
	block[_block].push_back(temp);
	BlockMgr::Get_Instance()->Add_Block(temp);
	Add_Object(OBJ::BLOCK, temp);
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
	default:
		return;
	}
	
	Add_Object(OBJ::NPC, npc[_npc].back());
}

const list<Obj*>& ObjPoolMgr::Get_OnScreenObj(OBJ::ID _obj) const
{
	return onScreen[_obj];
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



