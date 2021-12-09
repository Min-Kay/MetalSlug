#include "GrenadeBox.h"
#include "Manager.h"

void GrenadeBox::Initialize()
{
	id = OBJ::PROP;
	item_id = ITEM::GRENADE;
	info.cx = 50.f;
	info.cy = 50.f;
	isDead = false;

	isValid = true;
	validTime = GetTickCount();

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item.bmp", L"Item");
}

int GrenadeBox::Update()
{
	if (isDead)
		return OBJ_DEAD;

	Gravity();
	Update_Rect();
	return OBJ_DEFAULT;
}

void GrenadeBox::Late_Update()
{
	Valid(); 
}

void GrenadeBox::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	Anim_Counter(5, 100.f);
	GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndex * 35 + 3, 50, 33, 33, ITEM_COLOR);
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Item");
}

void GrenadeBox::Release()
{
}

void GrenadeBox::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (isDead || isValid)
		return;

	if (_id == OBJ::PLAYER)
	{
		DataMgr::Get_Instance()->Add_Grenade(5);
		isDead = true;
	}
}
