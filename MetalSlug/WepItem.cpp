#include "WepItem.h"
#include "Weapons.h"
#include "Manager.h"

void WepItem::Initialize()
{
	id = OBJ::PROP;
	item_id = ITEM::WEAPON;
	wep_id = WEAPON::HEAVY;
	info.cx = 50.f;
	info.cy = 50.f;
	isDead = false; 
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item.bmp",L"Item");
}

int WepItem::Update()
{
	if (isDead)
		return OBJ_DEAD; 
	if (wep_id)
	{
	}
	Gravity(); 
	Update_Rect(); 
	return OBJ_DEFAULT;
}

void WepItem::Late_Update()
{
}

void WepItem::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Item");
	switch (wep_id)
	{
	case WEAPON::HEAVY:
		Anim_Counter(3, 100.f, true, 0);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndex * 25 + 2, 22, 25, 25, ITEM_COLOR);
		break;
	case WEAPON::ROCKET:
		Anim_Counter( 3, 100.f, true, 4);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndex * 25 + 2, 22, 25, 25, ITEM_COLOR);
		break;
	default:
		Rectangle(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), int(rect.right + scrollX), int(rect.bottom + scrollY));
		break;
	}

	if (wep_id) {}
}

void WepItem::Release()
{
}

void WepItem::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (_id == OBJ::PLAYER)
	{
		switch (wep_id)
		{
		case WEAPON::HEAVY:
			if (DataMgr::Get_Instance()->Get_Weapon()->Get_WepID() != wep_id)
				ObjPoolMgr::Get_Instance()->Set_Player_Wep(new HeavyMachine);
			else
				DataMgr::Get_Instance()->Add_Ammo(200);
			break;
		case WEAPON::ROCKET:
			if (DataMgr::Get_Instance()->Get_Weapon()->Get_WepID() != wep_id)
				ObjPoolMgr::Get_Instance()->Set_Player_Wep(new HeavyMachine);
			else
				DataMgr::Get_Instance()->Add_Ammo(200);
			break;
		}
		isDead = true; 
	}
}
