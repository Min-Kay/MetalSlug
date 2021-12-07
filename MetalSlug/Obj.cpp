#include "Obj.h"
#include "Manager.h"

void Obj::Gravity()
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
	}
	else
	{
		info.y += FALL_DOWN;
	}
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
	if (animIndex < start)
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
