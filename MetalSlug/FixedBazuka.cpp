#include "FixedBazuka.h"
#include "Manager.h"

void FixedBazuka::Initialize()
{
	id = OBJ::ENEMY;
	enemy_id = ENEMY::BAZUKA;
	info.cx = 100.f;
	info.cy = 100.f;

	isMove = false;
	coll_Attack = false;
	canCollision = false;
	isDead = false;
	isDying = false; 
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier_Bazuka.bmp", L"Soldier_Bazuka");
}

int FixedBazuka::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (!isMove)
	{
		return OBJ_DEFAULT;
	}

	Attack();
	Update_Rect();
	return OBJ_DEFAULT;
}

void FixedBazuka::Late_Update()
{
	if (isDying || isDead)
		return;
}

void FixedBazuka::Render(HDC _hdc)
{
	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Bazuka");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White");

	switch (dir)
	{
	case DIR::LEFT:
		Anim_Counter(1, 100.f);
		GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, animIndex * 48 + 3, 398, 48, 48, RGB(255, 255, 255));
		break;
	case DIR::RIGHT:
		Anim_Counter(1, 100.f);
		StretchBlt(stretchDC, 0, 0, 48, 48, drawingDC, animIndex * 48 + 48 + 3, 398, -48, 48, SRCCOPY);
		GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 48, 48, RGB(255, 255, 255));
		break;
	case DIR::DOWN:
		GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx - 10, info.cy, drawingDC, 277, 398, 40, 48, RGB(255, 255, 255));
		break;
	case DIR::DOWN_RIGHT:
		StretchBlt(stretchDC, 0, 0, 48, 48, drawingDC, 148, 398, -48, 48, SRCCOPY);
		GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 48, 48, RGB(255, 255, 255));
		break;
	case DIR::DOWN_LEFT:
		GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, 100, 398, 48, 48, RGB(255, 255, 255));
		break;
	}
}

void FixedBazuka::Release()
{
}

void FixedBazuka::Attack()
{
	if (!isMove)
		return;

	float playerX = ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x;
	float playerY = ObjPoolMgr::Get_Instance()->Get_Player_Info().y - info.y;

	if (playerX < -100)
	{
		if (playerY < 100)
			dir = DIR::LEFT;
		else
			dir = DIR::DOWN_LEFT;
	}
	else if (playerX > 100)
	{
		if (playerY < 100)
			dir = DIR::RIGHT;
		else
			dir = DIR::DOWN_RIGHT;
	}
	else
	{
		if (playerY > 100)
			dir = DIR::DOWN;
		else if (abs(playerX) < 50 && playerX > -50)
			dir = DIR::RIGHT;
		else if (abs(playerX) < 50 && playerX < 50)
			dir = DIR::LEFT;
	}

	if (fireTime + 3500.f < GetTickCount())
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ENEMY_ATTACK);
		CSoundMgr::Get_Instance()->PlaySound(L"Rocket_Shoot.wav", CSoundMgr::ENEMY_ATTACK, 2.f);
		switch (dir)
		{
		case DIR::DOWN:
			ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, info.x, rect.bottom, DIR::DOWN, OBJ::ENEMY);
			break;
		case DIR::LEFT:
			ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.left, info.y, DIR::LEFT, OBJ::ENEMY);
			break;
		case DIR::RIGHT:
			ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.right, info.y, DIR::RIGHT, OBJ::ENEMY);
			break;
		case DIR::DOWN_RIGHT:
			ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.right, rect.bottom, DIR::DOWN_RIGHT, OBJ::ENEMY);
			break;
		case DIR::DOWN_LEFT:
			ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.left, rect.bottom, DIR::DOWN_LEFT, OBJ::ENEMY);
			break;
		}
		fireTime = GetTickCount();
	}
}
