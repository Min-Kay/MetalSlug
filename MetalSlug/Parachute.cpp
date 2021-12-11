#include "Parachute.h"
#include "Manager.h"
void Parachute::Initialize()
{
	id = OBJ::ENEMY; 
	hp = 1;
	info.cx = 54.f;
	info.cy = 50.f;
	animIndex = 0;
	animTimer = GetTickCount();
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Parachute_Effect.bmp",L"Parachute");
	
}

int Parachute::Update()
{
	if (isDead)
		return OBJ_DEAD;

	Set_Pos(parent->Get_Info().x, parent->Get_Info().y - info.cy);
	Update_Rect();
	return OBJ_DEFAULT;
}

void Parachute::Late_Update()
{
	if (hp <= 0)
	{
		isDead = true; 
	}
}

void Parachute::Render(HDC _hdc)
{
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Parachute");
	GdiTransparentBlt(_hdc, rect.left + CScrollMgr::Get_Instance()->Get_ScrollX() , rect.top + CScrollMgr::Get_Instance()->Get_ScrollY(), info.cx, info.cy, drawingDC, 2 + animIndex * 54, 98, 54, 50, RGB(0, 248, 0));
}

void Parachute::Release()
{
}
