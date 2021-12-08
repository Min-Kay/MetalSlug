#include "Arabian.h"
#include "Manager.h"

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
	canCollision = false;
	coll_Attack = false;
	attacking = false;
	hp = 10;
	speed = 4.f;

	attackTime = GetTickCount();

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

	Rectangle(_hdc, rect.left + scrollX,rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY );

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
	if (coll_Attack)
	{

	}
	else if(attacking)
	{
		
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

	Anim_Counter(1,100.f,false);

	if (animIndex == 1)
		isDead = true;

}

void Arabian::State_Machine()
{
	switch (action)
	{
	case ACTION::IDLE:
		if (attacking && abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < 50)
		{
			coll_Attack = true;
		}
		else
		{
			Change_Anim(ACTION::MOVE);
			attacking = false;
			coll_Attack = false;
		}

		if (!isMove && ObjPoolMgr::Get_Instance()->Check_Distance(this) < 300.f)
		{
			isMove = true;
			Change_Anim(ACTION::MOVE);
			attacking = false;
		}
		break;
	case ACTION::MOVE:

		if (abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < 50)
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

	if (hp <= 0)
	{
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
