#include "ShotgunBullet.h"
#include "Manager.h"
void ShotgunBullet::Initialize()
{
	damage = 40.f;
	parentID = OBJ::PLAYER;
	animIndex = 0;
	animTimer = GetTickCount();

	info.cx = 150.f;
	info.cy = 150.f;

	init_CX = 200.f;
	init_CY = 200.f;

	dir = DIR::RIGHT;
	id = OBJ::BULLET;
	render = RENDER::OBJECT;
	isDead = false;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shotgun.bmp",L"Shotgun");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shotgun_Up.bmp", L"Shotgun_Up");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shotgun_Down.bmp", L"Shotgun_Down");


	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_White5.bmp", L"Stretch_White5");

}

int ShotgunBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_DEFAULT;
}

void ShotgunBullet::Late_Update()
{
}

void ShotgunBullet::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White5");
	switch (dir)
	{
	case DIR::UP:
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Shotgun_Up");
		//Rectangle(_hdc,info.x - info.cx * 0.5f + scrollX, info.y - init_CY * 0.5f + scrollY,info.x + info.cx * 0.5f + scrollX, info.y + init_CY * 0.5f + scrollY);
		
		Anim_Counter(10,7.f, false);
		if (animIndex < 3)
		{
			GdiTransparentBlt(_hdc, info.x - info.cx * 0.5f + scrollX, info.y - init_CY * 0.5f + scrollY, info.cx, init_CY, drawingDC, 0, 270 - animIndex * 90, 70, 90, RGB(255, 255, 255));
		}
		else if (animIndex < 6)
		{
			GdiTransparentBlt(_hdc, info.x - info.cx * 0.5f + scrollX, info.y - init_CY * 0.5f + scrollY, info.cx, init_CY, drawingDC, 70, 250 - ( animIndex % 3) * 100, 70, 100, RGB(255, 255, 255));

		}
		else if (animIndex < 10)
		{
			GdiTransparentBlt(_hdc, info.x - info.cx * 0.5f + scrollX, info.y - init_CY * 0.5f + scrollY, info.cx, init_CY, drawingDC, 150, 250 - ( animIndex % 4) * 90, 70, 90, RGB(255, 255, 255));
		}
		else
		{
			GdiTransparentBlt(_hdc, info.x - info.cx * 0.5f + scrollX, info.y - init_CY * 0.5f + scrollY, info.cx, init_CY, drawingDC, 220, 270 , 70, 90, RGB(255, 255, 255));
		}

		break;
	case DIR::DOWN:
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Shotgun_Down");
		//Rectangle(_hdc, info.x - info.cx * 0.5f + scrollX, info.y - init_CY * 0.5f + scrollY, info.x + info.cx * 0.5f + scrollX, info.y + init_CY * 0.5f + scrollY);

		Anim_Counter(10, 7.f, false);
		if (animIndex < 3)
		{
			GdiTransparentBlt(_hdc, info.x - info.cx * 0.5f + scrollX, info.y - init_CY * 0.5f + scrollY, info.cx,init_CY, drawingDC, 280, animIndex * 90, 70, 90, RGB(255, 255, 255));
		}
		else if (animIndex < 6)
		{
			GdiTransparentBlt(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, info.cx, init_CY, drawingDC, 200, animIndex % 3 * 100,70, 100, RGB(255, 255, 255));

		}
		else if (animIndex < 10)
		{
			GdiTransparentBlt(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, info.cx, init_CY, drawingDC, 120, animIndex % 4 * 90, 70, 90, RGB(255, 255, 255));
		}
		else
		{
			GdiTransparentBlt(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, info.cx, init_CY, drawingDC, 80, 90, 70, 90, RGB(255, 255, 255));
		}

		break;
	case DIR::LEFT:
		//Rectangle(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, info.x + init_CX * 0.5f + scrollX, info.y + info.cy * 0.5f + scrollY);
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Shotgun");
		Anim_Counter(10, 7.f, false);
		if (animIndex < 3)
		{
			StretchBlt(stretchDC, 0, 0, 90, 70, drawingDC, 90 * animIndex + 90, 0, -90, 70, SRCCOPY);
			GdiTransparentBlt(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, init_CX, info.cy, stretchDC, 0, 0, 90, 70, RGB(255, 255, 255));
		}
		else if (animIndex < 6)
		{
			StretchBlt(stretchDC, 0, 0, 100, 70, drawingDC, 100 * (animIndex % 3) + 100, 70, -100, 70, SRCCOPY);
			GdiTransparentBlt(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, init_CX, info.cy, stretchDC, 0, 0, 100, 70, RGB(255, 255, 255));

		}
		else if (animIndex < 10)
		{
			StretchBlt(stretchDC, 0, 0, 90, 70, drawingDC, 90 * (animIndex % 4) + 90, 150, -90, 70, SRCCOPY);
			GdiTransparentBlt(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, init_CX, info.cy, stretchDC, 0, 0, 90, 70, RGB(255, 255, 255));
		}
		else
		{
			StretchBlt(stretchDC, 0, 0, 90, 70, drawingDC, 90, 220, -90, 70, SRCCOPY);

			GdiTransparentBlt(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, init_CX, info.cy, stretchDC, 0, 0, 90, 70, RGB(255, 255, 255));
		}

		
		break;
	case DIR::RIGHT:
		//Rectangle(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, info.x + init_CX * 0.5f + scrollX, info.y + info.cy * 0.5f + scrollY);
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Shotgun");
		Anim_Counter(10, 7.f, false);
		if (animIndex < 3)
		{
			GdiTransparentBlt(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, init_CX, info.cy, drawingDC, 90 * animIndex, 0, 90, 70, RGB(255, 255, 255));
		}
		else if (animIndex < 6)
		{
			GdiTransparentBlt(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, init_CX, info.cy, drawingDC, 100 * (animIndex % 3), 70,100, 70, RGB(255, 255, 255));

		}
		else if (animIndex < 10)
		{
			GdiTransparentBlt(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, init_CX, info.cy, drawingDC, 90 *( animIndex % 4), 150, 90, 70, RGB(255, 255, 255));

		}
		else 
			GdiTransparentBlt(_hdc, info.x - init_CX * 0.5f + scrollX, info.y - info.cy * 0.5f + scrollY, init_CX, info.cy, drawingDC, 90, 220, 90, 70, RGB(255, 255, 255));
		break;
	}

	if (animIndex == 10)
		isDead = true;
}

void ShotgunBullet::Release()
{
}