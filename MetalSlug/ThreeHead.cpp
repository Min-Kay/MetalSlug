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
	render = RENDER::OBJECT;
	isDead = false;

	isMove = false;
	coll_Attack = false;
	isDying = false;

	canCollision = false;

	state = THREEHEAD::NONE;

	towerOn = false; 
	doorOpening = false;

	totalY = 0.f;
		
	left = new Tower(this,THREEHEAD::LEFT, info.x + 300.f,info.y - 50.f);
	mid = new Tower(this, THREEHEAD::MID, info.x + 570.f, info.y - 50.f);
	right = new Tower(this, THREEHEAD::RIGHT, info.x + 850.f, info.y - 50.f);

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

	left->Update();
	right->Update();
	mid->Update();

	return OBJ_DEFAULT;
}

void ThreeHead::Late_Update()
{
	Tower_On();
	
	left->	Late_Update();
	right->	Late_Update();
	mid->	Late_Update();

	if (!isDying && AllDestroied())
	{
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
			GdiTransparentBlt(_hdc, int(info.x - 50.f + scrollX), int(info.y - 50.f + scrollY), 1026, 324, drawingDC, 0, 3394, 380, 120, RGB(0, 255, 0));
			Anim_Counter(13,100.f,false);
			if (animIndex < 3)
				GdiTransparentBlt(_hdc, int(info.x + scrollX ), int(info.y  + scrollY - 250.f), 160 * animIndex, 120 * animIndex , drawingDC, (animIndex % 3) * 160, 2751, 160, 120, RGB(0, 255, 0));
			else if(animIndex < 6)
				GdiTransparentBlt(_hdc, int(info.x + scrollX ), int(info.y  + scrollY - 250.f), 160 * animIndex, 120 * animIndex, drawingDC, (animIndex % 3) * 160, 2901, 160, 120, RGB(0, 255, 0));
			else if(animIndex < 9)																	 
				GdiTransparentBlt(_hdc, int(info.x + scrollX ), int(info.y  + scrollY - 250.f), 160 * animIndex, 120 * animIndex, drawingDC, (animIndex % 3) * 160, 3051, 160, 120, RGB(0, 255, 0));
			else if (animIndex < 12)																 
				GdiTransparentBlt(_hdc, int(info.x + scrollX ), int(info.y  + scrollY - 250.f), 160 * animIndex, 120 * animIndex, drawingDC, (animIndex % 3) * 160, 3301, 160, 120, RGB(0, 255, 0));
			else																					 
				GdiTransparentBlt(_hdc, int(info.x + scrollX ), int(info.y  + scrollY - 250.f), 160 * animIndex, 120 * animIndex, drawingDC, (animIndex % 3) * 160, 3451, 160, 120, RGB(0, 255, 0));
			if (animIndex == 13)
			{
				isDead = true;
				Release(); 
			}
		break;
	default:
		GdiTransparentBlt(_hdc, int(info.x - 50.f + scrollX), int(info.y - 50.f + scrollY), 1026, 324, drawingDC, 0, 30, 380, 120, RGB(0, 255, 0));
		break;
	}
}

void ThreeHead::Release()
{
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
	case THREEHEAD::DESTORY:
		
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

