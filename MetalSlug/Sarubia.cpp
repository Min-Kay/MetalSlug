#include "Sarubia.h"
#include "Manager.h"
void Sarubia::Initialize()
{
	enemy_id = ENEMY::SARUBIA;
	speed = 3.f;

	isMove = false;
	coll_Attack = false;
	canCollision = true;

	animIndex = 0;
	animTimer = GetTickCount(); 
	info.cx = 185.f;
	info.cy = 250.f;
	dir = DIR::RIGHT;
	id = OBJ::ENEMY; 
	isDead = false;
	isDying = false;
	state = STATE::IDLE;
	hp = 200.f;

	isFiring = false;
	effectEnd = false; 
	attackTimer = GetTickCount();
	fireTimer = GetTickCount();
	fireEffectTimer = GetTickCount();

	maxCount = 5;
	currCount = 0; 

	fireEffectIndex = 0;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Sarubia.bmp",L"Sarubia");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Sarubia_Bullet.bmp", L"Sarubia_Bullet");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_White6.bmp", L"Stretch_White6");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp", L"Stretch_Item");

}

int Sarubia::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (!isMove && abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < 700.f)
		isMove = true; 

	State_Machine();
	Gravity();
	Update_Rect(); 

	return OBJ_DEFAULT;
}

void Sarubia::Late_Update()
{
	if (isDying)
		return;

	if (hp <= 0)
	{
		animIndex = 0; 
		isDying = true;
		state = STATE::DESTROY;
	}
}

void Sarubia::Render(HDC _hdc)
{
	float x = CScrollMgr::Get_Instance()->Get_ScrollX(); 
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	//Rectangle(_hdc, rect.left + x, rect.top + y, rect.right + x, rect.bottom + y);

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Sarubia");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White6");
	switch (state)
	{
	case STATE::IDLE:
		if (isFiring)
		{
			Anim_Counter(5, 100.f,false);
			if (dir == DIR::LEFT)
			{
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, 75 * animIndex + 7, 810, 75, 100, RGB(255, 255, 255));

				drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Sarubia_Bullet");
				stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
				if (fireEffectTimer + 100.f < GetTickCount())
				{
					++fireEffectIndex;
					fireEffectTimer = GetTickCount();
					if (fireEffectIndex > 8)
						effectEnd = true;
				}

				if (!effectEnd && fireEffectIndex < 2)
				{
					GdiTransparentBlt(_hdc, rect.left - info.cx * 0.4f + x, rect.top + y, 120, 120, drawingDC, 45 * fireEffectIndex, 20, 45, 45, RGB(0, 248, 0));
				}
				else if (!effectEnd && fireEffectIndex < 9)
				{
					GdiTransparentBlt(_hdc, rect.left - info.cx * 0.4f + x, rect.top + y, 120, 120, drawingDC, 50 * fireEffectIndex - 18, 20, 50, 45, RGB(0, 248, 0));
				}
			}
			else
			{
				StretchBlt(stretchDC,0,0,75,100,drawingDC, 75 * animIndex + 4 - 75, 810, -75, 100,SRCCOPY);
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 75, 100, RGB(255, 255, 255));

				drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Sarubia_Bullet");
				stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
				if (fireEffectTimer + 100.f < GetTickCount())
				{
					++fireEffectIndex;
					fireEffectTimer = GetTickCount();
					if (fireEffectIndex > 8)
						effectEnd = true;
				}

				if (!effectEnd && fireEffectIndex < 2)
				{
					StretchBlt(stretchDC, 0, 0, 45, 45, drawingDC, 45 * fireEffectIndex + 45, 20, -45, 45, SRCCOPY);
					GdiTransparentBlt(_hdc, rect.right - info.cx * 0.2f + x, rect.top + y, 120, 120, stretchDC, 0, 0, 45, 45, RGB(0, 248, 0));
				}
				else if (!effectEnd && fireEffectIndex < 9)
				{
					StretchBlt(stretchDC, 0, 0, 50, 45, drawingDC, 50 * fireEffectIndex - 18 + 50, 20, -50, 45, SRCCOPY);
					GdiTransparentBlt(_hdc, rect.right - info.cx * 0.2f + x, rect.top + y, 120, 120, stretchDC, 0, 0, 50, 45, RGB(0, 248, 0));
				}
			}

			if (animIndex == 5)
			{
				animIndex = 0; 
				fireEffectIndex = 0;
				isFiring = false; 
				effectEnd = false;
			}
		}
		else
		{
			Anim_Counter(1,100.f);
			if (dir == DIR::LEFT)
			{
				GdiTransparentBlt(_hdc,rect.left + x, rect.top + y,info.cx,info.cy,drawingDC,75*animIndex,0,75,100,RGB(255,255,255));
			}
			else
			{
				StretchBlt(stretchDC, 0, 0, 75, 100, drawingDC, 75 * animIndex + 75, 0, -75, 100, SRCCOPY);
				GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 75, 100, RGB(255, 255, 255));
			}
		}
		break;
	case STATE::MOVE:
		break;
	case STATE::DESTROY:
		Anim_Counter(5, 150.f,false);
		if (dir == DIR::LEFT)
		{
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, drawingDC, 85 * animIndex - 10, 1198, 85, 100, RGB(255, 255, 255));
		}
		else
		{
			StretchBlt(stretchDC, 0, 0, 85, 100, drawingDC, 85 * animIndex - 10 + 85, 1198, -85, 100, SRCCOPY);
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy, stretchDC, 0, 0, 85, 100, RGB(255, 255, 255));
		}
		if (animIndex == 5)
			isDead = true;

		break; 
	}
}
void Sarubia::Release()
{
}

void Sarubia::State_Machine()
{
	switch (state)
	{
	case STATE::IDLE:
		if (!isMove)
			return;

		if (attackTimer + 5000.f < GetTickCount())
		{
			if (fireTimer + 1500.f < GetTickCount())
			{
				isFiring = true; 
				if(dir == DIR::LEFT)
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::SARUBIA,rect.left, rect.top + 50.f,dir,OBJ::ENEMY);
				else 
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::SARUBIA, rect.right, rect.top + 50.f, dir, OBJ::ENEMY);
				++currCount;
				fireTimer = GetTickCount();
				if (currCount == maxCount)
				{
					currCount = 0;
					attackTimer = GetTickCount();
				}
			}
		}
		break;
	case STATE::MOVE:
		break;
	case STATE::DESTROY:
		canCollision = false; 
		break;
	}
}
