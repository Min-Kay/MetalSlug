#include "Bullet.h"
#include "Items.h"
#include "Manager.h"
#include "Enemy.h"
#include "Npc.h"

void Bullet::Initialize()
{
	id = OBJ::BULLET;
	render = RENDER::OBJECT;
	parentID = OBJ::PLAYER;
	speed = 20.f; 
	info.cx = 30.f;
	info.cy = 30.f;
	damage = 10;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/PistolBullet.bmp",L"PistolBullet");
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
	Check_Screen_Out();
	Check_Line_Collision();
}

void Bullet::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"PistolBullet");
	GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, 0, 0, 15, 15, ITEM_COLOR);
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
		if (_opponent->Get_ID() != parentID && !static_cast<Enemy*>(_opponent)->Get_Dying())
			isDead = true;
		break;
	case OBJ::PROP:
		if (static_cast<Item*>(_opponent)->Get_ItemID() == ITEM::ITEMBOX)
			isDead = true;
		break;
	case OBJ::BLOCK:
		isDead = true;
	case OBJ::NPC:
		if(static_cast<Npc*>(_opponent)->Get_Ropped())
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
