#include "Burst.h"
#include "Manager.h"

Burst::Burst()
{
	active = false;

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
	id = OBJ::ENEMY;
	coll_Attack = true; 
	active = false;
	spawning = false;
	maintain = false; 

	info.cx = 100.f;
	info.cy = 50.f; 

	grownTime = GetTickCount();
	maintainTime = GetTickCount();

	init_CY = 50.f;
	totalCY = 400.f;
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Kessi.bmp", L"Kessi");

}

int Burst::Update()
{
	if (isDead)
		return OBJ_DEAD;

	return OBJ_DEFAULT;
}

void Burst::Late_Update()
{
}

void Burst::Render(HDC _hdc)
{
	if (active)
	{
		float x = CScrollMgr::Get_Instance()->Get_ScrollX();
		float y = CScrollMgr::Get_Instance()->Get_ScrollY();

		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Kessi");
		
		if (!spawning)
		{
			Anim_Counter(28,100.f,false);
			if (animIndex < 4)
			{

			}
			else if (animIndex < 13)
			{


			}
			else if (animIndex < 25)
			{

			}
			else
			{

			}

			if (grownTime + 200.f < GetTickCount() && info.cy < totalCY)
			{
				info.cy += 0.5f;
				grownTime = GetTickCount();
			}


			if (animIndex == 28)
			{
				animIndex = 0;
				spawning = true;
				maintainTime = GetTickCount();
			}
		}
		else if(!maintain) 
		{
			Anim_Counter(5, 100.f, true,0);
			

			if (maintainTime + 15000.f < GetTickCount())
			{
				animIndex = 0;
				maintain = true;
			}
		}
		else 
		{
			Anim_Counter(15,100.f,false);

			if (animIndex < 9)
			{
				
			}
			else
			{

			}

			if (grownTime + 200.f < GetTickCount() && info.cy > init_CY)
			{
				info.cy -= 0.5f;
				grownTime = GetTickCount();
			}

			if (animIndex == 16)
			{
				spawning = false;
				maintain = false; 
				active = false;
			}
		}
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