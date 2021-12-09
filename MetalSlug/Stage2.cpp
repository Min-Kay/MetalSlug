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

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/background.bmp", L"background");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-0.bmp", L"Map1-0");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-1.bmp", L"Map1-1");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-2.bmp", L"Map1-2");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-3.bmp", L"Map1-3");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MidBoss.bmp", L"MidBoss");

	ObjPoolMgr::Get_Instance()->Initialize();
	//DataMgr::Get_Instance()->Initialize();
	CLineMgr::Get_Instance()->Load(STAGE1_SAVE);
	ObjPoolMgr::Get_Instance()->Spawn_Player(PLAYER_X, 100);

	scrollLock.push_back({ 2000,-60 });
	scrollLock.push_back({ 3000,-60 });
	scrollLock.push_back({ 4600,-60 });
	scrollLock.push_back({ 5630,-60 });
	scrollLock.push_back({ 8000,-100 });
	scrollLock.push_back({ 8800,-100 });
	scrollLock.push_back({ 9500,-100 });


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

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"background");
	GdiTransparentBlt(_hdc, 0, 0, WINCX, WINCY, drawingDC, 0, 0, 100, 75, MAP_COLOR);

	if (-scrollX < 2500 + WINCX)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map1-0");
		GdiTransparentBlt(_hdc, scrollX, scrollY, 2500, 700, drawingDC, 0, 0, 1000, 280, MAP_COLOR);
	}

	if (-scrollX > 2500 - WINCX && -scrollX < 5000 + WINCX)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map1-1");
		GdiTransparentBlt(_hdc, scrollX + 2500, scrollY, 2500, 700, drawingDC, 0, 0, 1000, 280, MAP_COLOR);
	}

	if (-scrollX > 5000 - WINCX && -scrollX < 7500 + WINCX)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map1-2");
		GdiTransparentBlt(_hdc, scrollX + 5000, scrollY, 2500, 700, drawingDC, 0, 0, 1000, 280, MAP_COLOR);
	}

	if (spawnMidBoss && -scrollX > 5000 - WINCX && -scrollX < 7500 + WINCX)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"MidBoss");
		GdiTransparentBlt(_hdc, scrollX + 4650, scrollY + 250, 1026, 324, drawingDC, 0, 3394, 380, 120, RGB(0, 255, 0));
	}

	if (-scrollX > 7500 - WINCX && -scrollX < 9000 + WINCX)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map1-3");
		GdiTransparentBlt(_hdc, scrollX + 7500, scrollY, 2050, 700, drawingDC, 0, 0, 820, 280, MAP_COLOR);
	}

	CLineMgr::Get_Instance()->Render(_hdc);
	ObjPoolMgr::Get_Instance()->Render(_hdc);
	Render_Information(_hdc);

}

void Stage2::Release()
{
	ObjPoolMgr::Get_Instance()->DisableObj();
	CScrollMgr::Get_Instance()->Init_Scroll();
}



