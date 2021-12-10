#include "RocketBullet.h"
#include "Manager.h"
#include "Enemy.h"
#include "Player.h"
#include "Npc.h"

void RocketBullet::Initialize()
{
	damage = 20.f;
	parentID = OBJ::PLAYER;
	animIndex = 0;
	animTimer = GetTickCount();

	speed = 0.f; 
	blow = 0.f;

	info.cx = 50.f;
	info.cy = 50.f;

	init_CX = 130.f;
	init_CY = 130.f;

	dir = DIR::RIGHT;
	id = OBJ::BULLET;
	isDead = false;

	maxSpeed = 30.f;
	accel = 1.0f;

	explosion = false;
	
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rocket_Right.bmp",L"Rocket_Right");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rocket_Up.bmp", L"Rocket_Up");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rocket_Down.bmp", L"Rocket_Down");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp", L"Stretch_Item");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Explosion.bmp" , L"Explosion");
}

int RocketBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (!explosion)
	{
		Rocket();
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
	//Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);
	if (!explosion)
	{
		Anim_Counter(1, 100.f);
		switch (dir)
		{
		case DIR::UP:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Rocket_Up");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, init_CY, drawingDC, 0, 50 * animIndex, 20, 50, RGB(0, 248, 0));
			break;
		case DIR::DOWN:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Rocket_Down");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, init_CY, drawingDC, 0, 50 * animIndex, 20, 50, RGB(0, 248, 0));
			break;
		case DIR::LEFT:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Rocket_Right");
			stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
			StretchBlt(stretchDC, 0, 0, 50, 20, drawingDC, 50 * animIndex + 50, 0, -50, 20, SRCCOPY);
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, init_CX, info.cy, stretchDC, 0, 0, 50, 20, RGB(0, 248, 0));
			break;
		case DIR::RIGHT:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Rocket_Right");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, init_CX, info.cy, drawingDC, 50 * animIndex, 0, 50, 20, RGB(0, 248, 0));
			break;
		}
	}
	else
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Explosion");
		Anim_Counter(8,30.f,false);
		GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, init_CX, init_CY, drawingDC, 32 * animIndex + 11, 0, 32, 50, RGB(0, 248, 0));
		if (animIndex == 8)
		{
			Release();
			isDead = true;
		}
	}
}

void RocketBullet::Release()
{
	hits.erase(hits.begin(),hits.end());
	hits.clear();
}

void RocketBullet::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (_id == OBJ::ENEMY && _id != parentID)
	{
		if (_opponent->Get_Dying())
			return;

		ExplodePosing();
		if (find(hits.begin(), hits.end(), _opponent) == hits.end())
		{
			hits.push_back(_opponent);
			static_cast<Enemy*>(_opponent)->Add_HP(-damage);
		}
	}
	else if (_id == OBJ::PLAYER && _id != parentID)
	{
		if (_opponent->Get_Dying())
			return;

		ExplodePosing();
		if (find(hits.begin(), hits.end(), _opponent) == hits.end())
		{
			hits.push_back(_opponent);
			static_cast<Player*>(_opponent)->Set_Dying();
		}
	}
	else if (_id == OBJ::NPC && static_cast<Npc*>(_opponent)->Get_Ropped())
	{
		ExplodePosing();
		static_cast<Npc*>(_opponent)->Set_Ropped();
	}
}

void RocketBullet::Update_Rect()
{
	if (!explosion)
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
	else
	{
		rect.left = info.x - init_CX * 0.5f;
		rect.right = info.x + init_CX * 0.5f;
		rect.top = info.y - init_CY * 0.5f;
		rect.bottom = info.y + init_CY * 0.5f;
	}
}

void RocketBullet::Rocket()
{
	if (maxSpeed > speed)
	{
		speed += accel;
		blow = speed * 0.3f;
	}

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
	info.y -= blow;
	info.y += FALL_DOWN * 0.5f;
}

void RocketBullet::ExplodePosing()
{
	if (!explosion)
	{
		switch (dir)
		{
		case DIR::UP:
			info.y -= init_CY * 0.5f;
			break;
		case DIR::DOWN:
			info.y += init_CY * 0.5f;
			break;
		case DIR::LEFT:
			info.x -= init_CX * 0.5f;
			break;
		case DIR::RIGHT:
			info.x += init_CX * 0.5f;
			break;
		}
		Update_Rect();
		explosion = true;
	}
}
