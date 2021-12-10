#include "HeavyBullet.h"
#include "Items.h"
#include "Manager.h"

void HeavyBullet::Initialize()
{
	id = OBJ::BULLET;
	parentID = OBJ::PLAYER;
	speed = 20.f;
	info.cx = 80.f;
	info.cy = 80.f;

	init_CX = 100.f; 
	init_CY = 100.f;

	animTimer = GetTickCount(); 

	spread = (rand() % 150) * 0.01f;
	minus = rand() % 2;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/HeavyBullet_Right.bmp",L"HeavyRight");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/HeavyBullet_Up.bmp", L"HeavyUp");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/HeavyBullet_Down.bmp", L"HeavyDown");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp", L"Stretch_Item");

	damage = 1000;
}

int HeavyBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;

	switch (dir)
	{
	case DIR::UP:
		if (minus)
			info.x -= spread;
		else
			info.x += spread;
		info.y -= speed;
		break;
	case DIR::DOWN:
		if (minus)
			info.x -= spread;
		else
			info.x += spread;
		info.y += speed;
		break;
	case DIR::LEFT:
		if (minus)
			info.y -= spread;
		else
			info.y += spread;
		info.x -= speed;
		break;
	case DIR::RIGHT:
		if (minus)
			info.y -= spread;
		else
			info.y += spread;
		info.x += speed;
		break;
	}

	Update_Rect();

	return OBJ_DEFAULT;
}

void HeavyBullet::Late_Update()
{
	Check_Screen_Out();
}

void HeavyBullet::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	switch (dir)
	{
	case DIR::UP:
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"HeavyUp");
		Anim_Counter(1, 40.f, true);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, init_CY, drawingDC, 0, 35 * animIndex, 30, 35, ITEM_COLOR);
		break;
	case DIR::DOWN:
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"HeavyDown");
		Anim_Counter(1, 40.f, true);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, init_CY, drawingDC, 0, 35 * animIndex, 30, 35, ITEM_COLOR);
		break;
	case DIR::RIGHT:
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"HeavyRight");
		Anim_Counter(1, 40.f, true);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), init_CX, info.cy, drawingDC, 35 * animIndex, 0, 35, 30, ITEM_COLOR);
		break;
	case DIR::LEFT:
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"HeavyRight");
		stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
		Anim_Counter(0, 40.f, false);
		StretchBlt(stretchDC, 0, 0, init_CX, info.cy, drawingDC, 35 * animIndex + 35, 0, -35, 30, SRCCOPY);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), init_CX, info.cy, stretchDC, 0, 0, init_CX, info.cy, ITEM_COLOR);
		break;
	}
}

void HeavyBullet::Release()
{
}

void HeavyBullet::Update_Rect()
{
	switch (dir)
	{
	case DIR::UP:
		rect.left =  info.x - info.cx * 0.5f; 
		rect.right = info.x + info.cx * 0.5f;
		rect.top = info.y - init_CY * 0.5f;
		rect.bottom = info.y + init_CY * 0.5f;

		break;
	case DIR::DOWN:
		rect.left = info.x - info.cx * 0.5f;
		rect.right = info.x + info.cx * 0.5f;
		rect.top = info.y - init_CY * 0.5f;
		rect.bottom = info.y + init_CY * 0.5f;
		break;
	case DIR::LEFT:
		rect.left = info.x - init_CX * 0.5f;
		rect.right = info.x + init_CX * 0.5f;
		rect.top = info.y - info.cy * 0.5f;
		rect.bottom = info.y + info.cy * 0.5f;
		break;
	case DIR::RIGHT:
		rect.left = info.x - init_CX * 0.5f;
		rect.right = info.x + init_CX * 0.5f;
		rect.top = info.y - info.cy * 0.5f;
		rect.bottom = info.y + info.cy * 0.5f;
		break;
	}
}

