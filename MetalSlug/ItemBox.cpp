#include "ItemBox.h"
#include "Manager.h"
#include "Bullet.h"

void ItemBox::Initialize()
{
	id = OBJ::PROP;
	item_id = ITEM::ITEMBOX;
	info.cx = 60.f;
	info.cy = 60.f;
	isBreaking = false;
	isDead = false;
	animIndex = 0; 
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item.bmp", L"Item");
}

int ItemBox::Update()
{
	if (isDead)
		return OBJ_DEAD;

	Gravity();
	Update_Rect();
	return OBJ_DEFAULT;
}

void ItemBox::Late_Update()
{
}

void ItemBox::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (!isBreaking)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Item");
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - 30, info.cx, info.cy + 30, drawingDC, 0, 145, 35, 55, ITEM_COLOR);
	}
	else
	{
		Anim_Breaking(_hdc);
	}
}

void ItemBox::Release()
{
}

void ItemBox::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (isBreaking || isDead)
		return;

	if (_id == OBJ::BULLET)
	{
		if (static_cast<Bullet*>(_opponent)->Get_ParentID() == OBJ::PLAYER)
		{
			isBreaking = true;
		}
	}
}

void ItemBox::Anim_Breaking(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Item");
	Anim_Counter(10, 40.f, false);
	GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - 30, info.cx + 10, info.cy + 30, drawingDC, animIndex * 35, 145, 35, 55, ITEM_COLOR);

	if (animIndex == 10)
	{
		ITEM::ID box[ITEM::END - 1] = { ITEM::AMMO,ITEM::GRENADE,ITEM::LIFE,ITEM::WEAPON };
		int pos = rand() % ITEM::END - 1;
		if (pos == -1)
			pos = 0;
		ITEM::ID item = box[pos];
		int wep = rand() % WEAPON::ROCKET + 1;
		ObjPoolMgr::Get_Instance()->Spawn_Item(item,info.x,info.y,(WEAPON::ID)wep);
		isDead = true;
	}
	
}
