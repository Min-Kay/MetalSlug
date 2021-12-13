#include "AmmoBox.h"
#include "Manager.h"

void AmmoBox::Initialize()
{
	id = OBJ::PROP;
	item_id = ITEM::AMMO;
	info.cx = 50.f;
	info.cy = 50.f;
	isDead = false; 
	isValid = true;
	validTime = GetTickCount();

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item.bmp",L"Item");
}

int AmmoBox::Update()
{
	if (isDead)
		return OBJ_DEAD;

	Gravity(); 
	Update_Rect();
	return OBJ_DEFAULT;
}

void AmmoBox::Late_Update()
{
	Valid();
}

void AmmoBox::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	Anim_Counter(5, 100.f, true, 6);
	GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), (int)info.cx, (int)info.cy, drawingDC, animIndex * 35 + 5, 50, 33, 33, ITEM_COLOR);
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Item");
	
}

void AmmoBox::Release()
{
}

void AmmoBox::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (isDead || isValid)
		return;

	if (_id == OBJ::PLAYER)
	{
		DataMgr::Get_Instance()->Add_Score(50);
		DataMgr::Get_Instance()->Add_Ammo(100);
		isDead = true;
	}
}
