#include "Soldier.h"
#include "Include.h"
#include "Manager.h"
#include "Weapons.h"

void Soldier::Initialize()
{
	id = OBJ::ENEMY;
	render = RENDER::OBJECT;
	dir = DIR::RIGHT;
	onlySide = dir;
	sol_Class = SOLDIER::PRIVATE;
	info.cx = 100.f; 
	info.cy = 100.f;

	action = ACTION::IDLE;

	speed = 2.f; 
	hp = 10;
	isMove = false;
	ranAway = false;
	isJump = false;
	jumpForce = 20.f;
	jumpTime = 0; 
	jumping = false;
	canCollision = false;
	fireTime = GetTickCount();
	animIndex = 0; 

	attack = false;
	isDying = false;
	isFiring = false;
	isHolding = false;
	isHold = false;

	idleNum = rand() % 2;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier.bmp",L"Soldier");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier_Idle2.bmp", L"Soldier_Idle2");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier_Hold.bmp", L"Soldier_Hold");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_White.bmp", L"Stretch_White");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier_Fire.bmp",L"Soldier_Fire");

}

int Soldier::Update()
{
	if (isDead)
		return OBJ_DEAD;
	
	State_Machine(); 
	Jump();
	Update_Rect(); 

	return OBJ_DEFAULT;
}

void Soldier::Late_Update()
{
	Check_Hp();
}

void Soldier::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	Rectangle(_hdc,rect.left + scrollX,rect.top,rect.right + scrollX,rect.bottom);
	switch (action)
	{
		case ACTION::IDLE:
			Anim_Idle(_hdc);
			break;
		case ACTION::MOVE:
			Anim_Move(_hdc);
			break;
		case ACTION::DIE:
			Anim_Die(_hdc);
			break;
	}
}

void Soldier::Release()
{
}

void Soldier::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (isDead || isDying)
		return;

	switch (_id)
	{
	case OBJ::BULLET:
		if (static_cast<Bullet*>(_opponent)->Get_ParentID() != id)
		{
			Add_HP(-static_cast<Bullet*>(_opponent)->Get_Damage());
			break;
		}
	}
}

void Soldier::Jump()
{
	if (isDead)
		return;

	float fY = 0.f;
	float jumpingState = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x, info.y, &fY);

	if (isJump)
	{
		jumpingState = (jumpForce * jumpTime - 9.8f * jumpTime * jumpTime * 0.7f) / 2.f;
		info.y -= jumpingState;
		jumpTime += 0.2f;
		jumping = true;

		if (lineCol && info.y >= fY - info.cy * 0.5f && jumpingState < 0)
		{
			info.y = fY - info.cy * 0.5f;
			jumpTime = 0.f;
			isJump = false;
		}
	}
	else if (lineCol && info.y < fY - info.cy * 0.6f)
	{
		jumping = true;
		info.y += FALL_DOWN;
	}
	else if (lineCol && info.y >= fY - info.cy * 0.5f)
	{
		jumping = false;
		info.y = fY - info.cy * 0.5f;
	}
	else if (lineCol)
	{
		info.y = fY - info.cy * 0.5f;
		jumping = false;
	}
	else
	{
		jumping = true;
		info.y += FALL_DOWN;
	}
}

void Soldier::Anim_Idle(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White");

	if (attack)
	{
		if (isFiring)
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Fire");
			if (dir == DIR::RIGHT)
			{
				Anim_Counter(2, 50.f);
				StretchBlt(stretchDC, 0, 0, 60, 50, drawingDC, animIndex * 60 + 60, 0, -60, 50, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx * 1.2f, info.cy * 0.9f, stretchDC, 0, 0, 60, 50, RGB(255, 255, 255));
			}
			else if (dir == DIR::LEFT)
			{
				Anim_Counter(2, 50.f);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx * 1.2f, info.cy * 0.9f, drawingDC, animIndex * 60, 0, 60, 50, RGB(255, 255, 255));
			}
		}
		else
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Hold");
			if (dir == DIR::RIGHT)
			{
				Anim_Counter(5, 50.f, true);
				StretchBlt(stretchDC, 0, 0, 40, 50, drawingDC, animIndex * 40 + 40, 0, -40, 50, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx, info.cy * 0.9f, stretchDC, 0, 0, 40, 50, RGB(255, 255, 255));
			}
			else if (dir == DIR::LEFT)
			{
				Anim_Counter(5, 50.f, true);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx, info.cy * 0.9f, drawingDC, animIndex * 40, 0, 40, 50, RGB(255, 255, 255));
			}

		}
	}
	else
	{
		switch (idleNum)
		{
		case 0:
			switch (dir)
			{
			case DIR::RIGHT:
				Anim_Counter(3, 150.f);
				StretchBlt(stretchDC, 0, 0, 34, 40, drawingDC, animIndex * 34 + 34, 5, -34, 40, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx * 0.8f, info.cy * 0.9f, stretchDC, 0, 0, 34, 40, RGB(255, 255, 255));
				break;
			case DIR::LEFT:
				Anim_Counter(3, 150.f);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx * 0.8f, info.cy * 0.9f, drawingDC, animIndex * 34, 5, 34, 40, RGB(255, 255, 255));
				break;
			}
			break;
		case 1:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Idle2");

			switch (dir)
			{
			case DIR::RIGHT:
				Anim_Counter(9, 150.f);
				StretchBlt(stretchDC, 0, 0, 40, 50, drawingDC, animIndex * 40 + 40 - 5, 3, -40, 50, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 23.f, info.cx, info.cy, stretchDC, 0, 0, 40, 50, RGB(255, 255, 255));
				break;
			case DIR::LEFT:
				Anim_Counter(9, 150.f);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 23.f, info.cx, info.cy, drawingDC, animIndex * 40 - 5, 3, 40, 50, RGB(255, 255, 255));
				break;
			}
			break;
		}
	}
}

void Soldier::Anim_Move(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White");

	switch (dir)
	{
	case DIR::RIGHT:
		Anim_Counter(11, 70.f, true, 0);
		StretchBlt(stretchDC, 0, 0, 33, 40, drawingDC, animIndex * 33 + 33, 45, -33, 40, SRCCOPY);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx * 0.8f, info.cy * 0.9f, stretchDC, 0, 0, 33, 40, RGB(255, 255, 255));
		break;
	case DIR::LEFT:
		Anim_Counter(11, 70.f, true, 0);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx * 0.8f, info.cy * 0.9f, drawingDC, animIndex * 33, 45, 33, 40, RGB(255, 255, 255));
		break;
	}
}

void Soldier::Anim_Die(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White");

	switch (onlySide)
	{
	case DIR::RIGHT:
		Anim_Counter(10, 70.f, false, 0);
		StretchBlt(stretchDC, 0, 0, 35, 40, drawingDC, animIndex * 35 + 35 - 2, 1053, -35, 40, SRCCOPY);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx * 0.8f, info.cy * 0.9f, stretchDC, 0, 0, 35, 40, RGB(255, 255, 255));
		break;
	case DIR::LEFT:
		Anim_Counter(10, 70.f, false, 0);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx * 0.8f, info.cy * 0.9f, drawingDC, animIndex * 35 - 2, 1053, 35, 40, RGB(255, 255, 255));
		break;
	}
	if (animIndex == 10)
		Set_Dead(true);
	
}

void Soldier::State_Machine()
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	switch (action)
	{
	case ACTION::IDLE:

		if (attack)
		{
			if (ObjPoolMgr::Get_Instance()->Check_Distance(this) > 200 ||
				(abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) > 50 && (ObjPoolMgr::Get_Instance()->Get_Player_Rect().top > rect.bottom || ObjPoolMgr::Get_Instance()->Get_Player_Rect().bottom < rect.top)))
			{
				Change_Anim(ACTION::MOVE);
				attack = false;
				isFiring = false;
				isHolding = false;
				break;
			}

			if (fireTime + 1000.f < GetTickCount())
			{
				isFiring = true;

				if (ObjPoolMgr::Get_Instance()->Get_Player_Rect().bottom < rect.top)
				{
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::PISTOL, info.x, info.y - info.cy * 0.5f, DIR::UP, 0, OBJ::ENEMY);
				}
				else if (ObjPoolMgr::Get_Instance()->Get_Player_Rect().top > rect.bottom)
				{
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::PISTOL, info.x, info.y + info.cy * 0.5f, DIR::DOWN, 0, OBJ::ENEMY);
				}
				else if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x < info.x)
				{
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::PISTOL, info.x - info.cx * 0.5f, info.y, DIR::LEFT, 0, OBJ::ENEMY);
				}
				else if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x > info.x)
				{
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::PISTOL, info.x + info.cx * 0.5f, info.y, DIR::RIGHT, 0, OBJ::ENEMY);
				}
				fireTime = GetTickCount();
			}
			else if(fireTime + 500.f < GetTickCount())
			{
				isFiring = false;
				isHolding = true;
			}
		}

		if (!isMove && ObjPoolMgr::Get_Instance()->Check_Distance(this) < 300.f)
		{
			isMove = true; 
			Change_Anim(ACTION::MOVE);
			attack = false;
		}
		break;
	case ACTION::MOVE:
		if (sol_Class == SOLDIER::PRIVATE)
		{
			if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x < info.x)
			{
				info.x += speed * 1.5f;
				dir = DIR::RIGHT;
				onlySide = dir;
			}
			else
			{
				info.x -= speed * 1.5f;
				dir = DIR::LEFT;
				onlySide = dir;

			}
		}
		else
		{
			if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x < info.x)
			{
				info.x -= speed;
				dir = DIR::LEFT;
				onlySide = dir;

			}
			else if(ObjPoolMgr::Get_Instance()->Get_Player_Info().x > info.x)
			{
				info.x += speed;
				dir = DIR::RIGHT;
				onlySide = dir;

			}
			
		}

		if (sol_Class != SOLDIER::PRIVATE && abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < 200)
		{
			if (abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < 50)
			{
				Change_Anim(ACTION::IDLE);
				attack = true;
			}
			else if (ObjPoolMgr::Get_Instance()->Get_Player_Rect().bottom < rect.top || ObjPoolMgr::Get_Instance()->Get_Player_Rect().top > rect.bottom)
				break;	
			else
			{
				Change_Anim(ACTION::IDLE);
				attack = true;
			}
		}
		break;
	}
}

void Soldier::Check_Hp()
{
	if (isDead || isDying)
		return;

	if (hp <= 0)
	{
		DataMgr::Get_Instance()->Add_Score(100);
		isDying = true;
		animIndex = 0;
		action = ACTION::DIE;
	}
}
