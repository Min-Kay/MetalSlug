#include "Kessi.h"
#include "Manager.h"
#include "FixedBazuka.h"
#include "Burst.h"
#include "Rosin.h"

Kessi::Kessi()
{
	for (int i = 0; i < 2; ++i)
	{
		bazuka[i] = nullptr;
		burst[i] = nullptr;
	}
}

Kessi::~Kessi()
{
	Release();
}

void Kessi::Initialize()
{
	id = OBJ::ENEMY;
	enemy_id = ENEMY::KESSI;

	hp = 1500;

	info.cx = 600.f;
	info.cy = 200.f;

	dir = DIR::LEFT;

	state = STATE::IDLE;

	rosinTime = GetTickCount();
	totalMove = 0.f;
	rosinCount = 0;
	maxCount = 10;

	burstGauge = 0;
	rosinGauge = 0;

	angle = 0;

	speed = 15.f;

	init_x = info.x;
	init_y = info.y + 300.f;

	isMove = false;
	isDead = false;
	isDying = false;
	isInit = false;

	coll_Attack = false;
	canCollision = false;

	burstOn = false; 
	burstMove = 0.f; 
	if (!bazuka[0])
	{
		for (int i = 0; i < 2; ++i)
		{
			bazuka[i] = new FixedBazuka;
			bazuka[i]->Initialize();
			ObjPoolMgr::Get_Instance()->Add_Object(OBJ::ENEMY, bazuka[i]);
		}
	}

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Kessi.bmp",L"Kessi");
}

int Kessi::Update()
{
	if (isDead)
		return OBJ_DEAD;
	State_Machine();
	Update_Rect();
	return OBJ_DEFAULT;
}

void Kessi::Late_Update()
{
	if (isDying || isDead)
		return;

	auto iter = props.begin();
	for (; iter != props.end();)
	{
		if ((*iter)->Get_Dead())
		{
			ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, *iter);
			SAFE_DELETE(*iter);
			iter = props.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		bazuka[i]->Set_Pos(info.x - 100 + (i * 200.f), info.y - 50.f);
		if(burst[i])
			burst[i]->Set_Pos(info.x - 200 + (i * 400.f), info.y + 150.f);
	}

	if (hp <= 0)
	{
		list_Release();
		isInit = false; 
		idleTime = GetTickCount();
		state = STATE::DESTROY;
		animIndex = 0;
		isDying = true;
	}
}

void Kessi::Render(HDC _hdc)
{
	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Kessi");

	//Rectangle(_hdc, rect.left + x, rect.top + y, rect.right + x, rect.bottom + y);
	switch (state)
	{
	case Kessi::IDLE:
		GdiTransparentBlt(_hdc,rect.left + x, rect.top + y, info.cx, info.cy + 100.f, drawingDC,0,0,280,140,RGB(0,248,0));
		break;
	case Kessi::BURST:
		GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy + 100.f, drawingDC, 0, 0, 280, 140, RGB(0, 248, 0));
		GdiTransparentBlt(_hdc, rect.left + x + 15.f, rect.top + y + 135.f, 150, 80, drawingDC, 0, 765, 70, 35, RGB(0, 248, 0));
		GdiTransparentBlt(_hdc, rect.left + x + 430.f, rect.top + y + 140.f, 150, 80, drawingDC, 0, 835, 70, 35, RGB(0, 248, 0));
		break;
	case Kessi::ROSIN:
		GdiTransparentBlt(_hdc, rect.left + x, rect.top + y, info.cx, info.cy + 100.f, drawingDC, 0, 0, 280, 140, RGB(0, 248, 0));
		GdiTransparentBlt(_hdc, rect.left + x + 5.f, rect.top + y + 135.f, 150, 80, drawingDC, 260, 765, 70, 35, RGB(0, 248, 0));
		GdiTransparentBlt(_hdc, rect.left + x + 420.f, rect.top + y + 140.f, 150, 80, drawingDC, 260, 835, 70, 35, RGB(0, 248, 0));
		break;
	case Kessi::DESTROY:
		GdiTransparentBlt(_hdc, rect.left + x , rect.top + y, info.cx, info.cy + 100.f, drawingDC, 280, 0, 280, 140, RGB(0, 248, 0));
		if (idleTime + 3000.f < GetTickCount())
		{
			isDead = true; 
		}
		break;
	}
}

void Kessi::Release()
{
	list_Release();
}

void Kessi::State_Machine()
{

	float x = CScrollMgr::Get_Instance()->Get_ScrollX(); 
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	switch (state)
	{
	case Kessi::IDLE:
		if (!isMove)
		{
			if (CScrollMgr::Get_Instance()->Get_ScrollLockX() <= WINCX - x)
				isMove = true;
			return;
		}

		if (!isInit)
		{
			if(info.y < init_y)
				info.y += speed * 0.3f;
			else if(info.y > init_y)
				info.y -= speed * 0.3f;

			if (info.x < init_x)
				info.x += speed * 0.3f;
			else if (info.x > init_x)
				info.x -= speed * 0.3f;

			if (abs(init_y - info.y) < 50.f && abs(init_x - info.x) < 50.f)
			{
				isInit = true;
			}
			return;
		}

		info.x = init_x + speed * cosf(angle);
		info.y = init_y + speed * sinf(angle);

		if (idleTime + 30.f < GetTickCount())
		{
			idleTime = GetTickCount();
			angle += 0.3f;
		}

		rosinGauge += rand() % 40;
		burstGauge += rand() % 40;

		if (rosinGauge > 10000)
		{
			state = STATE::ROSIN;
			rosinGauge = 0;
			burstGauge >> 3;
		}
		else if (burstGauge > 10000)
		{
			state = STATE::BURST;
			rosinGauge >> 3;
			burstGauge = 0;
		}
		break;
	case Kessi::BURST:

		if (!burst[0])
		{
			for (int i = 0; i < 2; ++i)
			{
				burst[i] = new Burst;
				burst[i]->Initialize();
				ObjPoolMgr::Get_Instance()->Add_Object(OBJ::ENEMY, burst[i]);
			}
		}

		if (!burstOn)
		{
			for (int i = 0; i < 2; ++i)
			{
				static_cast<Burst*>(burst[i])->Set_Active();
			}
			burstOn = true;

			info.x = init_x;
			info.y = init_y; 
		}

		if (static_cast<Burst*>(burst[0])->Get_Spawning())
		{
			info.x += burstSpeed;
			
			if (info.x > WINCX-x ||
				info.x < -x )
			{
				burstSpeed *= -1.f;
			}
		}

		if (!static_cast<Burst*>(burst[0])->Get_Active())
		{
			burstMove = 0.f;
			isInit = false;
			state = STATE::IDLE;
		}

		break;
	case Kessi::ROSIN:
		info.x = init_x + speed * cosf(angle);
		info.y = init_y + speed * sinf(angle);

		if (idleTime + 30.f < GetTickCount())
		{
			idleTime = GetTickCount();
			angle += 0.3f;
		}

		if (rosinTime + 1500.f < GetTickCount())
		{
			Obj* rosin = new Rosin;
			rosin->Set_Pos(rand() % WINCX - x, y);
			rosin->Initialize();
			props.push_back(rosin);
			ObjPoolMgr::Get_Instance()->Add_Object(OBJ::ENEMY,rosin);
			++rosinCount;
			rosinTime = GetTickCount();

			if (rosinCount > maxCount)
			{
				rosinCount = 0;
				state = STATE::IDLE;
				isInit = false; 
			}
		}
		break;
	case Kessi::DESTROY:
		if (!isInit)
		{
			info.x = init_x;
			info.y = init_y;
			isInit = true;
		}
		
		info.y += speed * 0.05f; 
	
		break;
	}
}

void Kessi::list_Release()
{
	for (int i = 0; i < 2; ++i)
	{
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, burst[i]);
		SAFE_DELETE(burst[i]);
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, bazuka[i]);
		SAFE_DELETE(bazuka[i]);
	}

	for(auto & i : props)
	{
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY,i);
	}

	for_each(props.begin(),props.end(),CDeleteObj());

}
