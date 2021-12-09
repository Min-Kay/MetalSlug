#include "SolDaeLaser.h"
#include "Player.h"
#include "Manager.h"

void SolDaeLaser::Initialize()
{
	damage = 10000;
	speed = 0.f; 
	parentID = OBJ::ENEMY; 

	info.cx = 100.f; 
	info.cy = 500.f;

	animIndex = 0;
	animTimer = GetTickCount(); 
	id = OBJ::BULLET; 
	isDead = false;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SolDae_Laser.bmp",L"SolDae_Laser");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SolDae_Laser_Bottom.bmp", L"SolDae_Laser_Bottom");

}

int SolDaeLaser::Update()
{
	if (isDead)
		return OBJ_DEAD; 

	Set_Position();
	Update_Rect();
	return OBJ_DEFAULT;
}

void SolDaeLaser::Late_Update()
{
}

void SolDaeLaser::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	//Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);
	//Rectangle(_hdc, rect.left - 20 + scrollX, rect.top + 400 + scrollY, rect.right + 20 + scrollX, rect.bottom + scrollY);
	Anim_Counter(18,30.f);
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Laser");
	if (animIndex < 7)
	{
		GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 100, 600, drawingDC, animIndex * 110, 0, 110, 170, RGB(248, 0, 248));
	}
	else if (animIndex < 14)
	{
		GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 100, 600, drawingDC, (animIndex % 7) * 110, 170, 110, 170, RGB(248, 0, 248));
	}
	else
	{
		GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 100, 600, drawingDC, (animIndex % 7), 340, 110, 170, RGB(248, 0, 248));
	}
	
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Laser_Bottom");
	GdiTransparentBlt(_hdc, rect.left - 20 + scrollX, rect.top + 300 + scrollY, 140, 200, drawingDC, (animIndex % 7) * 95, 0, 95, 70, RGB(248, 0, 248));

	
}

void SolDaeLaser::Release()
{
}

void SolDaeLaser::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (_id == OBJ::PLAYER )
	{
		if(!static_cast<Player*>(_opponent)->Get_Dying())
			static_cast<Player*>(_opponent)->Set_Dying();
	}
}

void SolDaeLaser::Set_Position()
{
	info.x = parent->Get_Info().x;
	info.y = parent->Get_Info().y + parent->Get_Info().cy;
}
