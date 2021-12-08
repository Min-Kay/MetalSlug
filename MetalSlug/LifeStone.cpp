#include "LifeStone.h"
#include "Manager.h"

void LifeStone::Initialize()
{
	id = OBJ::PROP;
	item_id = ITEM::LIFE;
	render = RENDER::OBJECT;
	info.cx = 50.f;
	info.cy = 50.f;
	isDead = false;

	isValid = true;
	validTime = GetTickCount();

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item.bmp", L"Item");
}

int LifeStone::Update()
{
	if (isDead)
		return OBJ_DEAD;

	Gravity();
	Update_Rect();
	return OBJ_DEFAULT;
}

void LifeStone::Late_Update()
{
	Valid();
}

void LifeStone::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, 330, 120, 30, 30, ITEM_COLOR);

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Item");
}

void LifeStone::Release()
{
}

void LifeStone::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (isDead || isValid)
		return;

	if (_id == OBJ::PLAYER)
	{
		DataMgr::Get_Instance()->Add_Life(1);
		isDead = true;
	}
}
