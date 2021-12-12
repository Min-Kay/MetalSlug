#include "framework.h"
#include "Game.h"
#include "Manager.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "Enemys.h"

Game::Game()
{
}

Game::~Game()
{
	Release();
}

void Game::Initialize()
{
	totalX = 0;
	totalY = 0; 

	isClear = false; 
	isFail = false;
	spawnMidBoss = false;

	checkPoint = false;

	Init_Information();

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/background.bmp", L"background");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-0.bmp",L"Map1-0");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-1.bmp", L"Map1-1");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-2.bmp", L"Map1-2");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map1-3.bmp", L"Map1-3");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MidBoss.bmp", L"MidBoss");

	ObjPoolMgr::Get_Instance()->Initialize();
	DataMgr::Get_Instance()->Initialize(); 
	CLineMgr::Get_Instance()->Load(STAGE1_SAVE);
	ObjPoolMgr::Get_Instance()->Spawn_Player(PLAYER_X,100);

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

void Game::Render(HDC _hdc)
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
		GdiTransparentBlt(_hdc, scrollX + 4650, scrollY + 250, 1026, 324, drawingDC, 0, 3394, 380, 120, THREEHEAD_COLOR);
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

void Game::Release()
{
	BlockMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->Release();
	ObjPoolMgr::Get_Instance()->DisableObj();
	CScrollMgr::Get_Instance()->Init_Scroll();
}

void Game::KeyInput()
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

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
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::WEAPON, rand() % 200 + 100 - scrollX, 200, WEAPON::SHOTGUN);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('3'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::WEAPON, rand() % 200 + 100 - scrollX, 200, WEAPON::ROCKET);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('4'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::WEAPON, rand() % 200 + 100 - scrollX, 200, WEAPON::HEAVY);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('5'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::WEAPON, rand() % 200 + 100 - scrollX, 200, WEAPON::IRONLIZARD);
		//ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::AMMO, rand() % 200 + 100 - scrollX, 200);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('6'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Item(ITEM::WEAPON, rand() % 200 + 100 - scrollX, 200, WEAPON::LASER);
	}
	if (CKeyMgr::Get_Instance()->Key_Down('7'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::DICOKKA, 500 - scrollX, 300, DIR::LEFT);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('8'))
	{
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::BAZUKA, 500 - scrollX, 200, DIR::LEFT);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('0'))
	{
		checkPoint = true;
	}
}

bool Game::Check_GameState()
{
	if (isClear)
	{
		Release();
		SceneMgr::Get_Instance()->Change_Scene(SCENE::GAME1);
		return true;
	}
	else if (isFail)
	{
		Release();
		SceneMgr::Get_Instance()->Change_Scene(SCENE::MENU);
		return true;
	}
	
	return false;
}

void Game::Init_Information()
{
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Life_Bar.bmp",L"BAR");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Life_Bullet.bmp", L"UI");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/number.bmp", L"NUMBER");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Infinite.bmp", L"Infinite");
}

void Game::Render_Information(HDC _hdc)
{
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"UI");
	GdiTransparentBlt(_hdc, 10, 10, 300, 64, drawingDC, 0, 0, 300, 64, RGB(0,255,0));

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

void Game::Check_Scrolling()
{
	if (checkPoint)
	{
		Set_CheckPoint_Objects();

		formalX = CScrollMgr::Get_Instance()->Get_ScrollX();
		formalY = CScrollMgr::Get_Instance()->Get_ScrollY();
		currPlayerPos = ObjPoolMgr::Get_Instance()->Get_Player_Info().x;
		if(scrollLock.size() > 1)
			scrollLock.pop_front();

		CScrollMgr::Get_Instance()->Set_ScrollLockX((float)scrollLock.front().x);
		scrollUpdating = true;
		checkPoint = false;
	}
	else if (scrollUpdating)
	{
		int scrollPosX = (int)abs((currPlayerPos + formalX - PLAYER_X));
		int scrollPosY = (int)abs(scrollLock.front().y - formalY);

		if (totalX < scrollPosX || totalY < scrollPosY)
		{
			if (totalX < scrollPosX)
			{
				if ((currPlayerPos + formalX - PLAYER_X) < 0)
					CScrollMgr::Get_Instance()->Set_ScrollX(10.f);
				else if ((currPlayerPos + formalX - PLAYER_X) > 0)
					CScrollMgr::Get_Instance()->Set_ScrollX(-10.f);
				totalX += 10;
			}

			if (totalY < scrollPosY)
			{
				if (scrollLock.front().y > formalY)
					CScrollMgr::Get_Instance()->Set_ScrollY(10.f);
				else if (scrollLock.front().y < formalY)
					CScrollMgr::Get_Instance()->Set_ScrollY(-10.f);
				totalY += 10;
			}
		}
		else
		{
			totalX = 0;
			totalY = 0;
			scrollUpdating = false;
		}
	}
}

void Game::Set_CheckPoint_Objects()
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
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::ARABIAN, 1200, 300, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::ARABIAN, 1400, 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::ARABIAN, 1500, 300, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::ARABIAN, 1700, 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::ARABIAN, 1800, 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Npc(NPC::SLAVE, 1500, 300);
		break;
	case 1:
		ObjPoolMgr::Get_Instance()->Spawn_Npc(NPC::SLAVE, 2600, 300);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::ARABIAN, 2500 , 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::ARABIAN, 2700, 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::ARABIAN, 2800, 300, DIR::RIGHT);
		break;
	case 2:
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::ARABIAN, 3200, 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::ARABIAN, 3400, 300, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::ARABIAN, 3600, 300, DIR::LEFT);
		break;
	case 3:
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::THREEHEAD, 4650, 300);
		break;
	case 4:
		spawnMidBoss = true;
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 5900, 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 5950, 300, DIR::LEFT);
		ObjPoolMgr::Get_Instance()->Spawn_Npc(NPC::SLAVE, 6250, 300);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 6200, 300, DIR::RIGHT);
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 6300, 300, DIR::LEFT);
		break;
	case 5:
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::MASKNELL, 8700, 200, DIR::LEFT);
		break;
	case 6:
		ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDAE, 9100, -100, DIR::LEFT);
		break;
	}
	if(maxCheckPoint > currCheckPoint)
		++currCheckPoint;
}

void Game::Check_Checkpoint_Clear()
{
	if (ObjPoolMgr::Get_Instance()->Get_OnScreen_Count() <= 0)
		checkPoint = true;
}



