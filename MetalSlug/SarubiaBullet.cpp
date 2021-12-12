#include "SarubiaBullet.h"
#include "Manager.h"

void SarubiaBullet::Initialize()
{
	info.cx = 40.f; 
	info.cy = 40.f;
	id = OBJ::BULLET;
	parentID = OBJ::ENEMY;
	damage = 10;
	speed = 2.f;
	animIndex = 0;
	isDead = false;
	isDying = false; 
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Sarubia_Bullet.bmp",L"Sarubia_Bullet");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp",L"Stretch_Item");
}

int SarubiaBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (dir == DIR::LEFT)
	{
		info.x -= speed;
	}
	else
	{
		info.x += speed;
	}

	Gravity();
	Update_Rect();
	return OBJ_DEFAULT;
}

void SarubiaBullet::Late_Update()
{
	Check_Screen_Out();
}

void SarubiaBullet::Render(HDC _hdc)
{
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Sarubia_Bullet");
	Anim_Counter(20,30.f);
	if (dir == DIR::LEFT)
	{
		GdiTransparentBlt(_hdc, rect.left + CScrollMgr::Get_Instance()->Get_ScrollX(), rect.top + CScrollMgr::Get_Instance()->Get_ScrollY(), info.cx, info.cy, drawingDC, 17 * animIndex + 114, 127, 17, 15, RGB(0, 248, 0));
	}
	else
	{
		stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
		StretchBlt(stretchDC, 0,0,17,15, drawingDC, 17 * animIndex + 114 + 17, 127, -17,15, SRCCOPY);
		GdiTransparentBlt(_hdc, rect.left + CScrollMgr::Get_Instance()->Get_ScrollX(), rect.top + CScrollMgr::Get_Instance()->Get_ScrollY(), info.cx, info.cy, stretchDC, 0, 0, 17, 15, RGB(0, 248, 0));
	}
	
}

void SarubiaBullet::Release()
{
}
