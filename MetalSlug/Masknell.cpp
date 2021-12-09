#include "Masknell.h"
#include "Bullet.h"
#include "Manager.h"

void Masknell::Initialize()
{
	info.cx = 150.f;
	info.cy = 150.f;

	animIndex = 0;
	animTimer = GetTickCount();

	dir = DIR::LEFT;
	id = OBJ::ENEMY;
	render = RENDER::OBJECT;
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
}

int Masknell::Update()
{
	if (isDead)
		return OBJ_DEAD; 

	if (!isMove && abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < WINCX)
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

	if (hp <= 0)
	{
		isDying = true;
	}

	if (-scrollX > rect.left ||
		WINCX - scrollX < rect.right)
	{
		dir = dir == DIR::RIGHT ? DIR::LEFT : DIR::RIGHT;
		isTurning = true;
	}

	
}

void Masknell::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	Rectangle(_hdc,rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);


	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Masknell");
	switch (dir)
	{
	case DIR::RIGHT:
		if (isTurning)
		{
			if (!turned)
			{
				Anim_Counter(11, 40.f, false);
				if (animIndex == 0)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 120, 150, drawingDC, 5, 577, 60, 70, RGB(255, 255, 255));
				else if (animIndex == 1)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 110, 150, drawingDC, 65, 577, 50, 70, RGB(255, 255, 255));

				else if (animIndex == 2)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 105, 150, drawingDC, 115, 577, 45, 70, RGB(255, 255, 255));

				else if (animIndex == 3)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 110, 150, drawingDC, 160, 577, 50, 70, RGB(255, 255, 255));

				else if (animIndex == 4)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 120, 150, drawingDC, 210, 577, 60, 70, RGB(255, 255, 255));

				else if (animIndex == 5)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 120, 150, drawingDC, 270, 577, 60, 70, RGB(255, 255, 255));
				else if (animIndex == 6)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 140, 150, drawingDC, 5, 647, 70, 70, RGB(255, 255, 255));
				else if (animIndex == 7)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 115, 150, drawingDC, 75, 647, 55, 70, RGB(255, 255, 255));
				else if (animIndex == 8)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 105, 150, drawingDC, 130, 647, 45, 70, RGB(255, 255, 255));
				else if (animIndex == 9)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 105, 150, drawingDC, 175, 647, 45, 70, RGB(255, 255, 255));
				else if (animIndex == 10)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 110, 150, drawingDC, 220, 647, 50, 70, RGB(255, 255, 255));
				else if (animIndex == 11)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 140, 150, drawingDC, 270, 647, 70, 70, RGB(255, 255, 255));

				if (animIndex == 11)
				{
					turned = true;
					animIndex = 0;
				}
			}
			else
			{
				if (animIndex == 0)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 120, 150, drawingDC, 5, 577, 60, 70, RGB(255, 255, 255));
				else if (animIndex == 1)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 110, 150, drawingDC, 65, 577, 50, 70, RGB(255, 255, 255));

				else if (animIndex == 2)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 105, 150, drawingDC, 115, 577, 45, 70, RGB(255, 255, 255));

				else if (animIndex == 3)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 110, 150, drawingDC, 160, 577, 50, 70, RGB(255, 255, 255));

				else if (animIndex == 4)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 120, 150, drawingDC, 210, 577, 60, 70, RGB(255, 255, 255));

				else if (animIndex == 5)
					GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 120, 150, drawingDC, 270, 577, 60, 70, RGB(255, 255, 255));

				if (animIndex == 5)
				{
					isTurning = false;
					animIndex = 0;
				}
			}
		
		}
		else
		{
			Anim_Counter(5, 40.f, true);
			if (animIndex < 4)
			{
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 170, 150, drawingDC, 5 + animIndex * 85, 0, 85, 70, RGB(255, 255, 255));
			}
			else
			{
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 170, 150, drawingDC, 5, 70, 85, 70, RGB(255, 255, 255));
			}
		}
		break;
	case DIR::LEFT:
		if (isTurning)
		{
			Anim_Counter(11, 40.f, false);
			if (animIndex == 0)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 120, 150, drawingDC, 5, 577, 60, 70, RGB(255, 255, 255));
			else if (animIndex == 1)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 110, 150, drawingDC, 65, 577, 50, 70, RGB(255, 255, 255));

			else if (animIndex == 2)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 105, 150, drawingDC, 115, 577, 45, 70, RGB(255, 255, 255));

			else if (animIndex == 3)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 110, 150, drawingDC, 160, 577, 50, 70, RGB(255, 255, 255));

			else if (animIndex == 4)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 120, 150, drawingDC, 210, 577, 60, 70, RGB(255, 255, 255));

			else if (animIndex == 5)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 120, 150, drawingDC, 270, 577, 60, 70, RGB(255, 255, 255));
			else if (animIndex == 6)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 140, 150, drawingDC, 5, 647, 70, 70, RGB(255, 255, 255));
			else if (animIndex == 7)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 115, 150, drawingDC, 75, 647, 55, 70, RGB(255, 255, 255));
			else if (animIndex == 8)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 105, 150, drawingDC, 130, 647, 45, 70, RGB(255, 255, 255));
			else if (animIndex == 9)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 105, 150, drawingDC, 175, 647, 45, 70, RGB(255, 255, 255));
			else if (animIndex == 10)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 110, 150, drawingDC, 220, 647, 50, 70, RGB(255, 255, 255));
			else if (animIndex == 11)
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 140, 150, drawingDC, 270, 647, 70, 70, RGB(255, 255, 255));


			if (animIndex == 11)
				isTurning = false;
		}
		else
		{
			Anim_Counter(5, 40.f, true);
			if (animIndex < 4)
			{
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 170, 150, drawingDC, 5 + animIndex * 85, 0, 85, 70, RGB(255, 255, 255));
			}
			else
			{
				GdiTransparentBlt(_hdc, int(info.x + scrollX - info.cx * 0.5f), int(info.y + scrollY - info.cy * 0.5f), 170, 150, drawingDC, 5, 70, 85, 70, RGB(255, 255, 255));
			}
		}
		break;
	}
}

void Masknell::Release()
{
}

void Masknell::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (isDead || isDying)
		return;

	switch (_id)
	{
	case OBJ::BULLET:
		if (static_cast<Bullet*>(_opponent)->Get_ParentID() != id)
			Add_HP(-static_cast<Bullet*>(_opponent)->Get_Damage());
		break;
	}
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
	
	if (attackTime + 2000.f < GetTickCount())
	{
		ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::PISTOL,info.x,rect.bottom,DIR::DOWN,0,OBJ::ENEMY);
		attackTime = GetTickCount();
	}
}
