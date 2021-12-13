#include "Bullet.h"
#include "Items.h"
#include "Manager.h"
#include "Enemy.h"
#include "Npc.h"
#include "Player.h"

void Bullet::Initialize()
{
	id = OBJ::BULLET;
	parentID = OBJ::PLAYER;
	speed = 15.f; 
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
		if (_id != parentID && !_opponent->Get_Dying())
		{
			_opponent->Set_Dying();
			isDead = true;
		}
		break;
	case OBJ::ENEMY:
		if (_id != parentID && !_opponent->Get_Dying())
		{
			DataMgr::Get_Instance()->Add_Score(25);
			_opponent->Add_HP(-damage);
			isDead = true;
		}
		break;
	case OBJ::PROP:
		if (static_cast<Item*>(_opponent)->Get_ItemID() == ITEM::ITEMBOX)
			isDead = true;
		break;
	case OBJ::BLOCK:
		if (parentID == OBJ::PLAYER && !_opponent->Get_Dying())
		{
			_opponent->Add_HP(-damage);
			isDead = true;
		}
		break;
	case OBJ::NPC:
		if (static_cast<Npc*>(_opponent)->Get_Ropped())
		{
			static_cast<Npc*>(_opponent)->Set_Ropped();
			isDead = true;
		}
		break;
	}
}