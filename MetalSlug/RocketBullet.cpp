#include "RocketBullet.h"
#include "Manager.h"

void RocketBullet::Initialize()
{
	damage = 20.f;
	parentID = OBJ::PLAYER;
	animIndex = 0;
	animTimer = GetTickCount();

	info.cx = 50.f;
	info.cy = 50.f;

	init_CX = 100.f;
	init_CY = 100.f;

	dir = DIR::RIGHT;
	id = OBJ::BULLET;
	render = RENDER::OBJECT;
	isDead = false;
}

int RocketBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;

	Update_Rect();
	return OBJ_DEFAULT;
}

void RocketBullet::Late_Update()
{
}

void RocketBullet::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	

	switch (dir)
	{
	case DIR::UP:
		Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);
		break;
	case DIR::DOWN:
		Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);
		break;
	case DIR::LEFT:
		Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);
		break;
	case DIR::RIGHT:
		Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);
		break;
	}
}

void RocketBullet::Release()
{
}

void RocketBullet::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
}
