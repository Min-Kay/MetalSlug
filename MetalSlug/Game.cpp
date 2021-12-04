#include "Game.h"
#include "Manager.h"
#include "AbstractFactory.h"
#include "Player.h"

void Game::Initialize()
{
	isClear = false; 
	isFail = false;

	BmpMgr::Get_Instance()->Insert_Bmp(MISSION2_BMP,MISSION2_KEY);
	BmpMgr::Get_Instance()->Insert_Bmp(STRETCH_BMP, STRETCH_KEY);
	CScrollMgr::Get_Instance()->Set_ScrollLockX(12879);
	CScrollMgr::Get_Instance()->Set_ScrollLockY(600);
	CLineMgr::Get_Instance()->Load(STAGE1_SAVE);
	ObjPoolMgr::Get_Instance()->Add_Object(OBJ::PLAYER,CAbstractFactory<Player>::Create(100,100));
}

void Game::Update()
{
	KeyInput();
	ObjPoolMgr::Get_Instance()->Update();
}

void Game::Late_Update()
{
	Check_GameState();
	ObjPoolMgr::Get_Instance()->Late_Update();

}

void Game::Render(HDC _hdc)
{
	drawingDC = BmpMgr::Get_Instance()->Find_Image(MISSION2_KEY);
	StretchBlt(_hdc, CScrollMgr::Get_Instance()->Get_ScrollX(), CScrollMgr::Get_Instance()->Get_ScrollY(), 12879, 654, drawingDC, 0, 0, 4293, 218, SRCCOPY);

	CLineMgr::Get_Instance()->Render(_hdc);
	ObjPoolMgr::Get_Instance()->Render(_hdc);

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

	if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
	{
		CScrollMgr::Get_Instance()->Set_ScrollX(50.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
	{
		CScrollMgr::Get_Instance()->Set_ScrollX(-50.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
	{
		CScrollMgr::Get_Instance()->Set_ScrollY(50.f);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
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
