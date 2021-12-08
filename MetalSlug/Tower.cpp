#include "Tower.h"
#include "Bullet.h"
#include "Manager.h"

Tower::Tower()
{
}

Tower::Tower(THREEHEAD::HEAD _head, float _X, float _Y)
{
	head = _head; 
	info.x = _X;
	info.y = _Y;
	Initialize();
}

Tower::~Tower()
{
	Release(); 
}

void Tower::Initialize()
{
	enemy_id = ENEMY::THREEHEAD;
	action = ACTION::IDLE;
	hp = 500;
	dir = DIR::RIGHT;

	isMove = false;
	coll_Attack = false;
	isDying = false;

	canCollision = false;

	animIndex = 0;
	animTimer = GetTickCount();

	id = OBJ::ENEMY;
	render = RENDER::OBJECT;
	isDead = false;

	info.cx = 100.f;
	info.cy = 100.f;
	
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MidBoss.bmp",L"MidBoss");
}

int Tower::Update()
{
	if (isDead)
		return OBJ_DEAD;

	State_Machine();
	Update_Rect(); 

	return OBJ_DEFAULT;
}

void Tower::Late_Update()
{
	if (hp <= 0)
	{
		isDying = true; 
		state = THREEHEAD::DESTORY;
	}
}

void Tower::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"MidBoss");
	switch (state)
	{
	case THREEHEAD::NONE:
		switch (head)
		{
		case THREEHEAD::LEFT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 5, 184, 90, 140, RGB(0, 255, 0));
			break;
		case THREEHEAD::MID:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 95, 184, 90, 140, RGB(0, 255, 0));
			break;
		case THREEHEAD::RIGHT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 185, 184, 90, 140, RGB(0, 255, 0));
			break;
		}
		break;
	case THREEHEAD::DOOR_OPEN:
		break;
	case THREEHEAD::CURTAIN_OPEN:
		break;
	case THREEHEAD::ATTACK:
		if (dir == DIR::RIGHT)
		{

		}
		else
		{
			
		}
		break;
	case THREEHEAD::DESTORY:
		break;
	}
}

void Tower::Release()
{
}

void Tower::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (isDying || isMove)
		return;

	if (_id == OBJ::BULLET)
	{
		if (static_cast<Bullet*>(_opponent)->Get_ParentID() == OBJ::PLAYER)
			Add_HP(-static_cast<Bullet*>(_opponent)->Get_Damage());
	}
}

void Tower::State_Machine()
{
	switch (state)
	{
	case THREEHEAD::NONE:
		if (!isMove)
			return;
		break;
	case THREEHEAD::DOOR_OPEN:
		break;
	case THREEHEAD::CURTAIN_OPEN:
		break;
	case THREEHEAD::ATTACK:
		break;
	case THREEHEAD::DESTORY:
		break;
	}
}
