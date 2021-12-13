#include "Soldier.h"
#include "Include.h"
#include "Manager.h"
#include "Bullets.h"

void Soldier::Initialize()
{
	id = OBJ::ENEMY;
	dir = DIR::RIGHT;
	onlySide = dir;
	sol_Class = SOLDIER::SERGENT;
	info.cx = 100.f; 
	info.cy = 100.f;

	action = ACTION::IDLE;

	speed = 2.f; 
	hp = 10;
	isMove = false;
	ranAway = false;
	coll_Attack = false;
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
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier_Hold2.bmp", L"Soldier_Hold");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_White2.bmp", L"Stretch_White2");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier_Fire.bmp",L"Soldier_Fire");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier_Fire_Up.bmp", L"Soldier_Fire_Up");

}

int Soldier::Update()
{
	if (isDead)
		return OBJ_DEAD;
	
	State_Machine(); 
	Gravity();
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

	//Rectangle(_hdc,rect.left + scrollX,rect.top + scrollY ,rect.right + scrollX,rect.bottom + scrollY);
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

void Soldier::Anim_Idle(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White2");

	if (attack)
	{
		if (isFiring)
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Fire");
			Anim_Counter(2, 50.f);
			if (dir == DIR::RIGHT)
			{
				StretchBlt(stretchDC, 0, 0, 60, 50, drawingDC, animIndex * 60 + 60, 0, -60, 50, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx * 1.2f, info.cy * 0.9f, stretchDC, 0, 0, 60, 50, RGB(255, 255, 255));
			}
			else if (dir == DIR::LEFT)
			{
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx * 1.2f, info.cy * 0.9f, drawingDC, animIndex * 60, 0, 60, 50, RGB(255, 255, 255));
			}
			else if (dir == DIR::UP)
			{
				if (onlySide == DIR::RIGHT)
				{
					drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Fire_Up");			
					StretchBlt(stretchDC, 0, 0, 40, 80, drawingDC, animIndex * 40 + 40 - 5, 0, -40, 80, SRCCOPY);
					GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - 40.f, info.cx, info.cy * 1.5f, stretchDC, 0, 0, 40, 80, RGB(255, 255, 255));

				}
				else
				{
					drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Fire_Up");
					GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - 40.f, info.cx, info.cy * 1.5f, drawingDC, animIndex * 40 - 5, 0, 40, 80, RGB(255, 255, 255));
				}
			}
		}
		else
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Hold");
			Anim_Counter(2, 50.f, false);
			if (onlySide == DIR::RIGHT)
			{
				StretchBlt(stretchDC, 0, 0, 45, 50, drawingDC, animIndex * 45 + 45, 0, -45, 50, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx, info.cy * 0.9f, stretchDC, 0, 0, 45, 50, RGB(255, 255, 255));
			}
			else if (onlySide == DIR::LEFT)
			{
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx, info.cy * 0.9f, drawingDC, animIndex * 45, 0, 45, 50, RGB(255, 255, 255));
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
				StretchBlt(stretchDC, 0, 0, 40, 50, drawingDC, animIndex * 40 - 5, 3, 40, 50, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 23.f, info.cx, info.cy, stretchDC, 0, 0, 40, 50, RGB(255, 255, 255));
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
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White2");

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
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White2");

	switch (onlySide)
	{
	case DIR::RIGHT:
		Anim_Counter(10, 30.f, false);
		StretchBlt(stretchDC, 0, 0, 35, 40, drawingDC, animIndex * 35 + 35 - 2, 1053, -35, 40, SRCCOPY);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) + 15.f, info.cx * 0.8f, info.cy * 0.9f, stretchDC, 0, 0, 35, 40, RGB(255, 255, 255));
		break;
	case DIR::LEFT:
		Anim_Counter(10, 30.f, false);
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
	float playerX = abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x);
	float playerY = abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().y - info.y);
	switch (action)
	{
	case ACTION::IDLE:

		if (!isMove)
		{
			if(ObjPoolMgr::Get_Instance()->Check_Distance(this) < 600.f)
			{ 
				isMove = true;
				Change_Anim(ACTION::IDLE);
				attack = false;
			}
			return;
		}

		if (attack)
		{
			if ((playerX > 350.f && playerY > 200) || (playerX > 45 && ObjPoolMgr::Get_Instance()->Get_Player_Rect().bottom < rect.top))
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
					dir = DIR::UP;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::ENEMYBULLET, info.x, info.y - info.cy * 0.5f, DIR::UP, OBJ::ENEMY);
				}
				else if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x < info.x)
				{
					dir = DIR::LEFT;
					onlySide = dir;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::ENEMYBULLET, info.x - info.cx * 0.5f, info.y, DIR::LEFT, OBJ::ENEMY);
				}
				else if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x > info.x)
				{
					dir = DIR::RIGHT;
					onlySide = dir;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::ENEMYBULLET, info.x + info.cx * 0.5f, info.y, DIR::RIGHT, OBJ::ENEMY);
				}
				fireTime = GetTickCount();
			}
			else if (fireTime + 300.f < GetTickCount())
			{
				isFiring = false;
				isHolding = true;
			}
		}
		else if (playerX < 300.f)
			action = ACTION::MOVE;

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
			else if(ObjPoolMgr::Get_Instance()->Get_Player_Info().x > info.x)
			{
				info.x -= speed * 1.5f;
				dir = DIR::LEFT;
				onlySide = dir;
			}
		}
		else
		{
			if (playerX < 70 && ObjPoolMgr::Get_Instance()->Get_Player_Info().y - info.y > 100)
			{
				falling = true;
			}
			else if(ObjPoolMgr::Get_Instance()->Get_Player_Info().y - info.y < 100)
			{
				falling = false;
			}

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

		if (sol_Class != SOLDIER::PRIVATE && abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < 300)
		{
			if (playerX < 200 && playerY < 100)
			{
				falling = false;
				Change_Anim(ACTION::IDLE);
				attack = true;
			}
			else if (ObjPoolMgr::Get_Instance()->Get_Player_Rect().top > rect.bottom)
				break;	
			else if(ObjPoolMgr::Get_Instance()->Get_Player_Rect().bottom < rect.top && playerX < 50)
			{
				falling = false;
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
		falling = false; 
		animIndex = 0;
		action = ACTION::DIE;
	}
}

void Soldier::Gravity()
{
	if (isDead)
		return;

	float fY = 0.f;
	float blockY = 0.f; 
	float jumpingState = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x, info.y, &fY);
	bool blockCol = BlockMgr::Get_Instance()->Collision_Block(this, &blockY);

	if (falling)
	{
		info.y += FALL_DOWN;
		return;
	}
	else if (blockCol && info.y > blockY)
	{
		info.y = blockY + 1;
	}
	if (lineCol && info.y < fY - info.cy * 0.6f)
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
