#include "ObjPoolMgr.h"
#include "Obj.h"
#include "Manager.h"
#include "AbstractFactory.h"

ObjPoolMgr* ObjPoolMgr::pInstance = nullptr;

ObjPoolMgr::ObjPoolMgr()
{
}

ObjPoolMgr::~ObjPoolMgr()
{
}

void ObjPoolMgr::Initialize()
{
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

//void ObjPoolMgr::Spawn_Bullet(BULLET::ID _bullet, float _X, float _Y, DIR::ID _dir)
//{
//	for (auto& i : bullet[_bullet])
//	{
//		if (i->Get_Dead())
//		{
//			i->Initialize();
//			i->Set_Pos(_X, _Y);
//			i->Set_Dir(_dir);
//			i->Update_Rect();
//			i->Set_Dead(false);
//			return;
//		}
//	}
//
//	switch (_bullet)
//	{
//	case BULLET::PISTOL:
//		bullet_pool[_bullet].push_back(CAbstractFactory<Bullet>::Create(_X, _Y, _dir));
//		break;
//	case BULLET::HEAVY:
//		bullet_pool[_bullet].push_back(CAbstractFactory<CKoopa_bullet>::Create(_X, _Y, _dir));
//		break;
//	case BULLET::ROCKET:
//		bullet_pool[_bullet].push_back(CAbstractFactory<CFlowerBullet>::Create(_X, _Y, _dir));
//		break;
//	default:
//		return;
//	}
//
//	Add_Object(OBJ::BULLET, bullet_pool[_bullet].back());
//}
//
//void ObjPoolMgr::Spawn_Item(ITEM::ID _item, float _X, float _Y, DIR::DIR _dir)
//{
//	for (auto& i : item_pool[_item])
//	{
//		if (i->Get_Dead())
//		{
//			i->Initialize();
//			i->Set_Pos(_X, _Y);
//			i->Set_Direction(_dir);
//			i->Update_Rect();
//			i->Set_Dead(false);
//			CObjMgr::Get_Instance()->Add_Object(OBJ::ITEM, i);
//			return;
//		}
//	}
//
//	switch (_item)
//	{
//	case ITEM::COIN:
//		item_pool[_item].push_back(CAbstractFactory<CCoin>::Create(_X, _Y));
//		break;
//	case ITEM::MUSHROOM:
//		item_pool[_item].push_back(CAbstractFactory<CMushroom>::Create(_X, _Y));
//		break;
//	case ITEM::FLOWER:
//		item_pool[_item].push_back(CAbstractFactory<CFlower>::Create(_X, _Y));
//		break;
//	default:
//		return;
//	}
//
//	CObjMgr::Get_Instance()->Add_Object(OBJ::ITEM, item_pool[_item].back());
//}
//
//void ObjPoolMgr::Spawn_Obstacle(BLOCK::ID _item, float _X, float _Y)
//{
//	for (auto& i : obstacle_pool[_item])
//	{
//		if (i->Get_Dead())
//		{
//			i->Initialize();
//			i->Set_Pos(_X, _Y);
//			i->Update_Rect();
//			i->Set_Dead(false);
//			CObjMgr::Get_Instance()->Add_Object(OBJ::OBSTACLE, i);
//			return;
//		}
//	}
//
//	switch (_item)
//	{
//	case BLOCK::DEFAULT:
//		obstacle_pool[_item].push_back(CAbstractFactory<CBlock>::Create(_X, _Y));
//		break;
//	case BLOCK::ITEM:
//		obstacle_pool[_item].push_back(CAbstractFactory<CItemBlock>::Create(_X, _Y));
//		break;
//	case BLOCK::INVISIBLE:
//	{
//		CObj* temp = CAbstractFactory<CItemBlock>::Create(_X, _Y);
//		static_cast<CItemBlock*>(temp)->Setup(BLOCK::INVISIBLE,true);
//		obstacle_pool[_item].push_back(temp);
//	}
//		break;
//	case BLOCK::BREAKABLE:
//		obstacle_pool[_item].push_back(CAbstractFactory<CBreakableBlock>::Create(_X, _Y));
//		break;
//	case BLOCK::PIPE:
//		obstacle_pool[_item].push_back(CAbstractFactory<CPipe>::Create(_X,_Y));
//		break;
//	default:
//		return;
//	}
//
//	CObjMgr::Get_Instance()->Add_Object(OBJ::OBSTACLE, obstacle_pool[_item].back());
//}
//
//void ObjPoolMgr::Load_Obstacles(const TCHAR* _file)
//{
//	HANDLE			hFile = CreateFile(_file, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	DWORD		dwByte = 0;
//	BLOCKINFO	binfo{};
//
//	while (true)
//	{
//		ReadFile(hFile, &binfo, sizeof(BLOCKINFO), &dwByte, NULL);
//
//		if (0 == dwByte)
//			break;
//
//		Spawn_Obstacle(binfo.id,binfo.pos.x, binfo.pos.y);
//	}
//
//	CloseHandle(hFile);
//}

