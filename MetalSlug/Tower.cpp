#include "Tower.h"
#include "Bullets.h"
#include "Manager.h"
#include "ThreeHead.h"
#include "AbstractFactory.h"

Tower::Tower()
{
	head = THREEHEAD::LEFT;
	state = THREEHEAD::NONE;
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
	isDead = false;

	info.cx = 100.f;
	info.cy = 100.f;

	firing = false;
	fireTime = GetTickCount();

	effectIndex = 0;
	fireEffectTime = GetTickCount();
}

Tower::Tower(THREEHEAD::HEAD _head, float _X, float _Y)
{
	state = THREEHEAD::NONE;
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
	isDead = false;

	info.cx = 100.f;
	info.cy = 100.f;

	firing = false;
	fireTime = GetTickCount();

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
	isDead = false;

	info.cx = 100.f;
	info.cy = 100.f;
	
	firing = false;
	fireTime = GetTickCount();

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MidBoss.bmp",L"MidBoss");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp",L"Stretch_Item");

}

int Tower::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (isDying)
		return OBJ_DEFAULT;

	State_Machine();
	Update_Rect(); 

	return OBJ_DEFAULT;
}

void Tower::Late_Update()
{
	auto iter = bullets.begin();
	for (; iter != bullets.end();)
	{
		if ((*iter)->Get_Dead() || isDying)
		{
			ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::BULLET,*iter);
			delete *iter;
			iter = bullets.erase(iter);
		}
		else
		{
			++iter;
		}
	}

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

	//Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"MidBoss");
	switch (state)
	{
	case THREEHEAD::NONE:
		switch (head)
		{
			case THREEHEAD::LEFT:
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 5, 184, 90, 140, THREEHEAD_COLOR);
				break;
			case THREEHEAD::MID:
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 95, 184, 90, 140, THREEHEAD_COLOR);
				break;
			case THREEHEAD::RIGHT:
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 185, 184, 90, 140, THREEHEAD_COLOR);
				break;
		}
		break;
	case THREEHEAD::DOOR_OPEN:
		switch (head)
		{
			case THREEHEAD::LEFT:
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 5, 184, 90, 140, THREEHEAD_COLOR);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.5f, info.y + scrollY - info.cy * 0.7f, 100, 120, drawingDC, 5, 635, 53, 60, THREEHEAD_COLOR);
				Anim_Counter(15, 50.f, false);
				float x = info.x + scrollX - info.cx;
				float y = info.y + scrollY - info.cy * 0.8f;
				if (animIndex < 5)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + animIndex * 95, 353, 95, 60, THREEHEAD_COLOR);
				else if (animIndex < 10)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 413, 95, 60, THREEHEAD_COLOR);
				else if (animIndex < 15)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 473, 95, 60, THREEHEAD_COLOR);
				else
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 533, 95, 60, THREEHEAD_COLOR);

			}
			break;
			case THREEHEAD::MID:
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 95, 184, 90, 140, THREEHEAD_COLOR);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.5f, info.y + scrollY - info.cy * 0.7f, 100, 120, drawingDC, 5, 635, 53, 60, THREEHEAD_COLOR);
				Anim_Counter(15, 50.f, false);
				float x = info.x + scrollX - info.cx;
				float y = info.y + scrollY - info.cy * 0.8f;
				if (animIndex < 5)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + animIndex * 95, 353, 95, 60, THREEHEAD_COLOR);
				else if (animIndex < 10)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 413, 95, 60, THREEHEAD_COLOR);
				else if (animIndex < 15)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 473, 95, 60, THREEHEAD_COLOR);
				else
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 533, 95, 60, THREEHEAD_COLOR);

			}
			break;
			case THREEHEAD::RIGHT:
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 180, 280, drawingDC, 185, 184, 90, 140, THREEHEAD_COLOR);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.5f, info.y + scrollY - info.cy * 0.7f, 100, 120, drawingDC, 5, 635, 53, 60, THREEHEAD_COLOR);
				Anim_Counter(15, 50.f, false);

				float x = info.x + scrollX - info.cx;
				float y = info.y + scrollY - info.cy * 0.8f;
				if (animIndex < 5)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + animIndex * 95, 353, 95, 60, THREEHEAD_COLOR);
				else if (animIndex < 10)
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 413, 95, 60, THREEHEAD_COLOR);
				else if (animIndex < 15)	  
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 473, 95, 60, THREEHEAD_COLOR);
				else						   
					GdiTransparentBlt(_hdc, x, y, 190, 130, drawingDC, 3 + (animIndex % 5) * 95, 533, 95, 60, THREEHEAD_COLOR);

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
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 3, 850, 100, 140, THREEHEAD_COLOR);
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 40 * (animIndex % 2) + 110, 1765, 40, 50, THREEHEAD_COLOR);
			break;
		case THREEHEAD::MID:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 103, 850, 100, 140, THREEHEAD_COLOR);
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 40 * (animIndex % 2) + 110, 1765, 40, 50, THREEHEAD_COLOR);
			break;
		case THREEHEAD::RIGHT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 203, 850, 100, 140, THREEHEAD_COLOR);
			stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
			StretchBlt(stretchDC, 0, 0, 40, 50, drawingDC, (animIndex % 2) * 40 + 40 + 110, 1765, -40, 50, SRCCOPY);
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, stretchDC, 0, 0, 40, 50, THREEHEAD_COLOR);
			break;
		}

		Anim_Counter(12, 50.f, false);
		if (animIndex < 7)
		{
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.6f, info.y + scrollY - info.cy * 0.8f, 140, 140, drawingDC, 3 + animIndex * 70, 1021, 70, 70, THREEHEAD_COLOR);
		}
		else
		{
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.6f, info.y + scrollY - info.cy * 0.8f, 140, 140, drawingDC, 3 + (animIndex % 7) * 70, 1091, 70, 70, THREEHEAD_COLOR);
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
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 3, 850, 100, 140, THREEHEAD_COLOR);

			if (firing)
			{
				Anim_Counter(4,50.f,true);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, animIndex * 39, 1823, 40, 50, THREEHEAD_COLOR);

				GdiTransparentBlt(_hdc, info.x + scrollX - 50.f, info.y  + scrollY - 50.f,100,100,drawingDC,(effectIndex % 6) * 65, 1986 + 50 * int(effectIndex/5),65,50, THREEHEAD_COLOR);
				if (fireEffectTime + 30.f < GetTickCount())
				{
					++effectIndex;
					if (effectIndex > 25)
						effectIndex = 0;
					fireEffectTime = GetTickCount();
				}
			}
			else
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 40 * (animIndex % 2) + 110, 1765, 40, 50, THREEHEAD_COLOR);
			}
			
			break;
		case THREEHEAD::MID:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 103, 850, 100, 140, THREEHEAD_COLOR);
			if (firing)
			{
				Anim_Counter(4, 50.f, true);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, animIndex * 39, 1823, 40, 50, THREEHEAD_COLOR);
				GdiTransparentBlt(_hdc, info.x + scrollX - 50.f, info.y + scrollY - 50.f, 100, 100, drawingDC, (effectIndex % 6) * 65, 1986 + 50 * int(effectIndex / 6), 65, 50, THREEHEAD_COLOR);
				if (fireEffectTime + 30.f < GetTickCount())
				{
					++effectIndex;
					if (effectIndex > 25)
						effectIndex = 0;
					fireEffectTime = GetTickCount();
				}
			}
			else
			{
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, drawingDC, 40 * (animIndex % 2) + 110, 1765, 40, 50, THREEHEAD_COLOR);
			}
			
			break;
		case THREEHEAD::RIGHT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 203, 850, 100, 140, THREEHEAD_COLOR);
			if (firing)
			{
				Anim_Counter(4, 50.f, true);
				stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
				StretchBlt(stretchDC, 0, 0, 40, 50, drawingDC, animIndex * 39  + 40 , 1823, -40, 50, SRCCOPY);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, stretchDC, 0, 0, 40, 50, THREEHEAD_COLOR);

				StretchBlt(stretchDC, 0, 0, 65, 50, drawingDC, (effectIndex % 6) * 65 + 65, 1986 + 50 * int(effectIndex / 6), -65, 50, SRCCOPY);
				GdiTransparentBlt(_hdc, info.x + scrollX - 50.f, info.y + scrollY - 50.f, 100, 100, stretchDC, 0, 0, 65, 50, THREEHEAD_COLOR);
				if (fireEffectTime + 30.f < GetTickCount())
				{
					++effectIndex;
					if (effectIndex > 25)
						effectIndex = 0;
					fireEffectTime = GetTickCount();
				}
			}
			else
			{
				stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
				StretchBlt(stretchDC, 0, 0, 40, 50, drawingDC, (animIndex % 2) * 40 + 40 + 110, 1765, -40, 50, SRCCOPY);
				GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.3f, info.y + scrollY - info.cy * 0.6f, 70, 100, stretchDC, 0, 0, 40, 50, THREEHEAD_COLOR);
			}
			break;
		}
		break;
	case THREEHEAD::DESTORY:
		switch (head)
		{
		case THREEHEAD::LEFT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 3, 2634, 100, 100, THREEHEAD_COLOR);

			break;
		case THREEHEAD::MID:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 103, 2634, 100, 100, THREEHEAD_COLOR);

			break;
		case THREEHEAD::RIGHT:
			GdiTransparentBlt(_hdc, info.x + scrollX - info.cx * 0.9f, info.y + scrollY - info.cy, 200, 280, drawingDC, 203, 2634, 100, 100, THREEHEAD_COLOR);
			break;
		}
		break;
	}
}

void Tower::Release()
{
	for (auto& i : bullets)
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::BULLET, i);

	for_each(bullets.begin(),bullets.end(),CDeleteObj());
	
	bullets.clear();
}

void Tower::State_Machine()
{
	switch (state)
	{
	case THREEHEAD::ATTACK:
		if (!isMove)
		{
			isMove = true;
			fireTime = GetTickCount();
		}

		if (isMove && fireTime + 2500.f < GetTickCount())
		{
			DIR::ID bulletDir;

			switch (head)
			{
			case THREEHEAD::LEFT:
			case THREEHEAD::MID:
				bulletDir = DIR::RIGHT;
				break;
			case THREEHEAD::RIGHT:
				bulletDir = DIR::LEFT;
				break;
			}
			for (int i = 0; i < 6; ++i)
			{
				Obj* bullet;
				if (bulletDir == DIR::RIGHT)
				{
					bullet = CAbstractFactory<ThreeHeadBullet>::Create(info.x + (i % 3) * 25.f, info.y - info.cy * 0.3f * (i % 2), bulletDir);
				}
				else
				{
					bullet = CAbstractFactory<ThreeHeadBullet>::Create(info.x - (i % 3) * 25.f, info.y - info.cy * 0.3f * (i % 2), bulletDir);
				}
			
				static_cast<ThreeHeadBullet*>(bullet)->Set_ParentID(OBJ::ENEMY);
				if(!ObjPoolMgr::Get_Instance()->Get_OnScreenObj(OBJ::PLAYER).empty())
					static_cast<ThreeHeadBullet*>(bullet)->Set_Target(ObjPoolMgr::Get_Instance()->Get_OnScreenObj(OBJ::PLAYER).front());

				ObjPoolMgr::Get_Instance()->Add_Object(OBJ::BULLET,bullet);
				bullets.push_back(bullet);
			}

			fireTime = GetTickCount();
			fireEffectTime = GetTickCount();
			effectIndex = 0;
			firing = true;
		}
		else if (fireTime + 1000.f < GetTickCount())
		{
			firing = false;
		}
		break;
	}
}
