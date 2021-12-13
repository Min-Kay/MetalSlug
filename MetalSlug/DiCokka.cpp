#include "DiCokka.h"
#include "Manager.h"

void DiCokka::Initialize()
{
	enemy_id = ENEMY::DICOKKA;
	speed = 2.f;

	isMove = false;
	coll_Attack = false;
	canCollision = true;

	animIndex = 0;
	animTimer = GetTickCount();
	info.cx = 150.f;
	info.cy = 120.f;
	dir = DIR::RIGHT;
	id = OBJ::ENEMY;
	isDead = false;
	isDying = false;
	state = STATE::IDLE;
	hp = 300.f;

	isTurning = false;
	isFiring = false;
	fire = false;
	attackTimer = GetTickCount();
	fireTimer = GetTickCount();
	fireEffectTimer = GetTickCount();

	maxCount = 3;
	currCount = 0;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/DiCokka.bmp",L"DiCokka");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_White.bmp", L"Stretch_White");
}

int DiCokka::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (!isMove && abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < 500.f)
	{
		isMove = true; 
	}

	State_Machine();
	Gravity();
	Update_Rect(); 
	return OBJ_DEFAULT;
}

void DiCokka::Late_Update()
{
	if (isDead || isDying)
		return;

	if (hp <= 0)
	{
		DataMgr::Get_Instance()->Add_Kill(1);
		DataMgr::Get_Instance()->Add_Score(250);
		animIndex = 0;
		state = STATE::DESTROY;
		isDying = true;
		canCollision = false;
		coll_Attack = false;
	}
}

void DiCokka::Render(HDC _hdc)
{
	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	//Rectangle(_hdc,rect.left + x,rect.top + y,rect.right + x,rect.bottom + y);

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"DiCokka");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White");
	switch (state)
	{
	case DiCokka::IDLE:
		if (isTurning)
		{
			Anim_Counter(10, 100.f, false);
			switch (dir)
			{
			case DIR::LEFT:
				if (animIndex < 4)
				{
					GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, animIndex * 72, 237, 72, 60, RGB(255, 255, 255));
				}
				else if (animIndex < 7)
				{
					GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, 290 + (animIndex % 4) * 65, 235, 65, 60, RGB(255, 255, 255));
				}
				else
				{
					GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, (animIndex % 7) * 72, 297, 72, 60, RGB(255, 255, 255));
				}
				break;
			case DIR::RIGHT:
				if (animIndex < 4)
				{
					StretchBlt(stretchDC, 0, 0, 72, 60, drawingDC, animIndex * 72 + 72, 237, -72, 60, SRCCOPY);
					GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 72, 60, RGB(255, 255, 255));
				}
				else if (animIndex < 7)
				{
					StretchBlt(stretchDC, 0, 0, 65, 60, drawingDC, (animIndex % 4) * 65 + 290 + 65, 235, -65, 60, SRCCOPY);
					GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 65, 60, RGB(255, 255, 255));
				}
				else
				{
					StretchBlt(stretchDC, 0, 0, 72, 60, drawingDC, (animIndex % 7) * 72 + 72, 297, -72, 60, SRCCOPY);
					GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 72, 60, RGB(255, 255, 255));
				}
				break;
			}
			if (animIndex == 10)
			{
				animIndex = 0;
				dir = dir == DIR::RIGHT ? DIR::LEFT : DIR::RIGHT;
				attackTimer = GetTickCount();
				isTurning = false;
			}
		}
		else if (isFiring)
		{
			Anim_Counter(10, 30.f,false);
			switch (dir)
			{
			case DIR::LEFT:
				if (animIndex < 7)
				{
					GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, animIndex * 75 - 3, 360, 75, 60, RGB(255, 255, 255));
				}
				else
				{
					GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, (animIndex % 7) * 75, 420, 75, 60, RGB(255, 255, 255));
				}
				break;
			case DIR::RIGHT:
				if (animIndex < 7)
				{
					StretchBlt(stretchDC, 0, 0, 75, 60, drawingDC, animIndex * 75 + 75 - 3, 360, -75, 60, SRCCOPY);
					GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 75, 60, RGB(255, 255, 255));
				}
				else
				{
					StretchBlt(stretchDC, 0, 0, 75, 60, drawingDC, (animIndex % 7) * 75 + 75, 420, -75, 60, SRCCOPY);
					GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 75, 60, RGB(255, 255, 255));
				}
				break;
			}
			if (animIndex == 10)
				isFiring = false;
		}
		else
		{
			Anim_Counter(1,100.f);
			switch (dir)
			{
			case DIR::LEFT:
				GdiTransparentBlt(_hdc,rect.left + x,rect.top + y,info.cx,info.cy,drawingDC,animIndex * 75,0,75,60,RGB(255,255,255));
				break;
			case DIR::RIGHT:
				StretchBlt(stretchDC,0,0,75,60,drawingDC, animIndex * 75 + 75, 0, -75, 60,SRCCOPY);
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 75, 60, RGB(255, 255, 255));
				break;
			}
		}
		break;
	case DiCokka::MOVE:
		Anim_Counter(11, 100.f);
		switch (dir)
		{
		case DIR::LEFT:
			if (animIndex < 6)
			{
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, animIndex * 75, 58, 75, 60, RGB(255, 255, 255));
			}
			else
			{
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, (animIndex % 6) * 75, 118, 75, 60, RGB(255, 255, 255));
			}
			break;
		case DIR::RIGHT:
			if (animIndex < 6)
			{
				StretchBlt(stretchDC, 0, 0, 75, 60, drawingDC, animIndex * 75 + 75, 58, -75, 60, SRCCOPY);
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 75, 60, RGB(255, 255, 255));
			}
			else
			{
				StretchBlt(stretchDC, 0, 0, 75, 60, drawingDC, (animIndex % 6) * 75 + 75, 118, -75, 60, SRCCOPY);
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 75, 60, RGB(255, 255, 255));
			}
			break;
		}
		break;
	case DiCokka::DESTROY:
		Anim_Counter(5, 100.f,false);
		switch (dir)
		{
		case DIR::LEFT:
			if (animIndex < 2)
			{
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, animIndex * 72 + 4, 675, 72, 65, RGB(255, 255, 255));
			}
			else
			{
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, animIndex * 80, 675, 80, 65, RGB(255, 255, 255));
			}
			break;
		case DIR::RIGHT:
			if (animIndex < 2)
			{
				StretchBlt(stretchDC, 0, 0, 72, 65, drawingDC, animIndex * 72 + 72 + 4, 680, -72, 65, SRCCOPY);
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 72, 65, RGB(255, 255, 255));
			}
			else
			{
				StretchBlt(stretchDC, 0, 0, 80, 65, drawingDC, animIndex * 80 + 80, 680, -80, 65, SRCCOPY);
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 80, 65, RGB(255, 255, 255));
			}
			break;
		}
		if (animIndex == 5)
			isDead = true; 
		break;
	}
}

void DiCokka::Release()
{
}

void DiCokka::State_Machine()
{
	if (isDead || isDying)
		return;

	float playerX = abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x);
	switch (state)
	{
	case DiCokka::IDLE:
		coll_Attack = false;
		if (!isMove)
			return; 

		if (!isTurning && !fire)
		{
			if ((ObjPoolMgr::Get_Instance()->Get_Player_Info().x > info.x && dir == DIR::LEFT) ||
				(ObjPoolMgr::Get_Instance()->Get_Player_Info().x < info.x && dir == DIR::RIGHT))
			{
				isTurning = true;
				break;
			}

			if (playerX > 350.f)
			{
				animIndex = 0;
				state = STATE::MOVE;
				break;
			}
		}

		if (!isTurning && attackTimer + 4000.f < GetTickCount())
		{
			fire = true;
			if (!isFiring && currCount == maxCount)
			{
				fire = false;
				currCount = 0;
				attackTimer = GetTickCount();
			}

			if (currCount < maxCount && fireTimer + 700.f < GetTickCount())
			{
				if (dir == DIR::RIGHT)
				{
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::DICOKKA,rect.right,rect.top + 25.f,dir,OBJ::ENEMY);
				}
				else
				{
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::DICOKKA,rect.left, rect.top + 25.f, dir, OBJ::ENEMY);
				}
				animIndex = 0;
				isFiring = true;
				++currCount;
				fireTimer = GetTickCount();
			}
		}

		break;
	case DiCokka::MOVE:
		coll_Attack = true;
		if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x > info.x)
		{
			info.x += speed; 
			dir = DIR::RIGHT;
		}
		else
		{
			info.x -= speed; 
			dir = DIR::LEFT;
		}

		if (playerX < 350.f)
		{
			animIndex = 0;
			state = STATE::IDLE;
		}

		break;
	case DiCokka::DESTROY:
		coll_Attack = false;
		break;
	}
}
