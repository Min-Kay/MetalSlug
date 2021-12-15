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
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Masknell_Turn.bmp", L"Masknell_Turn");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_White4.bmp", L"Stretch_White4");

}

int Masknell::Update()
{
	if (isDead)
		return OBJ_DEAD; 

	if (!isMove && abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < WINCX - 100)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Masknell.wav", CSoundMgr::ENEMY, 2.0f);
		isMove = true;
	}

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
		animIndex = 0;
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
				drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Masknell_Turn");

				Anim_Counter(11, 10.f, false);
				
				if (animIndex < 6)
				{
					GdiTransparentBlt(_hdc, int(info.x + scrollX), int(info.y + scrollY - info.cy * 0.5f), 140, 150, drawingDC, animIndex * 70, 0, 70, 75, RGB(255, 255, 255));
				}
				else
				{
					GdiTransparentBlt(_hdc, int(info.x + scrollX), int(info.y + scrollY - info.cy * 0.5f), 140, 150, drawingDC, (animIndex % 6) * 70, 75, 70, 75, RGB(255, 255, 255));
				}


				if (animIndex == 11)
				{
					turned = true;
					animIndex = 0;
				}
			}
			else
			{
				Anim_Counter(5, 10.f, false);

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
				StretchBlt(stretchDC, 0, 0, 75, 75, drawingDC, animIndex * 75 + 75 + 5, 266, -75, 75, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), 160, 150, stretchDC, 0, 0, 75, 75, RGB(255, 255, 255));
			}
			else
			{
				StretchBlt(stretchDC, 0, 0, 85, 75, drawingDC, (animIndex % 2) * 85 + 85 + 230, 266, -85, 75, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), 170, 150, stretchDC, 0, 0, 85, 75, RGB(255, 255, 255));
			}
		}
		break;
	case DIR::LEFT:
		if (isTurning)
		{
			if (!turned)
			{
				drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Masknell_Turn");
				Anim_Counter(11, 10.f, false);
				if (animIndex < 6)
				{
					StretchBlt(stretchDC, 0, 0, 70, 75, drawingDC, animIndex * 70 + 70, 0, -70, 75, SRCCOPY);

					GdiTransparentBlt(_hdc, int(info.x + scrollX), int(info.y + scrollY - info.cy * 0.5f), 140, 150, stretchDC, 0, 0, 70, 75, RGB(255, 255, 255));
				}
				else
				{
					StretchBlt(stretchDC, 0, 0, 70, 75, drawingDC, (animIndex % 6) * 70 + 70, 75, -70, 75, SRCCOPY);

					GdiTransparentBlt(_hdc, int(info.x + scrollX), int(info.y + scrollY - info.cy * 0.5f), 140, 150, stretchDC, 0, 0, 70, 75, RGB(255, 255, 255));
				}

				if (animIndex == 11)
				{
					turned = true;
					animIndex = 0;
				}
			}
			else
			{
				Anim_Counter(5, 10.f, false);

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
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), 150, 150, drawingDC, animIndex * 75 + 5, 266, 75, 75, RGB(255, 255, 255));
			}
			else
			{
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), 170, 150, drawingDC,  230 + (animIndex % 2) * 85, 266, 85, 75, RGB(255, 255, 255));
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
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ENEMY_ATTACK);
		CSoundMgr::Get_Instance()->PlaySound(L"Pistol_Shoot.wav", CSoundMgr::ENEMY_ATTACK, 2.0f);
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
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ENEMY);
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ENEMY_DIE);
		CSoundMgr::Get_Instance()->PlaySound(L"Explode2.wav", CSoundMgr::ENEMY_DIE, 1.0f);
		isDead = true;
	}
	else
	{
		info.y += FALL_DOWN;
	}
}
