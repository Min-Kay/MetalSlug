#include "Bazuka.h"
#include "Parachute.h"
#include "Manager.h"

void Bazuka::Initialize()
{
	para = new Parachute;
	para->Initialize();
	para->Set_Parent(this);
	ObjPoolMgr::Get_Instance()->Add_Object(OBJ::ENEMY, para);

	id = OBJ::ENEMY;
	enemy_id = ENEMY::BAZUKA;
	dir = DIR::RIGHT;
	info.cx = 100.f;
	info.cy = 100.f;

	speed = 2.f;
	hp = 10;

	state = STATE::PARACHUTE;

	isMove = false;
	jumpForce = 20.f;
	jumpTime = 0;
	jumping = false;
	coll_Attack = false;
	canCollision = false;
	falling = false; 
	fireTime = GetTickCount();
	coll_Attack = false;

	isDying = false;
	isDead = false;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier_Bazuka.bmp",L"Soldier_Bazuka");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier.bmp", L"Soldier");
}

int Bazuka::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (!isMove)
	{
		if (abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < 700.f)
			isMove = true;
	}

	State_Machine(); 
	Gravity();
	Update_Rect();
	return OBJ_DEFAULT;
}

void Bazuka::Late_Update()
{
	if (isDying || isDead)
		return; 

	Check_Parachute();
	if (hp <= 0)
	{
		animIndex = 0;
		isDying = true;
		state = STATE::DIE;
		if (para)
		{
			ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, para);
			SAFE_DELETE(para);
		}
	}
}

void Bazuka::Render(HDC _hdc)
{
	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Bazuka");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White");

	switch (state)
	{
	case Bazuka::IDLE:
		switch (dir)
		{
		case DIR::LEFT:
			Anim_Counter(1,100.f);
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
		break;
	case Bazuka::MOVE:
		switch (dir)
		{
		case DIR::LEFT:
			Anim_Counter(10, 100.f);
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, animIndex * 43, 0, 43, 50, RGB(255, 255, 255));
			break;
		case DIR::RIGHT:
			Anim_Counter(10, 100.f);
			StretchBlt(stretchDC, 0, 0, 43, 50, drawingDC, animIndex * 43 + 43, 0, -43, 50, SRCCOPY);
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 43, 50, RGB(255, 255, 255));
			break;
		}
		break;
	case Bazuka::PARACHUTE:
		switch (dir)
		{
		case DIR::LEFT:
			GdiTransparentBlt(_hdc,rect.left + x, rect.top + y, info.cx, info.cy, drawingDC,0,659,45,50,RGB(255,255,255));
			break;
		case DIR::RIGHT:
			StretchBlt(stretchDC,0,0,45,50, drawingDC, 45, 659, -45, 50,SRCCOPY);
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 45, 50, RGB(255, 255, 255));
			break;
		case DIR::DOWN:
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx - 10, info.cy, drawingDC, 364, 659, 40, 50, RGB(255, 255, 255));
			break;
		case DIR::DOWN_RIGHT:
			StretchBlt(stretchDC, 0, 0, 48, 50, drawingDC, 233, 659, -48, 50, SRCCOPY);
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 48, 50, RGB(255, 255, 255));
			break;
		case DIR::DOWN_LEFT:
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, 185, 659, 48, 50, RGB(255, 255, 255));
			break;
		}
		break;
	case Bazuka::DIE:
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier");
		Anim_Counter(10, 70.f, false, 0);
		GdiTransparentBlt(_hdc, int(rect.left + x), int(rect.top + y), info.cx, info.cy , drawingDC, animIndex * 35 - 2, 1053, 35, 40, RGB(255, 255, 255));
		if (animIndex == 10)
			Set_Dead(true);
		break;
	}
}

void Bazuka::Release()
{
}

void Bazuka::Check_Parachute()
{
	if (para && para->Get_Dead())
	{
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, para);
		SAFE_DELETE(para);
	}
}

void Bazuka::Gravity()
{
	if (isDead)
		return;

	float fY = 0.f;
	float blockY = 0.f;
	float jumpingState = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x, info.y, &fY);
	bool blockCol = BlockMgr::Get_Instance()->Collision_Block(this, &blockY);

	if (para && ((lineCol && info.y >= fY - info.cy * 0.5f ) || blockCol && info.y > blockY))
	{
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, para);
		SAFE_DELETE(para);
	}
	else if (para)
	{
		info.y += 0.1f;
	}
	else if (blockCol && info.y > blockY)
	{
		info.y = blockY + 1;
	}
	else if (lineCol && info.y < fY - info.cy * 0.6f)
	{
		info.y += FALL_DOWN;
	}
	else if (lineCol && info.y >= fY - info.cy * 0.5f)
	{
		info.y = fY - info.cy * 0.5f;
	}
	else if (lineCol)
	{
		info.y = fY - info.cy * 0.5f;
	}
	else
	{
		info.y += FALL_DOWN;
	}
}

void Bazuka::State_Machine()
{
	if (!isMove)
		return;

	float playerX = ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x;
	float playerY = ObjPoolMgr::Get_Instance()->Get_Player_Info().y - info.y;

	switch (state)
	{
	case Bazuka::IDLE:
			if (abs(playerX) > 400.f )
			{
				action = ACTION::MOVE;
				break;
			}

			if (fireTime + 1200.f < GetTickCount())
			{
				if (playerX < -100)
				{
					if (playerY < 100)
					{
						dir = DIR::LEFT;
						ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.left, info.y, DIR::LEFT, OBJ::ENEMY);
					}
					else
					{
						dir = DIR::DOWN_LEFT;
						ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.left, rect.bottom, DIR::DOWN_LEFT, OBJ::ENEMY);
					}
				}
				else if (playerX > 100)
				{
					if (playerY < 100)
					{
						dir = DIR::RIGHT;
						ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.right, info.y, DIR::RIGHT, OBJ::ENEMY);
					}
					else
					{
						dir = DIR::DOWN_RIGHT;
						ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.right, rect.bottom, DIR::DOWN_RIGHT, OBJ::ENEMY);
					}
				}
				else
				{
					dir = DIR::DOWN;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, info.x, rect.bottom, DIR::DOWN, OBJ::ENEMY);
				}
				fireTime = GetTickCount();
			}
	
		break;
	case Bazuka::MOVE:
		if (abs(playerX) < 100 && playerY > 100)
			falling = true;
		else if (playerY < 100)
			falling = false;

		if (playerX < 0)
		{
			info.x -= speed;
			dir = DIR::LEFT;
		}
		else if (playerX > 0)
		{
			info.x += speed;
			dir = DIR::RIGHT;
		}
		break;
	case Bazuka::PARACHUTE:
		if (!para)
		{
			state = STATE::IDLE;
			break;
		}

		if (fireTime + 1200.f < GetTickCount())
		{
			if (playerX < -100)
			{
				if (playerY < 100)
				{
					dir = DIR::LEFT;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.left, info.y, DIR::LEFT, OBJ::ENEMY);
				}
				else
				{
					dir = DIR::DOWN_LEFT;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.left, rect.bottom, DIR::DOWN_LEFT, OBJ::ENEMY);
				}
			}
			else if (playerX > 100)
			{
				if (playerY < 100)
				{
					dir = DIR::RIGHT;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.right, info.y, DIR::RIGHT, OBJ::ENEMY);
				}
				else
				{
					dir = DIR::DOWN_RIGHT;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.right, rect.bottom, DIR::DOWN_RIGHT, OBJ::ENEMY);
				}
			}
			else
			{
				dir = DIR::DOWN;
				ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, info.x, rect.bottom, DIR::DOWN, OBJ::ENEMY);
			}
			fireTime = GetTickCount();
		}
		break;
	case Bazuka::DIE:
		break;
	}
}
