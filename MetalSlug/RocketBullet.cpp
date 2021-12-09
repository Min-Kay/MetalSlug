#include "RocketBullet.h"
#include "Manager.h"

void RocketBullet::Initialize()
{
	damage = 0.f;
	parentID = OBJ::PLAYER;
	animIndex = 0;
	animTimer = GetTickCount();

	speed = 0.f; 

	info.cx = 50.f;
	info.cy = 50.f;

	init_CX = 100.f;
	init_CY = 100.f;

	dir = DIR::RIGHT;
	id = OBJ::BULLET;
	isDead = false;

	maxSpeed = 15.f;
	accel = 0.5f;
}

int RocketBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;


	switch (dir)
	{
	case DIR::UP:
		info.y -= speed;
		break;
	case DIR::DOWN:
		break;
	case DIR::LEFT:
		break;
	case DIR::RIGHT:
		break;
	}

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
	if (_id == OBJ::ENEMY)
	{

		isDead = true;
	}
}

void RocketBullet::Update_Rect()
{
	switch (dir)
	{
	case DIR::UP:
		rect.left = info.x - info.cx * 0.5f;
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
