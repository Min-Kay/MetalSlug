#include "framework.h"
#include "Game.h"
#include "Manager.h"
#include "AbstractFactory.h"
#include "Player.h"

void Game::Initialize()
{
	isClear = false; 
	isFail = false;

	checkPoint = false;

	Init_Information();

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/background.bmp", L"background");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-0.bmp",L"Map1-0");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-1.bmp", L"Map1-1");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-2.bmp", L"Map1-2");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-3.bmp", L"Map1-3");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-building.bmp", L"Map1-building");

	ObjPoolMgr::Get_Instance()->Initialize();
	DataMgr::Get_Instance()->Initialize(); 
	CScrollMgr::Get_Instance()->Set_ScrollLockX(9550);
	CScrollMgr::Get_Instance()->Set_ScrollLockY(700);
	CLineMgr::Get_Instance()->Load(STAGE1_SAVE);
	ObjPoolMgr::Get_Instance()->Spawn_Player(200,100);
	ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER,300,300,DIR::RIGHT,SOLDIER::PRIVATE);
	ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 500, 300, DIR::RIGHT, SOLDIER::SERGENT);
	ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::WEAPON, 300, 300, WEAPON::HEAVY);


	//ObjPoolMgr::Get_Instance()->Spawn_Block(100, 100, 400, 400, false);

}

void Game::Update()
{
	KeyInput();
	ObjPoolMgr::Get_Instance()->Update();
}

void Game::Late_Update()
{
	if (ObjPoolMgr::Get_Instance()->Get_Player_Dead())
	{
		if (DataMgr::Get_Instance()->Get_Life() > 0)
		{
			ObjPoolMgr::Get_Instance()->Spawn_Player(200 - CScrollMgr::Get_Instance()->Get_ScrollX(), 100);
			ObjPoolMgr::Get_Instance()->Set_Player_Dead(false);
		}
		else
			isFail = true;

	}

	Check_GameState();

	ObjPoolMgr::Get_Instance()->Late_Update();
}

void Game::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"background");
	GdiTransparentBlt(_hdc, 0, 0, WINCX, WINCY, drawingDC, 0, 0, 100, 75, MAP_COLOR);

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map1-0");
	GdiTransparentBlt(_hdc, scrollX,  scrollY, 2500, 700, drawingDC, 0, 0, 1000, 280, MAP_COLOR);

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map1-1");
	GdiTransparentBlt(_hdc, scrollX + 2500, scrollY, 2500, 700, drawingDC, 0, 0, 1000, 280, MAP_COLOR);

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map1-2");
	GdiTransparentBlt(_hdc, scrollX + 5000, scrollY, 2500, 700, drawingDC, 0, 0, 1000, 280, MAP_COLOR);

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map1-building");
	GdiTransparentBlt(_hdc, scrollX + 4550, scrollY + 200, 1200, 450, drawingDC, 0, 0, 400, 150, MAP_COLOR);

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Map1-3");
	GdiTransparentBlt(_hdc, scrollX + 7500, scrollY, 2050, 700, drawingDC, 0, 0, 820, 280, MAP_COLOR);
	

	CLineMgr::Get_Instance()->Render(_hdc);
	ObjPoolMgr::Get_Instance()->Render(_hdc);
	Render_Information(_hdc); 

}

void Game::Release()
{
	ObjPoolMgr::Get_Instance()->DisableObj();
	CScrollMgr::Get_Instance()->Init_Scroll(0, 0);
}

void Game::KeyInput()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
	{
		isClear = true;
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
	{
		isFail = true;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('Q'))
	{
		CScrollMgr::Get_Instance()->Set_ScrollX(50.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
	{
		CScrollMgr::Get_Instance()->Set_ScrollX(-50.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('E'))
	{
		CScrollMgr::Get_Instance()->Set_ScrollY(50.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('R'))
	{
		CScrollMgr::Get_Instance()->Set_ScrollY(-50.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('1'))
	{
		DataMgr::Get_Instance()->Add_Grenade(10);
		DataMgr::Get_Instance()->Add_Life(10);
		DataMgr::Get_Instance()->Add_Score(100);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('2'))
	{
		DataMgr::Get_Instance()->Add_Grenade(-10);
		DataMgr::Get_Instance()->Add_Life(-10);
		DataMgr::Get_Instance()->Add_Score(-100);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('3'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::WEAPON, rand() % 200 + 100, 300, WEAPON::ROCKET);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('4'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::WEAPON, rand() % 200 + 100, 300, WEAPON::HEAVY);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('5'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::AMMO, rand() % 200 + 100, 300);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('6'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::GRENADE, rand() % 200 + 100, 300);
	}
	if (CKeyMgr::Get_Instance()->Key_Down('7'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::LIFE, rand() % 200 + 100, 300);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('8'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::ITEMBOX, rand() % 200 + 100, 300);
	}
}

void Game::Check_GameState()
{
	if (isClear)
		SceneMgr::Get_Instance()->Change_Scene(SCENE::GAME1);
	else if(isFail)
		SceneMgr::Get_Instance()->Change_Scene(SCENE::MENU);
}

void Game::Init_Information()
{
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Life_Bar.bmp",L"BAR");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Life_Bullet.bmp", L"UI");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/number.bmp", L"NUMBER");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Infinite.bmp", L"Infinite");


	uiRect = { 10,10,310,74 };
}

void Game::Render_Information(HDC _hdc)
{
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"UI");
	GdiTransparentBlt(_hdc, int(uiRect.left), int(uiRect.top), 300, 64, drawingDC, 0, 0, 300, 64, RGB(0,255,0));

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"BAR");
	BitBlt(_hdc,17,35,119,12,drawingDC,0,0,SRCCOPY);

	swprintf_s(scoreCount, _T("%d"), DataMgr::Get_Instance()->Get_Score());
	swprintf_s(grenadeCount, _T("%d"), DataMgr::Get_Instance()->Get_Grenade());
	swprintf_s(bulletCount, _T("%d"), DataMgr::Get_Instance()->Get_Ammo());
	swprintf_s(lifeCount, _T("%d"), DataMgr::Get_Instance()->Get_Life());

	int size = 0;

	// score
	size = Check_NumSize(DataMgr::Get_Instance()->Get_Score());
	for (int j = 0; j < size; ++j)
	{
		int num = Check_Number(scoreCount[size - 1 - j]);
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"NUMBER");
		GdiTransparentBlt(_hdc, 20 * (size - 1 - j) + 70 - ((size - 1) * 20), -25, 100, 100,drawingDC, 100 * num, 0, 100, 100, RGB(255, 255, 255));
	}

	// bullet
	size = Check_NumSize(DataMgr::Get_Instance()->Get_Ammo());
	for (int j = 0; j < size; ++j)
	{
		int num = Check_Number(bulletCount[size - 1 -j]);
		if (num == -1)
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Infinite");
			GdiTransparentBlt(_hdc, 170, 5, 75, 75,
				drawingDC, 0, 0, 100, 100, RGB(255, 255, 255));
			break; 
		}
		else
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"NUMBER");
			GdiTransparentBlt(_hdc, 20 * (size - 1 -j) + 170 - ((size - 1)* 20), -8, 100, 100,
				drawingDC, 100 * num, 0, 100, 100, RGB(255, 255, 255));
		}
	}

	// life
	size = Check_NumSize(DataMgr::Get_Instance()->Get_Life());
	for (int j = 0; j < size; ++j)
	{
		int num = Check_Number(lifeCount[size - 1 - j]);
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"NUMBER");
		GdiTransparentBlt(_hdc, 20 * (size - 1 - j) + 75 - ((size - 1) * 20), 12, 100, 100,
			drawingDC, 100 * num, 0, 100, 100, RGB(255, 255, 255));
	}

	// grenade
	size = Check_NumSize(DataMgr::Get_Instance()->Get_Grenade());
	for (int j = 0; j < size; ++j)
	{
		int num = Check_Number(grenadeCount[size - 1 - j]);
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"NUMBER");
		GdiTransparentBlt(_hdc, 20 * (size - 1 - j) + 230 - ((size - 1) * 20), -8, 100, 100,
			drawingDC, 100 * num, 0, 100, 100, RGB(255, 255, 255));
	}
}

int Game::Check_Number(TCHAR name)
{
	TCHAR temp[2] = L"";
	for (int k = 0; k < 10; ++k)
	{
		swprintf_s(temp, _T("%d"), k);
		if (name == *temp)
		{
			return k;
		}
	}
	return -1; 
}

int Game::Check_NumSize(int i)
{
	if (i >= 10000)
		return 5;
	else if (i >= 1000)
		return 4;
	else if (i >= 100)
		return 3;
	else if (i >= 10)
		return 2;
	return 1;
}
