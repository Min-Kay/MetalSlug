#include "HeavyBullet.h"
#include "Items.h"
#include "Manager.h"

void HeavyBullet::Initialize()
{
	id = OBJ::BULLET;
	render = RENDER::OBJECT;
	parentID = OBJ::PLAYER;
	speed = 10.f;
	info.cx = 80.f;
	info.cy = 80.f;

	init_CX = 100.f; 
	init_CY = 100.f;

	animTimer = GetTickCount(); 

	spread = (rand() % 100) * 0.01f;
	minus = rand() % 2;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/HeavyBullet_Right.bmp",L"HeavyRight");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/HeavyBullet_Up.bmp", L"HeavyUp");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/HeavyBullet_Down.bmp", L"HeavyDown");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp", L"Stretch_Item");
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
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (0 - scrollX > info.x ||
		0 - scrollY > info.y ||
		WINCX - scrollX < info.x ||
		WINCY - scrollY < info.y)
		isDead = true;
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

	//Ellipse(_hdc, rect.left, rect.top, rect.right, rect.bottom);
}

void HeavyBullet::Release()
{
}

void HeavyBullet::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	switch (_id)
	{
	case OBJ::PLAYER:
		if (_opponent->Get_ID() != parentID)
			isDead = true;
		break;
	case OBJ::ENEMY:
		if (_opponent->Get_ID() != parentID)
			isDead = true;
		break;
	case OBJ::PROP:
		if (static_cast<Item*>(_opponent)->Get_ItemID() == ITEM::ITEMBOX)
			isDead = true; 
		break;
	case OBJ::BLOCK:
		isDead = true;
		break;
	}
}
