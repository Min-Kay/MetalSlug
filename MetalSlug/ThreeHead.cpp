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
	isDoorOpened = false;
	isCurtainOpened = false;
	opening = false;

	totalY = 0.f;

	info.cx = 100.f; 
	info.cy = 100.f; 

	left = new Tower(THREEHEAD::LEFT, info.x + 300.f,info.y - 50.f);
	mid = new Tower(THREEHEAD::MID, info.x + 570.f, info.y - 50.f);
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

	left->Update();
	right->Update();
	mid->Update();

	Update_Rect();

	return OBJ_DEFAULT;
}

void ThreeHead::Late_Update()
{
	Tower_On();
	left->	Late_Update();
	right->	Late_Update();
	mid->	Late_Update();
}

void ThreeHead::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY,rect.right + scrollX, rect.bottom + scrollY);

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"MidBoss");
	switch (state)
	{
	case THREEHEAD::NONE:
		left->Render(_hdc);
		right->Render(_hdc);
		mid->Render(_hdc);

		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), 1026, 324, drawingDC, 0, 30, 380, 120, RGB(0, 255, 0));
		
		//타워 랜더
		//본체 랜더
		break;
	case THREEHEAD::DOOR_OPEN:
		left->Render(_hdc);
		right->Render(_hdc);
		mid->Render(_hdc);
		//타워 랜더
		//본체 랜더
		break;
	case THREEHEAD::CURTAIN_OPEN:
		left->Render(_hdc);
		right->Render(_hdc);
		mid->Render(_hdc);
		//타워 랜더
		//본체 랜더
		break;
	case THREEHEAD::ATTACK:
		left->Render(_hdc);
		right->Render(_hdc);
		mid->Render(_hdc);
		//타워 랜더
		//본체 랜더
		break;
	case THREEHEAD::DESTORY:
		left->Render(_hdc);
		right->Render(_hdc);
		mid->Render(_hdc);
		//타워 랜더
		//본체 랜더
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
	case THREEHEAD::DOOR_OPEN:
		if (isDoorOpened)
		{
			Send_State(THREEHEAD::CURTAIN_OPEN);
		}
		break;
	case THREEHEAD::CURTAIN_OPEN:
		if (isCurtainOpened)
		{
			Send_State(THREEHEAD::ATTACK);
		}
		break;
	case THREEHEAD::ATTACK:
		if (left->Get_Dying() && right->Get_Dying() && mid->Get_Dying())
		{
			isDying = true;
			Send_State(THREEHEAD::DESTORY);
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
	if (totalY >= 150.f)
		return; 

	if (!towerOn && CScrollMgr::Get_Instance()->Get_ScrollLockX() <= WINCX-CScrollMgr::Get_Instance()->Get_ScrollX())
		towerOn = true;

	if (towerOn && totalY < 150.f)
	{
		CScrollMgr::Get_Instance()->Set_ScrollY(0.5f);
		left->Add_Y(-3.f);
		mid->Add_Y(-3.f);
		right->Add_Y(-3.f);
		totalY += 3.f;
	}
}

