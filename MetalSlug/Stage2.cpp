#include "framework.h"
#include "Stage2.h"
#include "Manager.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "Enemys.h"

Stage2::Stage2()
{
}

Stage2::~Stage2()
{
	Release();
}

void Stage2::Initialize()
{
	isClear = false;
	isFail = false;
	spawnMidBoss = false;

	checkPoint = false;

	Init_Information();

	ObjPoolMgr::Get_Instance()->Initialize();
	CLineMgr::Get_Instance()->Load(STAGE2_SAVE);

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map2-0.bmp",L"Map2-0");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map2-1.bmp", L"Map2-1");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map2-2.bmp", L"Map2-2");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map2-3.bmp", L"Map2-3");

	ObjPoolMgr::Get_Instance()->Spawn_Player(PLAYER_X, 100);

	scrollLock.push_back({ 4000,0 });
	scrollLock.push_back({ 8500,0 });
	scrollLock.push_back({ 9500,0 });
	scrollLock.push_back({ 11591,0 });

	CScrollMgr::Get_Instance()->Set_ScrollLockX((float)scrollLock.front().x);
	CScrollMgr::Get_Instance()->Set_ScrollY((float)scrollLock.front().y);

	maxCheckPoint = scrollLock.size();
	Set_CheckPoint_Objects();
}

void Stage2::Update()
{
	KeyInput();
	ObjPoolMgr::Get_Instance()->Update();
}

void Stage2::Late_Update()
{
	if (ObjPoolMgr::Get_Instance()->Get_Player_Dead())
	{
		if (DataMgr::Get_Instance()->Get_Life() > 0)
		{
			ObjPoolMgr::Get_Instance()->Spawn_Player(PLAYER_X - CScrollMgr::Get_Instance()->Get_ScrollX(), 100);
			ObjPoolMgr::Get_Instance()->Set_Player_Dead(false);
		}
		else
			isFail = true;

	}

	Check_Checkpoint_Clear();
	if (Check_GameState()) return;
	Check_Scrolling();

	ObjPoolMgr::Get_Instance()->Late_Update();
}

void Stage2::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (-scrollX <= 2700 + WINCX)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map2-0");
		GdiTransparentBlt(_hdc, scrollX, scrollY, 2700, 600, drawingDC, 0, 0, 1000, 218, MAP_COLOR);
	}

	if (-scrollX > 2700 - WINCX && -scrollX <= 5400 + WINCX)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map2-1");
		GdiTransparentBlt(_hdc, scrollX + 2700, scrollY, 2700, 600, drawingDC, 0, 0, 1000, 218, MAP_COLOR);
	}

	if (-scrollX > 5400 - WINCX && -scrollX <= 8100 + WINCX)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map2-2");
		GdiTransparentBlt(_hdc, scrollX + 5400, scrollY, 2700, 600, drawingDC, 0, 0, 1000, 218, MAP_COLOR);
	}

	if (-scrollX > 8100 - WINCX)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map2-3");
		GdiTransparentBlt(_hdc, scrollX + 8100, scrollY, 3491, 600, drawingDC, 0, 0, 1293, 218, MAP_COLOR);
	}

	CLineMgr::Get_Instance()->Render(_hdc);
	ObjPoolMgr::Get_Instance()->Render(_hdc);
	Render_Information(_hdc);

}

void Stage2::Release()
{
	CLineMgr::Get_Instance()->Release(); 
	ObjPoolMgr::Get_Instance()->DisableObj();
	CScrollMgr::Get_Instance()->Init_Scroll();
}

bool Stage2::Check_GameState()
{
	if (isClear)
	{
		SceneMgr::Get_Instance()->Change_Scene(SCENE::MENU);
		return true;
	}
	else if (isFail)
	{
		SceneMgr::Get_Instance()->Change_Scene(SCENE::MENU);
		return true;
	}

	return false;
}

void Stage2::Set_CheckPoint_Objects()
{
	if (isClear || isFail)
		return;

	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (currCheckPoint == maxCheckPoint && checkPoint)
		isClear = true;

	switch (currCheckPoint)
	{
	case 0:
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 1500, 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 1600, 300, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 2500, 300, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 3000, 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 3100, 300, DIR::LEFT);

		ObjPoolMgr::Get_Instance()->Spawn_Block(BLOCK::CAR, 1200, 300);
		ObjPoolMgr::Get_Instance()->Spawn_Block(BLOCK::CAR, 2000, 300);
		ObjPoolMgr::Get_Instance()->Spawn_Block(BLOCK::CAR, 2300, 300);

		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::ITEMBOX, 1200, 300);
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::ITEMBOX, 1700,300);
		break;
	case 1:
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 5000, 300, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 5500, 300, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 6000, 300, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 6500, 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 7000, 300, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 7500, 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 8000, 300, DIR::LEFT);

		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::DICOKKA, 8400, 300, DIR::LEFT);

		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::ITEMBOX, 5100, 300);
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::ITEMBOX, 6100, 300);
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::ITEMBOX, 7100, 300);
		break;
	case 2:
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::BAZUKA, 8800, -50, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::BAZUKA, 8900, -50, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::BAZUKA, 9000, -50, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::BAZUKA, 9100, -50, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SARUBIA, 9400, 300, DIR::LEFT);
		break;
	case 3:
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SARUBIA, 11000, 300, DIR::LEFT);
		break;
	}
	if (maxCheckPoint > currCheckPoint)
		++currCheckPoint;
}
