#include "framework.h"
#include "Game.h"
#include "Manager.h"
#include "AbstractFactory.h"
#include "Player.h"

void Game::Initialize()
{
	isClear = false; 
	isFail = false;
	Init_Information();
	BmpMgr::Get_Instance()->Insert_Bmp(MISSION2_BMP,MISSION2_KEY);
	BmpMgr::Get_Instance()->Insert_Bmp(STRETCH_BMP, STRETCH_KEY);
	ObjPoolMgr::Get_Instance()->Initialize();
	DataMgr::Get_Instance()->Initialize(); 
	CScrollMgr::Get_Instance()->Set_ScrollLockX(12879);
	CScrollMgr::Get_Instance()->Set_ScrollLockY(600);
	CLineMgr::Get_Instance()->Load(STAGE1_SAVE);
	ObjPoolMgr::Get_Instance()->Spawn_Player(100,100);
	ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER,300,300,DIR::LEFT,SOLDIER::PRIVATE);
	ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 300, 300, DIR::LEFT, SOLDIER::SERGENT);

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
			ObjPoolMgr::Get_Instance()->Spawn_Player(100, 100);
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
	drawingDC = BmpMgr::Get_Instance()->Find_Image(MISSION2_KEY);
	StretchBlt(_hdc, CScrollMgr::Get_Instance()->Get_ScrollX(), CScrollMgr::Get_Instance()->Get_ScrollY(), 12879, 654, drawingDC, 0, 0, 4293, 218, SRCCOPY);

	CLineMgr::Get_Instance()->Render(_hdc);
	ObjPoolMgr::Get_Instance()->Render(_hdc);
	Render_Information(_hdc); 

}

void Game::Release()
{
	ObjPoolMgr::Get_Instance()->DisableObj();
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
}

void Game::Check_GameState()
{
	if (isClear)
		SceneMgr::Get_Instance()->Change_Scene(SCENE::MENU);
	else if(isFail)
		SceneMgr::Get_Instance()->Change_Scene(SCENE::MENU);
}

void Game::Init_Information()
{
	bulletRect = { 100, 50, 150, 100};
	lifeRect = { 230, 50, 280, 100 };
	scoreRect = { 310, 50, 360, 100 };
}

void Game::Render_Information(HDC _hdc)
{
	swprintf_s(scoreCount, _T("%d"), DataMgr::Get_Instance()->Get_Score());
	swprintf_s(bulletCount, _T("%d"), DataMgr::Get_Instance()->Get_Ammo());
	swprintf_s(lifeCount, _T("%d"), DataMgr::Get_Instance()->Get_Life());

	SetTextColor(_hdc, RGB(255, 255, 255));
	SetBkMode(_hdc, TRANSPARENT);

	TextOut(_hdc, scoreRect.left, scoreRect.top, scoreCount, lstrlen(scoreCount));
	TextOut(_hdc, bulletRect.left, bulletRect.top, bulletCount, lstrlen(bulletCount));
	TextOut(_hdc, lifeRect.left, lifeRect.top, lifeCount, lstrlen(lifeCount));
}
