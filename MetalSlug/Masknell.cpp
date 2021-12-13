#include "Masknell.h"
#include "Bullets.h"
#include "Manager.h"

void Masknell::Initialize()
{
	info.cx = 150.f;
	info.cy = 150.f;

	animIndex = 0;
	animTimer = GetTickCount();

	dir = DIR::LEFT;
	id = OBJ::ENEMY;
	isDead = false;

	enemy_id = ENEMY::MASKNELL;
	action = ACTION::IDLE;
	hp = 200;
	speed = 7.f;

	isMove = false;
	coll_Attack = false;
	isDying = false;

	canCollision = false ;

	isTurning = false;
	turned = false;

	attackTime = GetTickCount(); 

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Masknell.bmp",L"Masknell");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_White4.bmp", L"Stretch_White4");

}

int Masknell::Update()
{
	if (isDead)
		return OBJ_DEAD; 

	if (!isMove && abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < WINCX - 100)
		isMove = true;

	State_Machine(); 

	if (isDying)
		Gravity();

	Update_Rect();

	return OBJ_DEFAULT;
}

void Masknell::Late_Update()
{
	if (!isMove)
		return; 

	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (isDying && info.y > WINCY - scrollY)
	{
		isDead = true;
	}

	if (!isDying && hp <= 0)
	{
		DataMgr::Get_Instance()->Add_Kill(1);
		DataMgr::Get_Instance()->Add_Score(300);
		isDying = true;
	}

	if (isMove && (-scrollX > rect.left ||
		WINCX - scrollX < rect.right))
	{
		dir = dir == DIR::RIGHT ? DIR::LEFT : DIR::RIGHT;
		isTurning = true;
	}

	
}

void Masknell::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	//Rectangle(_hdc,rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Masknell");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White4");
	switch (dir)
	{
	case DIR::RIGHT:
		if (isTurning)
		{
			if (!turned)
			{
				Anim_Counter(11, 20.f, false);
				if (animIndex == 0)
				{
					StretchBlt(stretchDC,0,0, 60, 70, drawingDC ,65, 577, -60, 70, SRCCOPY);
					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 120, 150, stretchDC, 0, 0, 60, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 1)
				{
					StretchBlt(stretchDC, 0, 0, 50, 70, drawingDC, 115, 577, -50, 70, SRCCOPY);
					GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), 110, 150, stretchDC, 0, 0, 50, 70, RGB(255, 255, 255));
				}

				else if (animIndex == 2)
				{
					StretchBlt(stretchDC, 0, 0, 45, 70, drawingDC, 160, 577, -45, 70, SRCCOPY);
					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 105, 150, stretchDC, 0, 0, 45, 70, RGB(255, 255, 255));
				}

				else if (animIndex == 3)
				{
					StretchBlt(stretchDC, 0, 0, 50, 70, drawingDC, 210, 577, -50, 70, SRCCOPY);
					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY), 110, 150, stretchDC, 0, 0, 50, 70, RGB(255, 255, 255));
				}

				else if (animIndex == 4)
				{
					StretchBlt(stretchDC, 0, 0, 60, 70, drawingDC, 270, 577, -60, 70, SRCCOPY);
					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 120, 150, stretchDC, 0, 0, 60, 70, RGB(255, 255, 255));
				}

				else if (animIndex == 5)
				{
					StretchBlt(stretchDC, 0, 0, 60, 70, drawingDC, 330, 577, -60, 70, SRCCOPY);
					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 120, 150, stretchDC, 0, 0, 60, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 6)
				{
					StretchBlt(stretchDC, 0, 0, 70, 70, drawingDC, 75, 647, -70, 70, SRCCOPY);
					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 140, 150, stretchDC, 0, 0, 70, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 7)
				{
					StretchBlt(stretchDC, 0, 0, 55, 70, drawingDC, 130, 647, -55, 70, SRCCOPY);
					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 115, 150, stretchDC, 0, 0, 55, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 8)
				{
					StretchBlt(stretchDC, 0, 0, 45, 70, drawingDC, 175, 647, -45, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 105, 150, stretchDC, 0, 0, 45, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 9)
				{
					StretchBlt(stretchDC, 0, 0, 45, 70, drawingDC, 220, 647, -45, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY), 105, 150, stretchDC, 0, 0, 45, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 10)
				{
					StretchBlt(stretchDC, 0, 0, 50, 70, drawingDC, 270, 647, -50, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 110, 150, stretchDC, 0, 0, 50, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 11)
				{
					StretchBlt(stretchDC, 0, 0, 70, 70, drawingDC, 340, 647, -70, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY), 140, 150, stretchDC, 0, 0, 70, 70, RGB(255, 255, 255));
				}

				if (animIndex == 11)
				{
					turned = true;
					animIndex = 0;
				}
			}
			else
			{
				Anim_Counter(5, 20.f, false);

				if (animIndex == 0)
				{
					StretchBlt(stretchDC, 0, 0, 60, 70, drawingDC, 330, 577, -60, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(info.x + scrollX ), int(info.y + scrollY - info.cy * 0.5f), 120, 150, stretchDC, 0, 0, 60, 70, RGB(255, 255, 255));
				}

				else if (animIndex == 1)
				{
					StretchBlt(stretchDC, 0, 0, 60, 70, drawingDC, 270, 577, -60, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 120, 150, stretchDC, 0, 0, 60, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 2)
				{
					StretchBlt(stretchDC, 0, 0, 50, 70, drawingDC, 210, 577, -50, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 110, 150, stretchDC, 0, 0, 50, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 3)
				{
					StretchBlt(stretchDC, 0, 0, 45, 70, drawingDC, 160, 577, -45, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 105, 150, stretchDC, 0, 0, 45, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 4)
				{
					StretchBlt(stretchDC, 0, 0, 50, 70, drawingDC, 115, 577, -50, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY ), 110, 150, stretchDC, 0, 0, 50, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 5)
				{
					StretchBlt(stretchDC, 0, 0, 60, 70, drawingDC, 65, 577, -60, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 120, 150, stretchDC,0, 0, 60, 70, RGB(255, 255, 255));
				}
				

				if (animIndex == 5)
				{
					isTurning = false;
					turned = false;
					animIndex = 0;
				}
			}
		
		}
		else
		{
			Anim_Counter(5, 20.f, true);
			if (animIndex < 4)
			{
				StretchBlt(stretchDC, 0, 0, 85, 70, drawingDC,  animIndex * 85 + 85, 0, -85, 70, SRCCOPY);

				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY ), 170, 150, stretchDC, 0, 0, 85, 70, RGB(255, 255, 255));
			}
			else
			{
				StretchBlt(stretchDC, 0, 0, 85, 70, drawingDC,  100, 65, -85, 70, SRCCOPY);

				GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 170, 150, stretchDC, 0, 0, 85, 70, RGB(255, 255, 255));
			}
		}
		break;
	case DIR::LEFT:
		if (isTurning)
		{
			if (!turned)
			{
				Anim_Counter(11, 20.f, false);
				if (animIndex == 0)
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 120, 150, drawingDC, 5, 577, 60, 70, RGB(255, 255, 255));
				else if (animIndex == 1)									
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 110, 150, drawingDC, 65, 577, 50, 70, RGB(255, 255, 255));
																			 
				else if (animIndex == 2)									 
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 105, 150, drawingDC, 115, 577, 45, 70, RGB(255, 255, 255));
																			
				else if (animIndex == 3)									
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 110, 150, drawingDC, 160, 577, 50, 70, RGB(255, 255, 255));
																			 
				else if (animIndex == 4)		 							
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 120, 150, drawingDC, 210, 577, 60, 70, RGB(255, 255, 255));
																			 
				else if (animIndex == 5)		 							 
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 120, 150, drawingDC, 270, 577, 60, 70, RGB(255, 255, 255));
				else if (animIndex == 6)		 							
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 140, 150, drawingDC, 5, 647, 70, 70, RGB(255, 255, 255));
				else if (animIndex == 7)		 							
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 115, 150, drawingDC, 75, 647, 55, 70, RGB(255, 255, 255));
				else if (animIndex == 8)									
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 105, 150, drawingDC, 130, 647, 45, 70, RGB(255, 255, 255));
				else if (animIndex == 9)		 							 
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 105, 150, drawingDC, 175, 647, 45, 70, RGB(255, 255, 255));
				else if (animIndex == 10)		 							 
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 110, 150, drawingDC, 220, 647, 50, 70, RGB(255, 255, 255));
				else if (animIndex == 11)									 
					GdiTransparentBlt(_hdc, int(rect.left  + scrollX ), int(rect.top  + scrollY ), 140, 150, drawingDC, 270, 647, 70, 70, RGB(255, 255, 255));

				if (animIndex == 11)
				{
					turned = true;
					animIndex = 0;
				}
			}
			else
			{
				Anim_Counter(5, 20.f, false);

				if (animIndex == 0)
				{
					StretchBlt(stretchDC, 0, 0, 60, 70, drawingDC, 270, 577, 60, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 120, 150, stretchDC, 0, 0, 60, 70, RGB(255, 255, 255));
				}

				else if (animIndex == 1)
				{
					StretchBlt(stretchDC, 0, 0, 60, 70, drawingDC, 210, 577, 60, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 120, 150, stretchDC, 0, 0, 60, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 2)
				{
					StretchBlt(stretchDC, 0, 0, 50, 70, drawingDC, 160, 577, 50, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY ), 110, 150, stretchDC, 0, 0, 50, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 3)
				{
					StretchBlt(stretchDC, 0, 0, 45, 70, drawingDC, 115, 577, 45, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 105, 150, stretchDC, 0, 0, 45, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 4)
				{
					StretchBlt(stretchDC, 0, 0, 50, 70, drawingDC, 65, 577, 50, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY), 110, 150, stretchDC, 0, 0, 50, 70, RGB(255, 255, 255));
				}
				else if (animIndex == 5)
				{
					StretchBlt(stretchDC, 0, 0, 60, 70, drawingDC, 0, 577, 60, 70, SRCCOPY);

					GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY), 120, 150, stretchDC, 0, 0, 60, 70, RGB(255, 255, 255));
				}
				if (animIndex == 5)
				{
					isTurning = false;
					turned = false;
					animIndex = 0;
				}
			}
		}
		else
		{
			Anim_Counter(5, 20.f, true);
			if (animIndex < 4)
			{
				GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 170, 150, drawingDC, animIndex * 85, 0, 85, 70, RGB(255, 255, 255));
			}
			else
			{
				GdiTransparentBlt(_hdc, int(rect.left + scrollX ), int(rect.top + scrollY ), 170, 150, drawingDC, 15, 65, 85, 70, RGB(255, 255, 255));
			}
		}
		break;
	}
}

void Masknell::Release()
{
}

void Masknell::State_Machine()
{
	if (!isMove)
		return;

	switch (dir)
	{
	case DIR::LEFT:
		info.x -= speed;
		break;
	case DIR::RIGHT:
		info.x += speed;
		break;
	}

	if (isDying)
		return;
	
	if (attackTime + 2000.f < GetTickCount())
	{
		isFiring = true;
		ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::ENEMYBULLET,info.x, (float)rect.bottom,DIR::DOWN,OBJ::ENEMY);
		attackTime = GetTickCount();
	}
	else if (isFiring && attackTime + 200.f < GetTickCount())
	{
		isFiring = false;
	}
}

void Masknell::Gravity()
{
	if (isDead)
		return;

	float fY = 0.f;
	float jumpingState = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x, info.y, &fY);

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
		isDead = true;
	}
	else
	{
		info.y += FALL_DOWN;
	}
}
