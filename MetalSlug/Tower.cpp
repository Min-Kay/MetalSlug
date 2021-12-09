#include "Tower.h"
#include "Bullet.h"
#include "Manager.h"
#include "ThreeHead.h"

Tower::Tower()
{
}

Tower::Tower(ThreeHead* _parent, THREEHEAD::HEAD _head, float _X, float _Y)
{
	parent = _parent;
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
	hp = 300;
	dir = head == THREEHEAD::RIGHT ? DIR::LEFT : DIR::RIGHT;

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
	
	curtainIdle = false;
	firing = false;
	fireTime = GetTickCount();

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MidBoss.bmp",L"MidBoss");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp",L"Stretch_Item");

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
	if (!isDying && hp <= 0)
	{
		DataMgr::Get_Instance()->Add_Score(500);
		isDying = true; 
		state = THREEHEAD::DESTORY;
		animIndex = 0;
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
		switch (head)
		{
			case THREEHEAD::LEFT:
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 5, 184, 90, 140, RGB(0, 255, 0));
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 60, 70, drawingDC, 5, 635, 53, 60, RGB(0, 255, 0));
				Anim_Counter(15, 100.f, false);
				float x = info.x + scrollX - info.cx;
				float y = info.y + scrollY - info.cy * 0.8f;
				if (animIndex < 5)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + animIndex * 95, 353, 95, 60, RGB(0, 255, 0));
				else if (animIndex < 10)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 413, 95, 60, RGB(0, 255, 0));
				else if (animIndex < 15)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 473, 95, 60, RGB(0, 255, 0));
				else
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 533, 95, 60, RGB(0, 255, 0));

			}
			break;
			case THREEHEAD::MID:
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 95, 184, 90, 140, RGB(0, 255, 0));
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 60, 70, drawingDC, 5, 635, 53, 60, RGB(0, 255, 0));
				Anim_Counter(15, 100.f, false);
				float x = info.x + scrollX - info.cx;
				float y = info.y + scrollY - info.cy * 0.8f;
				if (animIndex < 5)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + animIndex * 95, 353, 95, 60, RGB(0, 255, 0));
				else if (animIndex < 10)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 413, 95, 60, RGB(0, 255, 0));
				else if (animIndex < 15)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 473, 95, 60, RGB(0, 255, 0));
				else
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 533, 95, 60, RGB(0, 255, 0));

			}
			break;
			case THREEHEAD::RIGHT:
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 185, 184, 90, 140, RGB(0, 255, 0));
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 60, 70, drawingDC, 5, 635, 53, 60, RGB(0, 255, 0));
				Anim_Counter(15, 100.f, false);

				float x = info.x + scrollX - info.cx;
				float y = info.y + scrollY - info.cy * 0.8f;
				if (animIndex < 5)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + animIndex * 95, 353, 95, 60, RGB(0, 255, 0));
				else if (animIndex < 10)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 413, 95, 60, RGB(0, 255, 0));
				else if (animIndex < 15)	  
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 473, 95, 60, RGB(0, 255, 0));
				else						   
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 533, 95, 60, RGB(0, 255, 0));

			}
			break;
		}
		if (animIndex == 15)
		{
			animIndex = 0; 
			state = THREEHEAD::CURTAIN_OPEN;
			animTimer = GetTickCount();
		}
		break;
	case THREEHEAD::CURTAIN_OPEN:
		switch (head)
		{
		case THREEHEAD::LEFT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 3, 850, 100, 140, RGB(0, 255, 0));
			
			if (curtainIdle)
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 150, 1765, 40, 50, RGB(0, 255, 0));
				curtainIdle = false;
			}
			else
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 190, 1765, 40, 50, RGB(0, 255, 0));
				curtainIdle = true;
			}
			break;
		case THREEHEAD::MID:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 103, 850, 100, 140, RGB(0, 255, 0));
			if (curtainIdle)
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 150, 1765, 40, 50, RGB(0, 255, 0));
				curtainIdle = false;
			}
			else
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 190, 1765, 40, 50, RGB(0, 255, 0));
				curtainIdle = true;
			}
			break;
		case THREEHEAD::RIGHT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 203, 850, 100, 140, RGB(0, 255, 0));
			if (curtainIdle)
			{
				stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
				StretchBlt(stretchDC,0,0,40,50,drawingDC, 190, 1765, -40,50,SRCCOPY);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, stretchDC, 0, 0, 40, 50, RGB(0, 255, 0));
				curtainIdle = false;
			}
			else
			{
				stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
				StretchBlt(stretchDC, 0, 0, 40, 50, drawingDC, 230, 1765, -40, 50, SRCCOPY);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, stretchDC, 0, 0, 40, 50, RGB(0, 255, 0));
				curtainIdle = true;
			}
			break;
		}

		Anim_Counter(12, 100.f, false);
		if (animIndex < 7)
		{
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.6f, info.y + scrollY - info.cy * 0.8f, 140, 140, drawingDC, 3 + animIndex * 70, 1021, 70, 70, RGB(0, 255, 0));
		}
		else
		{
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.6f, info.y + scrollY - info.cy * 0.8f, 140, 140, drawingDC, 3 + (animIndex % 7) * 70, 1091, 70, 70, RGB(0, 255, 0));
		}

		if (animIndex == 12)
		{
			state = THREEHEAD::ATTACK;
		}
		break;
	case THREEHEAD::ATTACK:

		switch (head)
		{
		case THREEHEAD::LEFT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 3, 850, 100, 140, RGB(0, 255, 0));

			if (firing)
			{
				Anim_Counter(4,70.f,true);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, animIndex * 39, 1823, 40, 50, RGB(0, 255, 0));

			}
			else
			{
				if (curtainIdle)
				{
					GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 150, 1765, 40, 50, RGB(0, 255, 0));
					curtainIdle = false;
				}
				else
				{
					GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 190, 1765, 40, 50, RGB(0, 255, 0));
					curtainIdle = true;
				}
			}
			
			break;
		case THREEHEAD::MID:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 103, 850, 100, 140, RGB(0, 255, 0));
			if (firing)
			{
				Anim_Counter(4, 70.f, true);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, animIndex * 39, 1823, 40, 50, RGB(0, 255, 0));

			}
			else
			{
				if (curtainIdle)
				{
					GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 150, 1765, 40, 50, RGB(0, 255, 0));
					curtainIdle = false;
				}
				else
				{
					GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 190, 1765, 40, 50, RGB(0, 255, 0));
					curtainIdle = true;
				}
			}
			
			break;
		case THREEHEAD::RIGHT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 203, 850, 100, 140, RGB(0, 255, 0));
			if (firing)
			{
				Anim_Counter(4, 70.f, true);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, animIndex * 39, 1823, 40, 50, RGB(0, 255, 0));

			
			}
			else
			{
				if (curtainIdle)
				{
					stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
					StretchBlt(stretchDC, 0, 0, 40, 50, drawingDC, 190, 1765, -40, 50, SRCCOPY);
					GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, stretchDC, 0, 0, 40, 50, RGB(0, 255, 0));
					curtainIdle = false;
				}
				else
				{
					stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
					StretchBlt(stretchDC, 0, 0, 40, 50, drawingDC, 230, 1765, -40, 50, SRCCOPY);
					GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, stretchDC, 0, 0, 40, 50, RGB(0, 255, 0));
					curtainIdle = true;
				}
			}
			break;
		}
		break;
	case THREEHEAD::DESTORY:
		switch (head)
		{
		case THREEHEAD::LEFT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 3, 2634, 100, 100, RGB(0, 255, 0));

			break;
		case THREEHEAD::MID:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 103, 2634, 100, 100, RGB(0, 255, 0));

			break;
		case THREEHEAD::RIGHT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 203, 2634, 100, 100, RGB(0, 255, 0));

			break;
		}

		break;
	}
}

void Tower::Release()
{
}

void Tower::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (isDying || !isMove)
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
	case THREEHEAD::ATTACK:
		isMove = true;
		if (fireTime + 2000.f < GetTickCount())
		{
			ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::PISTOL,info.x,info.y - info.cy,DIR::DOWN,0,OBJ::ENEMY);
			fireTime = GetTickCount();
			firing = true;
		}
		else if (fireTime + 700.f < GetTickCount())
		{
			firing = false;
		}
		break;
	}
}
