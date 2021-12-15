#include "Burst.h"
#include "Manager.h"

Burst::Burst()
{
	active = false;
	canCollision = false;
	totalCY = 0.f;
	init_CY = 0.f;
	maintainTime = GetTickCount();
	spawning = false;
	maintain = false;
}

Burst::~Burst()
{
}

void Burst::Initialize()
{
	coll_Attack = false; 
	canCollision = false;
	active = false;
	spawning = false;
	maintain = false; 
	isDead = false;
	isDying = false;

	info.cx = 80.f;
	info.cy = 50.f; 

	grownTime = GetTickCount();
	maintainTime = GetTickCount();

	init_CY = 50.f;
	totalCY = 0.f;
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Kessi.bmp", L"Kessi");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp", L"Stretch_Item");


}

int Burst::Update()
{
	if (isDead)
		return OBJ_DEAD;

	Update_Rect();
	return OBJ_DEFAULT;
}

void Burst::Late_Update()
{
}

void Burst::Render(HDC _hdc)
{
	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();
	//Rectangle(_hdc, rect.left + x, rect.top + y, rect.right + x, rect.bottom + y);
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Kessi");
	stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");

	switch (dir)
	{
	case DIR::LEFT:
		if (active)
		{

			if (!spawning)
			{
				coll_Attack = false;

				Anim_Counter(28, 100.f, false);
				if (animIndex < 4)
				{
					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 80, 100, drawingDC, animIndex * 40, 1725, 40, 50, RGB(0, 248, 0));
				}
				else if (animIndex < 13)
				{
					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, drawingDC, (animIndex % 9) * 50, 1775, 50, 165, RGB(0, 248, 0));
				}
				else if (animIndex < 25)
				{
					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, drawingDC, (animIndex % 12) * 50, 1940, 50, 165, RGB(0, 248, 0));
				}
				else
				{
					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, drawingDC, (animIndex % 4) * 50, 2105, 50, 165, RGB(0, 248, 0));
				}

				if (grownTime + 50.f < GetTickCount() && totalCY < 300.f)
				{
					totalCY += 10.f;
					grownTime = GetTickCount();
				}

				if (animIndex == 28)
				{
					animIndex = 0;
					spawning = true;
					maintainTime = GetTickCount();
				}
			}
			else if (!maintain)
			{
				coll_Attack = true;

				Anim_Counter(5, 100.f, true, 0);

				GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, drawingDC, animIndex * 50, 2260, 50, 165, RGB(0, 248, 0));
				GdiTransparentBlt(_hdc, rect.left - 26.f + x, rect.bottom - 60.f + y, 132, 60, drawingDC, animIndex * 66, 2815, 66, 30, RGB(0, 248, 0));


				if (maintainTime + 15000.f < GetTickCount())
				{
					animIndex = 0;
					maintain = true;
				}
			}
			else
			{
				coll_Attack = false;

				Anim_Counter(5, 100.f, false);

				if (animIndex < 3)
				{
					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, drawingDC, animIndex * 50, 2425, 50, 165, RGB(0, 248, 0));
				}
				else
				{
					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, drawingDC, animIndex * 60, 2590, 60, 165, RGB(0, 248, 0));
				}

				if (grownTime + 20.f < GetTickCount() && totalCY > init_CY)
				{
					totalCY -= 50.f;
					grownTime = GetTickCount();
				}

				if (animIndex == 5)
				{
					spawning = false;
					maintain = false;
					active = false;
				}
			}
		}
		break;
	case DIR::RIGHT:
	

		if (active)
		{

			if (!spawning)
			{
				coll_Attack = false;
				Anim_Counter(28, 100.f, false);
				if (animIndex < 4)
				{
					StretchBlt(stretchDC,0,0, 40, 50, drawingDC, animIndex * 40 + 40, 1725, -40, 50,SRCCOPY);
					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 80, 100, stretchDC, 0, 0, 40, 50, RGB(0, 248, 0));
				}
				else if (animIndex < 13)
				{
					StretchBlt(stretchDC, 0, 0, 50, 165, drawingDC,(animIndex % 9) * 50 + 50, 1775, -50, 165, SRCCOPY);

					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, stretchDC, 0, 0, 50, 165, RGB(0, 248, 0));
				}
				else if (animIndex < 25)
				{
					StretchBlt(stretchDC, 0, 0, 50, 165, drawingDC, (animIndex % 12) * 50 + 50, 1940, -50, 165, SRCCOPY);

					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, stretchDC, 0, 0, 50, 165, RGB(0, 248, 0));
				}
				else
				{
					StretchBlt(stretchDC, 0, 0, 50, 165, drawingDC, (animIndex % 4) * 50 + 50, 2105, -50, 165, SRCCOPY);

					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, stretchDC, 0, 0, 50, 165, RGB(0, 248, 0));
				}

				if (grownTime + 50.f < GetTickCount() && totalCY < 300.f)
				{
					totalCY += 10.f;
					grownTime = GetTickCount();
				}

				if (animIndex == 28)
				{
					animIndex = 0;
					spawning = true;
					maintainTime = GetTickCount();
				}
			}
			else if (!maintain)
			{
				coll_Attack = true;

				Anim_Counter(5, 100.f, true, 0);

				StretchBlt(stretchDC, 0, 0, 50, 165, drawingDC, animIndex * 50 + 50, 2260, -50, 165, SRCCOPY);
				GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, stretchDC, 0, 0, 50, 165, RGB(0, 248, 0));
				GdiTransparentBlt(_hdc, rect.left - 26.f + x, rect.bottom - 60.f + y, 132, 60, drawingDC, animIndex * 66, 2815, 66, 30, RGB(0, 248, 0));


				if (maintainTime + 15000.f < GetTickCount())
				{
					animIndex = 0;
					maintain = true;
				}
			}
			else
			{
				coll_Attack = false;

				Anim_Counter(5, 100.f, false);

				if (animIndex < 3)
				{
					StretchBlt(stretchDC, 0, 0, 50, 165, drawingDC, animIndex * 50 + 50, 2425, -50, 165, SRCCOPY);

					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, stretchDC, 0, 0, 50, 165, RGB(0, 248, 0));
				}
				else
				{
					StretchBlt(stretchDC, 0, 0, 50, 165, drawingDC, animIndex * 60 + 60, 2590, -60, 165, SRCCOPY);

					GdiTransparentBlt(_hdc, rect.left + x - 10.f, rect.top + y, 100, 330, stretchDC, 0, 0, 50, 165, RGB(0, 248, 0));
				}

				if (grownTime + 20.f < GetTickCount() && totalCY > init_CY)
				{
					totalCY -= 50.f;
					grownTime = GetTickCount();
				}

				if (animIndex == 5)
				{
					spawning = false;
					maintain = false;
					active = false;
				}
			}
		}
		break;
	}

}

void Burst::Release()
{
}

void Burst::Set_Active()
{
	active = true; 

}

void Burst::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (_id == OBJ::PLAYER)
	{
		_opponent->Set_Dying();
	}
}

void Burst::Update_Rect()
{
	rect.left = info.x - info.cx * 0.5f;
	rect.right = info.x + info.cx * 0.5f;
	rect.top = info.y - info.cy * 0.5f;
	rect.bottom = info.y + info.cy * 0.5f + totalCY;
}
