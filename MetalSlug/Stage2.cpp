#include "framework.h"
#include "Stage2.h"
#include "Manager.h"
#include "AbstractFactory.h"
#include "Player.h"

void Stage2::Initialize()
{
	isClear = false;
	isFail = false;
	Init_Information();
	BmpMgr::Get_Instance()->Insert_Bmp(MISSION2_BMP, MISSION2_KEY);
	BmpMgr::Get_Instance()->Insert_Bmp(STRETCH_BMP, STRETCH_KEY);
	ObjPoolMgr::Get_Instance()->Initialize();
	CScrollMgr::Get_Instance()->Set_ScrollLockX(12879);
	CScrollMgr::Get_Instance()->Set_ScrollLockY(600);
	CLineMgr::Get_Instance()->Load(STAGE1_SAVE);
	ObjPoolMgr::Get_Instance()->Spawn_Player(100, 100);
	ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 300, 300, DIR::LEFT, SOLDIER::PRIVATE);
	ObjPoolMgr::Get_Instance()->Spawn_Enemy(ENEMY::SOLDIER, 300, 300, DIR::LEFT, SOLDIER::SERGENT);

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
			ObjPoolMgr::Get_Instance()->Spawn_Player(100, 100);
			ObjPoolMgr::Get_Instance()->Set_Player_Dead(false);
		}
		else
			isFail = true;

	}

	Check_GameState();

	ObjPoolMgr::Get_Instance()->Late_Update();
}

void Stage2::Render(HDC _hdc)
{
	drawingDC = BmpMgr::Get_Instance()->Find_Image(MISSION2_KEY);
	StretchBlt(_hdc, CScrollMgr::Get_Instance()->Get_ScrollX(), CScrollMgr::Get_Instance()->Get_ScrollY(), 12879, 654, drawingDC, 0, 0, 4293, 218, SRCCOPY);

	CLineMgr::Get_Instance()->Render(_hdc);
	ObjPoolMgr::Get_Instance()->Render(_hdc);
	Render_Information(_hdc);

}

void Stage2::Release()
{
	ObjPoolMgr::Get_Instance()->DisableObj();
}
