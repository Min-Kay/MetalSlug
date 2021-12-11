#include "BazukaBullet.h"

void BazukaBullet::Initialize()
{
	id = OBJ::BULLET;
	parentID = OBJ::ENEMY;
	speed = 10.f;
	info.cx = 35.f;
	info.cy = 35.f;
	damage = 20;
}

int BazukaBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;
	Update_Rect();
	return OBJ_DEFAULT;
}

void BazukaBullet::Late_Update()
{
}

void BazukaBullet::Render(HDC _hdc)
{
}

void BazukaBullet::Release()
{
}
