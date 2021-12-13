#include "Npc.h"
#include "Manager.h"
#include "Items.h"
#include "Bullets.h"

void Npc::Initialize()
{
	id = OBJ::NPC;
	action = ACTION::IDLE;
	animIndex = 0;

	info.cx = 100.f; 
	info.cy = 100.f;

	isUntied = false;
	isRopped = true;

	ranout = false;
	moving = false;
	thank = false;

	spawningItem = false;
	spawnTiming = false;
	spawned = false;
	speed = 2.f;
	totalMove = 0.f;
	isMotion = false;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC_Right.bmp", L"NPC");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_White3.bmp", L"Stretch_White3");

}

int Npc::Update()
{
	if (isDead)
		return OBJ_DEAD;
	StateMachine();
	Gravity();
	Update_Rect();

	return OBJ_DEFAULT;
}

void Npc::Late_Update()
{
	if(!isRopped)
		Check_Screen_Out();
}

void Npc::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	//Rectangle(_hdc,rect.left + scrollX,rect.top + scrollY,rect.right + scrollX,rect.bottom + scrollY);

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"NPC");
	switch (action)
	{
	case ACTION::IDLE:
		if (isRopped)
		{
			Anim_Counter(5,50.f);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx * 1.5f, int(rect.top + scrollY) - info.cy * 1.2f, info.cx * 4.f, info.cy * 4.f, drawingDC, animIndex * 200 , 0, 200, 200, RGB(255, 255, 255));
		}
		else if (thank)
		{
			if (dir == DIR::RIGHT)
			{
				Anim_Counter(10, 50.f, false, 4);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx * 1.5f, int(rect.top + scrollY) - info.cy * 1.2f, info.cx * 4.f, info.cy * 4.f, drawingDC, animIndex * 200, 200, 200, 200, RGB(255, 255, 255));
			}
			else
			{
				Anim_Counter(10, 50.f, false, 4);
				stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White3");
				StretchBlt(stretchDC, 0, 0, 200, 200, drawingDC, animIndex * 200 - 200, 200, -200, 200, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx * 1.5f, int(rect.top + scrollY) - info.cy * 1.2f, info.cx * 4.f, info.cy * 4.f, stretchDC, 0, 0, 200, 200, RGB(255, 255, 255));
			}


			if (animIndex == 14 && !isMotion)
			{
				isMotion = true;
				motionDelay = GetTickCount();
			}
			else if (isMotion && motionDelay + 700.f < GetTickCount())
			{
				animIndex = 0;
				action = ACTION::MOVE;
				ranout = true;
				dir = dir == DIR::RIGHT ? DIR::LEFT : DIR::RIGHT;
				isMotion = false;
			}
		}
		else if (spawningItem)
		{
			if (dir == DIR::RIGHT)
			{
				Anim_Counter(10, 50.f, false);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx * 1.5f, int(rect.top + scrollY) - info.cy * 1.2f, info.cx * 4.f, info.cy * 4.f, drawingDC, animIndex * 200, 800, 200, 200, RGB(255, 255, 255));
			}
			else
			{
				Anim_Counter(10, 50.f, false);
				stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White3");
				StretchBlt(stretchDC, 0, 0, 200, 200, drawingDC, animIndex * 200 - 200, 800, -200, 200, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx * 1.5f, int(rect.top + scrollY) - info.cy * 1.2f, info.cx * 4.f, info.cy * 4.f, stretchDC, 0, 0, 200, 200, RGB(255, 255, 255));
			}

			if (animIndex == 9 && !spawned)
			{
				spawnTiming = true;
			}

			if (animIndex == 10 && !isMotion)
			{
				isMotion = true;
				motionDelay = GetTickCount();
			}
			else if (isMotion && motionDelay + 700.f < GetTickCount())
			{
				animIndex = 0;
				thank = true;
				isMotion = false;
			}
		}
		else if (isUntied)
		{
			Anim_Counter(5, 50.f, false);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx * 1.5f, int(rect.top + scrollY) - info.cy * 1.2f, info.cx * 4.f, info.cy * 4.f, drawingDC, animIndex * 200, 200, 200, 200, RGB(255, 255, 255));

			if (animIndex == 5)
			{
				action = ACTION::MOVE;
				animIndex = 0;
				moving = true;
			}
		}
		break;
	case ACTION::MOVE:
		if (ranout)
		{
			if (dir == DIR::RIGHT)
			{
				Anim_Counter(7, 50.f);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx * 1.5f, int(rect.top + scrollY) - info.cy * 1.2f, info.cx * 4.f, info.cy * 4.f, drawingDC, animIndex * 200, 1200, 200, 200, RGB(255, 255, 255));
			}
			else
			{
				Anim_Counter(7, 50.f);
				stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White3");
				StretchBlt(stretchDC, 0, 0, 200, 200, drawingDC, animIndex * 200 - 200, 1200, -200, 200, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx * 1.5f, int(rect.top + scrollY) - info.cy * 1.2f, info.cx * 4.f, info.cy * 4.f, stretchDC, 0, 0, 200, 200, RGB(255, 255, 255));
			}
		}
		else if (moving)
		{
			if (dir == DIR::RIGHT)
			{
				Anim_Counter(12, 50.f);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx * 1.5f, int(rect.top + scrollY) - info.cy * 1.2f, info.cx * 4.f, info.cy * 4.f, drawingDC, animIndex * 200, 400, 200, 200, RGB(255, 255, 255));
			}
			else
			{
				Anim_Counter(12, 50.f);
				stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_White3");
				StretchBlt(stretchDC, 0, 0,200, 200, drawingDC, animIndex * 200 - 200, 400, -200, 200, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx * 1.5f, int(rect.top + scrollY) - info.cy * 1.2f, info.cx * 4.f, info.cy * 4.f, stretchDC, 0,0, 200, 200, RGB(255, 255, 255));
			}
		}
		break;
	}
}

void Npc::Release()
{
}

void Npc::StateMachine()
{
	switch (action)
	{
	case ACTION::IDLE:
		if (spawnTiming) 
		{
			DataMgr::Get_Instance()->Add_Score(100);
			ITEM::ID box[ITEM::END - 1] = { ITEM::AMMO,ITEM::GRENADE,ITEM::LIFE,ITEM::WEAPON };
			int pos = rand() % ITEM::END - 1;
			if (pos == -1)
				pos = ITEM::WEAPON;
			ITEM::ID item = box[pos];
			int wep = rand() % WEAPON::IRONLIZARD + 1;
			if (wep == WEAPON::END)
				wep = WEAPON::IRONLIZARD;
			
			if (dir == DIR::RIGHT)
				ObjPoolMgr::Get_Instance()->Spawn_Item(item, info.x + 50.f, info.y, (WEAPON::ID)wep);
			else
				ObjPoolMgr::Get_Instance()->Spawn_Item(item, info.x - 50.f, info.y, (WEAPON::ID)wep);

			spawnTiming = false;
			spawned = true;
		}
		break;
	case ACTION::MOVE:
		if (ranout)
		{
			if (dir == DIR::RIGHT)
			{
				info.x += speed * 3.f;
			}
			else
			{
				info.x -= speed * 3.f;
			}
		}
		else if (moving)
		{
			if (totalMove > 300)
			{
				totalMove = 0.f;
				dir = dir == DIR::RIGHT ? DIR::LEFT : DIR::RIGHT; 
			}

			if (dir == DIR::RIGHT)
			{
				info.x += speed;
				totalMove += speed;
			}
			else
			{
				info.x -= speed;
				totalMove += speed;
			}
		}
		break;
	}
}

void Npc::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (spawningItem)
		return;

	if (_id == OBJ::PLAYER && isUntied)
	{
		dir = ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x > 0 ? DIR::RIGHT : DIR::LEFT;
		action = ACTION::IDLE;
		spawningItem = true;
	}
}

void Npc::Set_Ropped()
{
	if (!isRopped)
		return;

	action = ACTION::IDLE;
	isRopped = false;
	isUntied = true;
}
