#include "DiCokkaBullet.h"
#include "Manager.h"
void DiCokkaBullet::Initialize()
{
	id = OBJ::BULLET;
	parentID = OBJ::ENEMY;
	speed = 5.f;
	damage = 20.f;

	info.cx = 100.f;
	info.cy = 50.f;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/DiCokka.bmp",L"DiCokka");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp", L"Stretch_Item");
}

int DiCokkaBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;

	switch (dir)
	{
	case DIR::LEFT:
		info.x -= speed;
		break;
	case DIR::RIGHT:
		info.x += speed;
		break;
	}
	Update_Rect();
	return OBJ_DEFAULT;
}

void DiCokkaBullet::Late_Update()
{
	Check_Screen_Out();
}

void DiCokkaBullet::Render(HDC _hdc)
{
	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();
	
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"DiCokka");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");

	Anim_Counter(3,100.f);
	switch (dir)
	{
	case DIR::LEFT:
		GdiTransparentBlt(_hdc,rect.left + x, rect.top + y, info.cx ,info.cy, drawingDC, 249 + animIndex * 50, 900, 50,25, RGB(0,248,0));
		break;
	case DIR::RIGHT:
		StretchBlt(stretchDC,0,0,50,25, drawingDC, 249 + animIndex * 50 + 50, 900, -50, 25,SRCCOPY);
		GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 50, 25, RGB(0, 248, 0));
		break;
	}
}

void DiCokkaBullet::Release()
{
}
