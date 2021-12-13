#include "ThreeHead.h"
#include "Manager.h"
#include "Include.h"
#include "Tower.h"

ThreeHead::ThreeHead()
{
}

ThreeHead::~ThreeHead()
{
	Release();
}

void ThreeHead::Initialize()
{
	enemy_id = ENEMY::THREEHEAD;
	action = ACTION::IDLE;
	speed = 5.f;

	animIndex = 0;
	animTimer = GetTickCount();

	id = OBJ::ENEMY;
	isDead = false;

	isMove = false;
	coll_Attack = false;
	isDying = false;

	canCollision = false;

	state = THREEHEAD::NONE;

	towerOn = false; 
	doorOpening = false;

	totalY = 0.f;
		
	left = new Tower(THREEHEAD::LEFT, info.x + 300.f,info.y - 50.f);
	mid = new Tower( THREEHEAD::MID, info.x + 570.f, info.y - 50.f);
	right = new Tower(THREEHEAD::RIGHT, info.x + 850.f, info.y - 50.f);

	ObjPoolMgr::Get_Instance()->Add_Object(OBJ::ENEMY,left);
	ObjPoolMgr::Get_Instance()->Add_Object(OBJ::ENEMY, right);
	ObjPoolMgr::Get_Instance()->Add_Object(OBJ::ENEMY, mid);

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MidBoss.bmp",L"MidBoss");
	
}

int ThreeHead::Update()
{
	if (isDead)
		return OBJ_DEAD;

	State_Machine();

	return OBJ_DEFAULT;
}

void ThreeHead::Late_Update()
{
	Tower_On();

	if (!isDying && AllDestroied())
	{
		DataMgr::Get_Instance()->Add_Score(3000);
		isDying = true;
		state = THREEHEAD::DESTORY;
	}
}

void ThreeHead::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"MidBoss");
	switch (state)
	{
	case THREEHEAD::DESTORY:
			GdiTransparentBlt(_hdc, int(info.x - 50.f + scrollX), int(info.y - 50.f + scrollY), 1026, 324, drawingDC, 0, 3394, 380, 120, THREEHEAD_COLOR);
			Anim_Counter(9,30.f,false);
			if (animIndex < 3)
			{
				GdiTransparentBlt(_hdc, int(info.x + scrollX + 300), int(info.y  + scrollY - 100.f), 80 * animIndex, 60 * animIndex , drawingDC, (animIndex % 3) * 160, 2720, 160, 120, THREEHEAD_COLOR);
				GdiTransparentBlt(_hdc, int(info.x + scrollX + 600), int(info.y + scrollY - 100.f ), 80 * animIndex, 60 * animIndex, drawingDC, (animIndex % 3) * 160, 2720, 160, 120, THREEHEAD_COLOR);

			}
			else if (animIndex < 6)
			{
				GdiTransparentBlt(_hdc, int(info.x + scrollX + 300), int(info.y  + scrollY - 100.f), 80 * animIndex, 60 * animIndex, drawingDC, (animIndex % 3) * 160, 2880, 160, 120, THREEHEAD_COLOR);
				GdiTransparentBlt(_hdc, int(info.x + scrollX + 600), int(info.y + scrollY - 100.f), 80 * animIndex, 60 * animIndex, drawingDC, (animIndex % 3) * 160, 2880, 160, 120, THREEHEAD_COLOR);

			}
			else if (animIndex < 9)
			{
				GdiTransparentBlt(_hdc, int(info.x + scrollX + 300), int(info.y  + scrollY - 100.f), 80 * animIndex, 60 * animIndex, drawingDC, (animIndex % 3) * 160, 3230, 160, 120, THREEHEAD_COLOR);
				GdiTransparentBlt(_hdc, int(info.x + scrollX + 600), int(info.y + scrollY - 100.f), 80 * animIndex, 60 * animIndex, drawingDC, (animIndex % 3) * 160, 3230, 160, 120, THREEHEAD_COLOR);

			}


			if (animIndex == 9)
			{
				isDead = true;
				Release(); 
			}
		break;
	default:
		GdiTransparentBlt(_hdc, int(info.x - 50.f + scrollX), int(info.y - 50.f + scrollY), 1026, 324, drawingDC, 0, 30, 380, 120, THREEHEAD_COLOR);
		break;
	}
}

void ThreeHead::Release()
{
	ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY,left);
	ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, right);
	ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, mid);

	SAFE_DELETE(left);
	SAFE_DELETE(right);
	SAFE_DELETE(mid);
}

void ThreeHead::State_Machine()
{
	switch (state)
	{
	case THREEHEAD::NONE:
		if (isMove)
		{
			Send_State(THREEHEAD::DOOR_OPEN);
		}
		break;
	}
}

void ThreeHead::Send_State(THREEHEAD::STATE _state)
{
	state = _state;
	if(!left->Get_Dying())
		left->Set_State(_state);
	if (!right->Get_Dying())
		right->Set_State(_state);
	if (!mid->Get_Dying())
		mid->Set_State(_state);
}

void ThreeHead::Tower_On()
{
	if (doorOpening)
		return;

	if (totalY >= 150.f && !doorOpening)
	{
		Send_State(THREEHEAD::DOOR_OPEN);
		doorOpening = true;
		return; 
	}

	if (!towerOn && CScrollMgr::Get_Instance()->Get_ScrollLockX() <= WINCX-CScrollMgr::Get_Instance()->Get_ScrollX())
		towerOn = true;

	if (towerOn && totalY < 150.f)
	{
		CScrollMgr::Get_Instance()->Set_ScrollY(0.6f);
		left->Add_Y(-3.f);
		mid->Add_Y(-3.f);
		right->Add_Y(-3.f);
		totalY += 3.f;
	}
}

bool ThreeHead::AllDestroied()
{
	if (left->Get_Dying() && mid->Get_Dying() && right->Get_Dying())
		return true;
	return false;
}

