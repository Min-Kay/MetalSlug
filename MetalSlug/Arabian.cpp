#include "Arabian.h"
#include "Manager.h"
#include "Bullets.h"

void Arabian::Initialize()
{
	info.cx = 100.f;
	info.cy = 100.f; 
	id = OBJ::ENEMY;
	enemy_id = ENEMY::ARABIAN;
	render = RENDER::OBJECT;
	dir = DIR::LEFT;
	action = ACTION::IDLE; 
	isDead = false;
	isMove = false;
	isDying = false;
	canCollision = false;
	coll_Attack = false;
	attacking = false;
	hp = 10;
	speed = 4.f;
	falling = false; 
	attackTime = GetTickCount();

	isJump = false;
	jumpTime = 0.f;
	jumpY = 0.f;
	jumping = false;
	jumpForce = 40.f;


	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Arabian.bmp",L"Arabian");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_White.bmp", L"Stretch_White");

}

int Arabian::Update()
{
	if (isDead)
		return OBJ_DEAD;

	State_Machine();
	Gravity(); 
	Update_Rect(); 
	return OBJ_DEFAULT;
}

void Arabian::Late_Update()
{
	Check_Hp();
}

void Arabian::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	//Rectangle(_hdc, rect.left + scrollX,rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY );

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

void Arabian::Release()
{
}

void Arabian::Anim_Idle(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Arabian");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White");
	if (attacking && attackTime + 1500.f < GetTickCount())
	{
		coll_Attack = true; 
		if (dir == DIR::RIGHT)
		{
			Anim_Counter(4, 70.f,false);
			StretchBlt(stretchDC, 0, 0, 75, 70, drawingDC, animIndex * 75 + 75 - 5, 710, -75, 70, SRCCOPY);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - info.cy * 0.4f, info.cx * 1.5f, info.cy * 1.5f, stretchDC, 0, 0, 75, 70, RGB(255, 255, 255));
		}
		else if (dir == DIR::LEFT)
		{
			Anim_Counter(4, 70.f, false);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - info.cy * 0.4f, info.cx * 1.5f, info.cy * 1.5f, drawingDC, animIndex * 75 - 5, 710, 75, 70, RGB(255, 255, 255));
		}
		if (animIndex == 4)
		{
			coll_Attack = false;
			animIndex = 0; 
			attackTime = GetTickCount();
		}
	}
	else if (attacking && attackTime + 1500.f > GetTickCount())
	{
		if (dir == DIR::RIGHT)
		{
			Anim_Counter(1, 100.f);
			StretchBlt(stretchDC, 0, 0, 60, 60, drawingDC, animIndex * 60 + 60, 620, -60, 60, SRCCOPY);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx * 0.1f, int(rect.top + scrollY) - info.cy * 0.15f, info.cx * 1.2f, info.cy * 1.2f, stretchDC, 0, 0, 60, 60, RGB(255, 255, 255));
		}
		else if (dir == DIR::LEFT)
		{
			Anim_Counter(1, 100.f);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX) + info.cx * 0.1f, int(rect.top + scrollY) - info.cy * 0.15f, info.cx * 1.2f, info.cy * 1.2f, drawingDC, animIndex * 60, 620, 60, 60, RGB(255, 255, 255));
		}
	}
	else
	{
		if (dir == DIR::RIGHT)
		{
			Anim_Counter(5, 100.f);
			StretchBlt(stretchDC, 0, 0, 40, 50, drawingDC, animIndex * 40 + 40 + 5, 25, -40, 50, SRCCOPY);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx , info.cy * 1.1f, stretchDC, 0,0, 40, 50, RGB(255, 255, 255));

			
		}
		else if (dir == DIR::LEFT)
		{
			Anim_Counter(5,100.f);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy * 1.1f, drawingDC, animIndex * 40 + 5, 25, 40, 50, RGB(255, 255, 255));

		}
	}
}

void Arabian::Anim_Move(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Arabian");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White");

	if (dir == DIR::RIGHT)
	{
		Anim_Counter(2, 70.f);
		StretchBlt(stretchDC, 0, 0, 55, 55, drawingDC, animIndex * 55 + 55 + 303, 205, -55, 55, SRCCOPY);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx * 1.1f, info.cy * 1.1f, stretchDC, 0, 0, 55, 55, RGB(255, 255, 255));


	}
	else if (dir == DIR::LEFT)
	{
		Anim_Counter(2, 70.f);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx * 1.1f, info.cy * 1.1f, drawingDC, animIndex * 55 + 303, 205, 55, 55, RGB(255, 255, 255));
	}

}

void Arabian::Anim_Die(HDC _hdc)
{
	if (isDead)
		return; 

	coll_Attack = false;

	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Arabian");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White");

	if (dir == DIR::RIGHT)
	{
		Anim_Counter(13, 70.f, false);
		if (animIndex < 6)
		{
			StretchBlt(stretchDC, 0, 0, 35, 45, drawingDC, animIndex * 35 + 35 + 10, 1045, -35, 45, SRCCOPY);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx * 0.8f, info.cy, stretchDC, 0, 0, 35, 45, RGB(255, 255, 255));
		}
		else
		{
			StretchBlt(stretchDC, 0, 0, 45, 45, drawingDC, animIndex * 45 + 45 + 20, 1045, -45, 45, SRCCOPY);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx * 0.8f, info.cy, stretchDC, 0, 0, 45, 45, RGB(255, 255, 255));
		}
	}
	else if (dir == DIR::LEFT)
	{
		Anim_Counter(13, 50.f,false);
		if(animIndex < 6)
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx * 0.8f, info.cy , drawingDC, animIndex * 35 + 10, 1045, 35, 45, RGB(255, 255, 255));
		else 
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx * 0.8f, info.cy, drawingDC, animIndex * 45 + 20, 1045, 45, 45, RGB(255, 255, 255));

	}

	if (animIndex == 13)
		isDead = true;

}

void Arabian::State_Machine()
{
	if (isDying)
		return;

	float disX = abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x);
	float disY = ObjPoolMgr::Get_Instance()->Get_Player_Info().y - info.y;

	switch (action)
	{
	case ACTION::IDLE:
	{
		if (!isMove)
		{
			if (disX < 400.f)
			{
				isMove = true;
				Change_Anim(ACTION::MOVE);
				attacking = false;
			}
			return;
		}

		if (!attacking && disX < 70 && disY < -200)
		{
			isJump = true;
			falling = false;
		}
		else if (disX < 70 && abs(disY) <= 100)
		{
			falling = false;
			attacking = true;
		}
		else if (abs(disY) < 100)
		{
			falling = false;
			Change_Anim(ACTION::MOVE);
			attacking = false;
			coll_Attack = false;
		}
		else if (disX < 70 && disY > 100)
		{
			falling = true;
		}
		else if (attacking)
		{
			falling = false;
			if (disX > 70)
				Change_Anim(ACTION::MOVE);
			attacking = false;
			coll_Attack = false;
		}

	}
		break;
	case ACTION::MOVE:

		if (disX < 70)
		{
			Change_Anim(ACTION::IDLE);
			attacking = true;
		}
		
		if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x > info.x)
		{
			info.x += speed;
			dir = DIR::RIGHT;
		}
		else if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x < info.x)
		{
			info.x -= speed;
			dir = DIR::LEFT;
		}
		break;
	}
}

void Arabian::Check_Hp()
{
	if (isDead || isDying)
		return;

	if ( hp <= 0)
	{
		DataMgr::Get_Instance()->Add_Score(100);
		animIndex = 0;
		action = ACTION::DIE;
		isDying = true; 
	}
}

void Arabian::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (isDead || isDying)
		return;

	switch (_id)
	{
	case OBJ::BULLET:
		if(static_cast<Bullet*>(_opponent)->Get_ParentID() != id)
			Add_HP(-static_cast<Bullet*>(_opponent)->Get_Damage());
		break;
	}
}

void Arabian::Gravity()
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

		if (lineCol && info.y >= fY - info.cy * 0.5f && jumpingState < 0)
		{
			info.y = fY - info.cy * 0.5f;
			if (action != ACTION::DIE) action = ACTION::IDLE;
			jumpTime = 0.f;
			isJump = false;
		}
	}
	else if (falling)
	{
		info.y += FALL_DOWN;
		return;
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
