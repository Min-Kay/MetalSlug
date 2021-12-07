#include "Bullet.h"
#include "Items.h"

void Bullet::Initialize()
{
	id = OBJ::BULLET;
	render = RENDER::OBJECT;
	parentID = OBJ::PLAYER;
	speed = 10.f; 
	info.cx = 10.f;
	info.cy = 10.f;
	damage = 10;
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

void Bullet::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	switch (_id)
	{
	case OBJ::PLAYER:
		if (_opponent->Get_ID() != parentID)
			isDead = true;
		break;
	case OBJ::ENEMY:
		if (_opponent->Get_ID() != parentID)
			isDead = true;
		break;
	case OBJ::PROP:
		if (static_cast<Item*>(_opponent)->Get_ItemID() == ITEM::ITEMBOX)
			isDead = true;
		break;
	case OBJ::BLOCK:
		isDead = true;
		break;
	}
}

void Bullet::Anim_Counter(int count, float _timer, bool _roop, int start)
{
	if (animIndex < start)
		animIndex = start;

	if (animTimer + _timer < GetTickCount())
	{
		if (_roop && animIndex >= start + count)
		{
			animIndex = start;
		}
		else  if (animIndex < start + count)
			++animIndex;

		animTimer = GetTickCount();
	}
}
