#include "Soldier.h"
#include "Include.h"
#include "Manager.h"
#include "Weapons.h"

void Soldier::Initialize()
{
	id = OBJ::ENEMY;
	render = RENDER::OBJECT;
	dir = DIR::RIGHT;
	sol_Class = SOLDIER::PRIVATE;
	info.cx = 100.f; 
	info.cy = 100.f;

	speed = 2.f; 
	hp = 10;
	isMove = false;
	ranAway = false;
	isJump = false;
	jumpForce = 20.f;
	jumpTime = 0; 
	jumping = false;
	fireTime = GetTickCount();
}

int Soldier::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (ObjPoolMgr::Get_Instance()->Get_Player_Dead())
		action = ACTION::IDLE;
	
	State_Machine(); 
	Jump();
	Update_Rect(); 

	return OBJ_DEFAULT;
}

void Soldier::Late_Update()
{
	Check_Hp();
}

void Soldier::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	Rectangle(_hdc,rect.left + scrollX,rect.top,rect.right + scrollX,rect.bottom);
	switch (action)
	{
		case ACTION::IDLE:
			break;
		case ACTION::MOVE:
			break;
		case ACTION::JUMP:
			break;
		case ACTION::DIE:
			break;
	}
}

void Soldier::Release()
{
}

void Soldier::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (isDead)
		return;

	switch (_id)
	{
	case OBJ::BULLET:
		if (static_cast<Bullet*>(_opponent)->Get_ParentID() != id)
		{
			DataMgr::Get_Instance()->Add_Score(100);
			isDead = true;
			break;
		}
	}
}

void Soldier::Jump()
{
	if (isDead)
		return;

	float fY = 0.f;
	float jumpingState = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x, info.y, &fY);

	if (isJump)
	{
		if (action != ACTION::DIE)	action = ACTION::JUMP;
		jumpingState = (jumpForce * jumpTime - 9.8f * jumpTime * jumpTime * 0.7f) / 2.f;
		info.y -= jumpingState;
		jumpTime += 0.2f;
		jumping = true;

		if (lineCol && info.y >= fY - info.cy * 0.5f && jumpingState < 0)
		{
			info.y = fY - info.cy * 0.5f;
			if (action != ACTION::DIE) action = ACTION::IDLE;
			jumpTime = 0.f;
			isJump = false;
		}
	}
	else if (lineCol && info.y < fY - info.cy * 0.6f)
	{
		jumping = true;
		if (action != ACTION::DIE) action = ACTION::JUMP;
		info.y += FALL_DOWN;
	}
	else if (lineCol && info.y >= fY - info.cy * 0.5f)
	{
		if (action == ACTION::JUMP && action != ACTION::DIE)
			action = ACTION::IDLE;
		jumping = false;
		info.y = fY - info.cy * 0.5f;
	}
	else if (lineCol)
	{
		if (action == ACTION::JUMP && action != ACTION::DIE)
			action = ACTION::IDLE;
		info.y = fY - info.cy * 0.5f;
		jumping = false;
	}
	else
	{
		jumping = true;
		if (action != ACTION::DIE) action = ACTION::JUMP;
		info.y += FALL_DOWN;
	}
}

void Soldier::Anim_Idle(HDC _hdc)
{
}

void Soldier::Anim_Move(HDC _hdc)
{
}

void Soldier::Anim_Jump(HDC _hdc)
{
}

void Soldier::Anim_Die(HDC _hdc)
{
}

void Soldier::State_Machine()
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	switch (action)
	{
	case ACTION::IDLE:

		// IDLE 애니메이션 실행
		if (ObjPoolMgr::Get_Instance()->Check_Distance(this) < WINCX - scrollX)
		{
			isMove = true;
			action = ACTION::MOVE;
		}

		break;
	case ACTION::MOVE:
		if (sol_Class == SOLDIER::PRIVATE)
		{
			if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x < info.x)
			{
				info.x += speed * 1.5f;
			}
			else
			{
				info.x -= speed * 1.5f;
			}
		}
		else
		{
			if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x < info.x)
			{
				info.x -= speed;
			}
			else if(ObjPoolMgr::Get_Instance()->Get_Player_Info().x > info.x)
			{
				info.x += speed;
			}
			
		}

		if (sol_Class != SOLDIER::PRIVATE && abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < 200)
		{
			if(abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < 50)
				action = ACTION::FIRE;
			else if (ObjPoolMgr::Get_Instance()->Get_Player_Rect().bottom < rect.top || ObjPoolMgr::Get_Instance()->Get_Player_Rect().top > rect.bottom)
				break;	
			else
				action = ACTION::FIRE;
		}
		break;
	case ACTION::JUMP:
		break;
	case ACTION::FIRE:

		if (ObjPoolMgr::Get_Instance()->Check_Distance(this) > 200 ||
			(abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) > 50 && (ObjPoolMgr::Get_Instance()->Get_Player_Rect().top > rect.bottom || ObjPoolMgr::Get_Instance()->Get_Player_Rect().bottom < rect.top)))
		{
			action = ACTION::MOVE;
			break;
		}

		if (fireTime + 1000.f < GetTickCount() && ObjPoolMgr::Get_Instance()->Check_Distance(this) < 200)
		{
			if (ObjPoolMgr::Get_Instance()->Get_Player_Rect().bottom < rect.top)
			{
				ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::PISTOL, info.x, info.y, DIR::UP, 0, OBJ::ENEMY);
			}
			else if (ObjPoolMgr::Get_Instance()->Get_Player_Rect().top > rect.bottom)
			{
				ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::PISTOL, info.x, info.y, DIR::DOWN, 0, OBJ::ENEMY);
			}
			else if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x < info.x)
			{
				ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::PISTOL, info.x, info.y, DIR::LEFT, 0, OBJ::ENEMY);
			}
			else if (ObjPoolMgr::Get_Instance()->Get_Player_Info().x > info.x)
			{
				ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::PISTOL, info.x, info.y, DIR::RIGHT, 0, OBJ::ENEMY);
			}
			fireTime = GetTickCount();
		}
		
		/*if (ObjPoolMgr::Get_Instance()->Check_Distance(this) > 50)
		{
			action = ACTION::STAB;
		}*/

		
		break;
	case ACTION::STAB:
		if (ObjPoolMgr::Get_Instance()->Check_Distance(this) < 100)
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
