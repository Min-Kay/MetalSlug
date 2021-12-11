#include "Car.h"
#include "Manager.h"

void Car::Initialize()
{
	id = OBJ::BLOCK;

	info.cx = 160.f; 
	info.cy = 100.f;

	dir == DIR::LEFT;

	isDead = false;
	isDying = false;
	hp = 100;
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Taxi.bmp", L"Taxi");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_White.bmp",L"Stretch_White");
}

int Car::Update()
{
	if (isDead)
		return OBJ_DEAD; 
	
	if (haveGravity)
		Gravity(); 
	Update_Rect(); 

	return OBJ_DEFAULT;
}

void Car::Late_Update()
{
	if (isDying)
		return;

	if (hp <= 0)
	{
		animIndex = 0; 
		isDying = true;
	}
}

void Car::Render(HDC _hdc)
{
	if (isDead)
		return;

	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Taxi");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White");
	if (isDying)
	{
		Anim_Counter(4,100.f,false);
		if (dir == DIR::LEFT)
		{
			GdiTransparentBlt(_hdc, x + rect.left, y + rect.top, info.cx, info.cy, drawingDC, animIndex * 90 + 5,170, 90, 60, RGB(255, 255, 255));
		}
		else
		{
			StretchBlt(stretchDC, 0, 0, 90, 60, drawingDC, animIndex * 90 + 5 , 170, 90, 60, SRCCOPY);
			GdiTransparentBlt(_hdc, x + rect.left, y + rect.top, info.cx, info.cy, stretchDC, 0, 0, 90, 60, RGB(255, 255, 255));
		}

		if (animIndex == 4)
			isDead = true; 
	}
	else if (!onHit)
	{
		if(dir == DIR::LEFT)
		{
			GdiTransparentBlt(_hdc,x + rect.left , y + rect.top, info.cx , info.cy , drawingDC,  90, 0, 90,60 ,RGB(255,255,255));
		}
		else
		{
			StretchBlt(stretchDC, 0, 0, 90, 60, drawingDC,  90, 0, 90, 60, SRCCOPY);
			GdiTransparentBlt(_hdc, x + rect.left, y + rect.top, info.cx, info.cy, stretchDC, 0, 0, 90, 60,RGB(255, 255, 255));
		}
		animIndex = 0; 
	}
	else
	{
		Anim_Counter(9,100.f,false,1);
		if (dir == DIR::LEFT)
		{
			GdiTransparentBlt(_hdc, x + rect.left, y + rect.top, info.cx, info.cy, drawingDC, animIndex * 90, 0, 90, 60, RGB(255, 255, 255));
		}
		else
		{
			StretchBlt(stretchDC, 0, 0, 90, 60, drawingDC, animIndex * 90, 0, 90, 60, SRCCOPY);
			GdiTransparentBlt(_hdc, x + rect.left, y + rect.top, info.cx, info.cy, stretchDC, 0, 0, 90, 60, RGB(255, 255, 255));
		}
		if (animIndex == 9)
		{
			animIndex = 0;
			onHit = false;
		}
	}
}

void Car::Release()
{
}