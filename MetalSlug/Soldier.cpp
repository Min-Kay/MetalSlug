#include "Soldier.h"
#include "Include.h"
#include "Manager.h"
void Soldier::Initialize()
{
	id = OBJ::ENEMY;
	render = RENDER::OBJECT;
	dir = DIR::RIGHT;
	sol_Class = SOLDIER::PRIVATE;
	info.cx = 50.f; 
	info.cy = 50.f; 

	speed = 5.f; 
	hp = 10;
	isMove = false;
}

int Soldier::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (!isMove)
	{
		action = ACTION::IDLE;
		return OBJ_DEFAULT;
	}
	
	State_Machine(); 
	Update_Rect(); 

	return OBJ_DEFAULT;
}

void Soldier::Late_Update()
{
	
	Check_Hp();
}

void Soldier::Render(HDC _hdc)
{
	Rectangle(_hdc,rect.left,rect.top,rect.right,rect.bottom);
}

void Soldier::Release()
{
}

void Soldier::State_Machine()
{
	switch (action)
	{
	case ACTION::IDLE:
		// IDLE 애니메이션 실행
		if (ObjPoolMgr::Get_Instance()->Check_Distance(this) < WINCX)
		{
			isMove = true;
			action = ACTION::MOVE;
		}

		break;
	case ACTION::MOVE:
		if (sol_Class == SOLDIER::PRIVATE)
		{
			
		}
		// 플레이어 방향으로 이동 
		if (ObjPoolMgr::Get_Instance()->Check_Distance(this) < 200)
		{
			action = ACTION::FIRE;
		}
		break;
	case ACTION::JUMP:
		break;
	case ACTION::FIRE:
		if (ObjPoolMgr::Get_Instance()->Check_Distance(this) > 50)
		{
			action = ACTION::STAB;
		}

		if (ObjPoolMgr::Get_Instance()->Check_Distance(this) > 200)
		{
			action = ACTION::MOVE;
		}
		break;
	case ACTION::STAB:
		if (ObjPoolMgr::Get_Instance()->Check_Distance(this) < 200)
		{
			action = ACTION::FIRE;
		}
		break;
	case ACTION::GRENADE:
		// 수류탄을 플레이어 방향으로
		break;
	case ACTION::DIE:
		// 사망 애니메이션
		break;
	default:
		break;
	}
}
