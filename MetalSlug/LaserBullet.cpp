#include "framework.h"
#include "LaserBullet.h"
#include "Manager.h"
#include "Npc.h"

LaserBullet::LaserBullet()
{
}
LaserBullet::~LaserBullet()
{
	Release();
}
void LaserBullet::Initialize()
{
	id = OBJ::BULLET;
	parentID = OBJ::PLAYER;
	isDead = false;
	
	damage = 10;
	info.cx = 15.f;
	info.cy = 15.f;
	maxSize = 700.f;
	renderSize = -1.f; 
	isFiring = false;
	listClear = false;
	animIndex = 0;
	animTimer = GetTickCount();
	formalDir = DIR::RIGHT;
	diffDir = false; 

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Laser_Head.bmp", L"Laser_Head");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Laser_Head_Up.bmp", L"Laser_Head_Up");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Laser_Head_Down.bmp", L"Laser_Head_Down");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Laser_Body.bmp", L"Laser_Body");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Laser_Body_Up.bmp", L"Laser_Body_Up");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Laser_Body_Down.bmp", L"Laser_Body_Down");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Laser_Hit.bmp", L"Laser_Hit");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Laser_Hit_Up.bmp", L"Laser_Hit_Up");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Laser_Hit_Down.bmp", L"Laser_Hit_Down");

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp",L"Stretch_Item");

}

int LaserBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (!isFiring)
	{
		if (!listClear)
		{
			listClear = true; 
			tick.erase(tick.begin(), tick.end());
			tick.clear();
		}
		renderSize = -1.f; 
		return OBJ_DEFAULT;
	}

	Check_Tick();
	Check_Distance();
	Update_Rect();
	return OBJ_DEFAULT;
}

void LaserBullet::Late_Update()
{
	if (formalDir != dir)
	{
		diffDir = true; 
		renderSize = -1.f;
		formalDir = dir;

		tick.erase(tick.begin(),tick.end());
		tick.clear();
	}
}

void LaserBullet::Render(HDC _hdc)
{
	if (!isFiring || diffDir)
	{
		diffDir = false;
		return;
	}

	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	//Rectangle(_hdc, rect.left + x, rect.top + y, rect.right + x, rect.bottom + y);
	Anim_Counter(2, 50.f);
	if (renderSize == -1.f)
	{
		switch (dir)
		{
		case DIR::UP:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Body_Up");
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y,info.cx,maxSize,drawingDC,0, animIndex * 15,15,15,RGB(0,248,0));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Hit_Up");
			GdiTransparentBlt(_hdc, rect.left + x - 30.f, rect.top + 40.f + y, 72, 80, drawingDC, 0, animIndex * 40, 36, 40, RGB(192, 192, 192));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Head_Up");
			GdiTransparentBlt(_hdc, rect.left - 7.5f + x , rect.bottom - 30.f + y, 30, 30, drawingDC, 0, animIndex * 25, 25, 25, RGB(0, 248, 0));
			break;
		case DIR::DOWN:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Body_Down");
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, maxSize, drawingDC,0, animIndex * 15, 15, 15, RGB(0, 248, 0));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Hit_Down");
			GdiTransparentBlt(_hdc, rect.left + x - 30.f, rect.bottom - 40.f + y, 72, 80, drawingDC, 0, animIndex * 40, 36, 40, RGB(192, 192, 192));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Head_Down");
			GdiTransparentBlt(_hdc, rect.left - 7.5f + x, rect.top + y, 30, 30, drawingDC, 0, animIndex * 25, 25, 25, RGB(0, 248, 0));
			break;
		case DIR::LEFT:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Body");
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, maxSize, info.cy, drawingDC, animIndex * 15, 0, 15, 15, RGB(0, 248, 0));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Hit");
			stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
			StretchBlt(stretchDC, 0, 0, 40, 36, drawingDC, animIndex * 40 + 40, 0, -40, 36, SRCCOPY);
			GdiTransparentBlt(_hdc, rect.left - 40.f + x, rect.top - 30.f + y, 80, 72, stretchDC, 0, 0, 40, 36, RGB(192, 192, 192));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Head");
			StretchBlt(stretchDC, 0, 0, 30, 30, drawingDC, animIndex * 25 + 25, 0, -25, 25, SRCCOPY);
			GdiTransparentBlt(_hdc, rect.right + x - 30.f, rect.top - 7.5f + y, 30, 30, stretchDC, 0, 0, 30, 30, RGB(0, 248, 0));
			break;
		case DIR::RIGHT:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Body");
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, maxSize, info.cy, drawingDC, animIndex * 15, 0, 15, 15, RGB(0, 248, 0));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Hit");
			GdiTransparentBlt(_hdc, rect.right - 40.f + x, rect.top - 30.f + y, 80, 72, drawingDC, animIndex * 40, 0, 40, 36, RGB(192, 192, 192));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Head");
			GdiTransparentBlt(_hdc, rect.left + x, rect.top - 7.5f + y, 30, 30, drawingDC, animIndex * 25,0, 25, 25, RGB(0, 248, 0));
			break;
		}
	}
	else
	{
		switch (dir)
		{
		case DIR::UP:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Body_Up");
			GdiTransparentBlt(_hdc, rect.left +x, info.y - renderSize + y, info.cx, renderSize, drawingDC, 0, animIndex * 15, 15, 15, RGB(0, 248, 0));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Hit_Up");
			GdiTransparentBlt(_hdc, rect.left + x - 30.f, info.y - renderSize - 40.f + y, 72, 80, drawingDC, 0, animIndex * 40, 36, 40, RGB(192, 192, 192));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Head_Up");
			GdiTransparentBlt(_hdc, rect.left - 7.5f + x , rect.bottom - 30.f + y, 30, 30, drawingDC, 0, animIndex * 25, 25, 25, RGB(0, 248, 0));
			break;
		case DIR::DOWN:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Body_Down");
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, renderSize, drawingDC, 0, animIndex * 15, 15, 15, RGB(0, 248, 0));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Hit_Down");
			GdiTransparentBlt(_hdc, rect.left + x - 30.f, info.y + renderSize - 40.f + y, 72, 80, drawingDC, 0, animIndex * 40, 36, 40, RGB(192, 192, 192));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Head_Down");
			GdiTransparentBlt(_hdc, rect.left - 7.5f + x , rect.top + y, 30, 30, drawingDC, 0, animIndex * 25, 25, 25, RGB(0, 248, 0));
			break;
		case DIR::LEFT:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Body");
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, renderSize, info.cy, drawingDC, animIndex * 15, 0, 15, 15, RGB(0, 248, 0));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Hit");
			stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
			StretchBlt(stretchDC, 0, 0, 40, 36, drawingDC, animIndex * 40 + 40, 0, -40, 36, SRCCOPY);
			GdiTransparentBlt(_hdc, info.x - renderSize - 40.f + x, rect.top - 30.f + y, 80, 72, stretchDC, 0, 0, 40, 36, RGB(192, 192, 192));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Head");
			StretchBlt(stretchDC, 0, 0, 30, 30, drawingDC, animIndex * 25 + 25, 0, -25, 25, SRCCOPY);
			GdiTransparentBlt(_hdc, rect.right + x - 30.f, rect.top - 7.5f + y, 30, 30, stretchDC, 0, 0, 30, 30, RGB(0, 248, 0));
			break;
		case DIR::RIGHT:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Body");
			GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, renderSize, info.cy, drawingDC, animIndex * 15, 0, 15, 15, RGB(0, 248, 0));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Hit");
			GdiTransparentBlt(_hdc, info.x + renderSize - 40.f + x, rect.top - 30.f + y, 80, 72, drawingDC, animIndex * 40, 0, 40, 36, RGB(192, 192, 192));
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Laser_Head");
			GdiTransparentBlt(_hdc, rect.left + x, rect.top - 7.5f + y, 30, 30, drawingDC, animIndex * 25, 0, 25, 25, RGB(0, 248, 0));
			break;
		}
	}


}

void LaserBullet::Release()
{
	tick.erase(tick.begin(),tick.end());
	tick.clear();
}

void LaserBullet::Check_Distance()
{
	if (tick.empty())
	{
		renderSize = -1.f;
		return;
	}

	float min = -1.f;
	for (auto& i : tick)
	{
		if (min < 0 || (min > i.distance && i.collTime + 100.f > GetTickCount()))
		{
			min = i.distance;
		}
	}
	
	renderSize = min;
}

void LaserBullet::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (!isFiring || isDead)
		return;

	float distance = 0.f;
	switch (_id)
	{
	case OBJ::BLOCK:
	{
		if (_opponent->Get_Dying())
			break;

		switch (dir)
		{
		case DIR::UP:
			distance = info.y - _opponent->Get_Rect().bottom;
			break;
		case DIR::DOWN:
			distance = _opponent->Get_Rect().top - info.y;
			break;
		case DIR::LEFT:
			distance = info.x - _opponent->Get_Rect().right;
			break;
		case DIR::RIGHT:
			distance = _opponent->Get_Rect().left - info.x;
			break;
		}

		for (auto& i : tick)
		{
			if (i.target == _opponent)
			{
				i.distance = distance;
				i.collTime = GetTickCount();

				if(i.distance > renderSize)
					return;

				if (i.dealTime + LASER_TICK < i.collTime)
				{
					i.dealTime = i.collTime;
					i.target->Add_HP(-damage);
				}
				return;
			}
		}

		if (renderSize != -1.f && renderSize < distance)
			return;

		Tick temp;
		temp.target = _opponent;
		temp.dealTime = GetTickCount();
		temp.collTime = temp.dealTime;
		temp.distance = distance; 
		_opponent->Add_HP(-damage);
		tick.push_back(temp);

	}
		break;
	case OBJ::ENEMY:
	{
		if (_opponent->Get_Dying())
			break;

		switch (dir)
		{
		case DIR::UP:
			distance = info.y - _opponent->Get_Rect().bottom;
			break;
		case DIR::DOWN:
			distance = _opponent->Get_Rect().top - info.y;
			break;
		case DIR::LEFT:
			distance = info.x - _opponent->Get_Rect().right;
			break;
		case DIR::RIGHT:
			distance = _opponent->Get_Rect().left - info.x;
			break;
		}

		for (auto& i : tick)
		{
			if (i.target == _opponent)
			{
				i.distance = distance;
				i.collTime = GetTickCount();
				if (i.distance > renderSize)
					return;

				if (i.dealTime + LASER_TICK < i.collTime)
				{
					i.dealTime = i.collTime;
					i.target->Add_HP(-damage);
				}
				return;
			}
		}

		if (renderSize != -1.f && renderSize < distance)
			return;

		Tick temp;
		temp.target = _opponent;
		temp.dealTime = GetTickCount();
		temp.collTime = temp.dealTime;
		temp.distance = distance;
		_opponent->Add_HP(-damage);
		tick.push_back(temp);
	}
		break;
	case OBJ::NPC:
		if (!static_cast<Npc*>(_opponent)->Get_Ropped())
			return;

		switch (dir)
		{
		case DIR::UP:
			distance = info.y - _opponent->Get_Rect().bottom;
			break;
		case DIR::DOWN:
			distance = _opponent->Get_Rect().top - info.y;
			break;
		case DIR::LEFT:
			distance = info.x - _opponent->Get_Rect().right;
			break;
		case DIR::RIGHT:
			distance = _opponent->Get_Rect().left - info.x;
			break;
		}

		if (renderSize != -1.f && renderSize < distance)
			return;

		static_cast<Npc*>(_opponent)->Set_Ropped();
		break;
	}
}

void LaserBullet::Update_Rect()
{
	switch (dir)
	{
	case DIR::UP:
		rect.left = info.x - info.cx * 0.5f;
		rect.right = info.x + info.cx * 0.5f;
		rect.top = info.y - maxSize;
		rect.bottom = info.y;
		break;
	case DIR::DOWN:
		rect.left = info.x - info.cx * 0.5f;
		rect.right = info.x + info.cx * 0.5f;
		rect.top = info.y ;
		rect.bottom = info.y + maxSize;
		break;
	case DIR::LEFT:
		rect.left = info.x - maxSize;
		rect.right = info.x;
		rect.top = info.y - info.cy * 0.5f;
		rect.bottom = info.y + info.cy * 0.5f;
		break;
	case DIR::RIGHT:
		rect.left = info.x ;
		rect.right = info.x + maxSize;
		rect.top = info.y - info.cy * 0.5f;
		rect.bottom = info.y + info.cy * 0.5f;
		break;
	}
}

void LaserBullet::Pressing(bool _bool)
{
	isFiring = _bool;
}

void LaserBullet::Check_Tick()
{
	if (tick.empty())
		return;

	auto iter = tick.begin();
	for (; iter != tick.end();)
	{
		if (iter->target->Get_Dying() || iter->collTime + LASER_TICK < GetTickCount())
		{
			if (iter->distance == renderSize)
				renderSize = -1.f;

			iter = tick.erase(iter);
		}
		else
			++iter;
	}
}

void LaserBullet::Reset()
{
	renderSize = -1.f;
}

