#include "Bullet.h"

void Bullet::Initialize()
{
	id = OBJ::BULLET;
	render = RENDER::OBJECT;
	parentID = OBJ::PLAYER;
	speed = 10.f; 
	info.cx = 10.f;
	info.cy = 10.f;
}

int Bullet::Update()
{
	if (isDead)
		return OBJ_DEAD; 

	switch (dir)
	{
	case DIR::UP:
		info.y -= speed; 
		break;
	case DIR::DOWN:
		info.y += speed;
		break;
	case DIR::LEFT:
		info.x -= speed;
		break;
	case DIR::RIGHT:
		info.x += speed;
		break;
	}

	Update_Rect(); 

	return OBJ_DEFAULT;
}

void Bullet::Late_Update()
{
	if (0 > info.x ||
		0 > info.y ||
		WINCX < info.x ||
		WINCY < info.y)
		isDead = true; 
}

void Bullet::Render(HDC _hdc)
{
	Ellipse(_hdc, rect.left, rect.top, rect.right, rect.bottom);
}

void Bullet::Release()
{
}
