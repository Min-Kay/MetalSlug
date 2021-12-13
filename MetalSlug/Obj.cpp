#include "Obj.h"
#include "Manager.h"

Obj::Obj()
{
	drawingDC = NULL;
	stretchDC = NULL;

	animIndex = 0;
	animTimer = GetTickCount();

	dir = DIR::RIGHT;
	id = OBJ::PLAYER;
	isDead = false;
	isDying = false;

	hp = 0;
}

void Obj::Gravity()
{
	if (isDead)
		return;

	float fY = 0.f;
	float blockY = 0.f;
	float jumpingState = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x, info.y, &fY);
	bool blockCol = BlockMgr::Get_Instance()->Collision_Block(this, &blockY);
	
	if (blockCol && info.y > blockY)
	{
		info.y = blockY + 1;
	}
	else if (lineCol && info.y < fY - info.cy * 0.6f)
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

void Obj::Check_Line_Collision()
{
	float fY = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x, info.y, &fY);

	if (lineCol && fY <= info.y)
		isDead = true;
}

void Obj::Update_Rect()
{
	rect.top = info.y - info.cy * 0.5f;
	rect.bottom = info.y + info.cy * 0.5f;
	rect.left = info.x - info.cx * 0.5f;
	rect.right = info.x + info.cx * 0.5f;
}

void Obj::Anim_Counter(int count, float _timer, bool _roop, int start)
{
	if (animIndex < start || animIndex > start + count)
		animIndex = start;

	if (animTimer + _timer < GetTickCount())
	{
		if (_roop && animIndex >= start + count)
		{
			animIndex = start;
		}
		else  if (animIndex < start + count)
			++animIndex;

		animTimer = GetTickCount();
	}
}

void Obj::Check_Screen_Out()
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	if (-scrollX > info.x ||
		WINCX - scrollX < info.x ||
		scrollY > info.y ||
		WINCY - scrollY < info.y)
		isDead = true;
}
