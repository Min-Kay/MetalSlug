#include "Parachute.h"
#include "Manager.h"
#include "Bullet.h"

void Parachute::Initialize()
{
	id = OBJ::ENEMY; 
	hp = 1;
	info.cx = 120.f;
	info.cy = 120.f;
	animIndex = 0;
	animTimer = GetTickCount();

	canCollision = false;
	coll_Attack = false; 

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Parachute_Effect.bmp",L"Parachute");
	
}

int Parachute::Update()
{
	if (isDead)
		return OBJ_DEAD;

	Set_Pos(parent->Get_Info().x , parent->Get_Info().y - info.cy * 0.5f );
	Update_Rect();
	return OBJ_DEFAULT;
}

void Parachute::Late_Update()
{
	if (isDead)
		return;

	if (hp <= 0)
	{
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY,this);
		isDead = true; 
	}
}

void Parachute::Render(HDC _hdc)
{
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Parachute");
	GdiTransparentBlt(_hdc, rect.left + CScrollMgr::Get_Instance()->Get_ScrollX() , rect.top + CScrollMgr::Get_Instance()->Get_ScrollY(), info.cx, info.cy, drawingDC, 2 + animIndex * 54, 90, 54, 50, RGB(0, 255, 0));
}

void Parachute::Release()
{
	parent = nullptr;
}

void Parachute::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (_id == OBJ::BULLET)
	{
		if (static_cast<Bullet*>(_opponent)->Get_ParentID() == OBJ::PLAYER)
			isDead = true;
	}
}
