#include "Bazuka.h"
#include "Parachute.h"
#include "Manager.h"

void Bazuka::Initialize()
{
	id = OBJ::ENEMY;
	enemy_id = ENEMY::BAZUKA;
	dir = DIR::RIGHT;
	info.cx = 100.f;
	info.cy = 100.f;

	speed = 2.f;
	hp = 10;

	state = STATE::PARACHUTE;

	isMove = false;
	coll_Attack = false;
	jumpForce = 20.f;
	jumpTime = 0;
	jumping = false;
	canCollision = false;
	falling = false; 
	fireTime = GetTickCount();
	canCollision = false;
	coll_Attack = false;

	attack = false;
	isDying = false;
	isFiring = false;
	isHolding = false;
	isHold = false;

	para = new Parachute;
	para->Initialize();
	para->Set_Parent(this);
	ObjPoolMgr::Get_Instance()->Add_Object(OBJ::ENEMY,para);

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier_Bazuka.bmp",L"Bazuka");
}

int Bazuka::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (!isMove)
	{
		if (abs(ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x) < 700.f)
			isMove = true;
	}

	State_Machine(); 
	Gravity();
	Update_Rect();
	return OBJ_DEFAULT;
}

void Bazuka::Late_Update()
{
	if (hp <= 0)
	{
		isDying = true;
		state = STATE::DIE;
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, para);
		SAFE_DELETE(para);
	}
	Check_Parachute();
}

void Bazuka::Render(HDC _hdc)
{
	
	switch (state)
	{
	case Bazuka::IDLE:
		switch (dir)
		{
		case DIR::DOWN:
			break;
		case DIR::LEFT:
			break;
		case DIR::RIGHT:
			break;
		case DIR::DOWN_RIGHT:
			break;
		case DIR::DOWN_LEFT:
			break;
		default:
			break;
		}
		break;
	case Bazuka::MOVE:
		switch (dir)
		{
		case DIR::LEFT:
			break;
		case DIR::RIGHT:
			break;
		}
		break;
	case Bazuka::PARACHUTE:
		switch (dir)
		{
		case DIR::LEFT:
			if (isFiring)
			{

			}
			else
			{

			}
			break;
		case DIR::RIGHT:
			if (isFiring)
			{

			}
			else
			{

			}
			break;
		case DIR::DOWN:
			if (isFiring)
			{

			}
			else
			{

			}
			break;
		case DIR::DOWN_RIGHT:
			if (isFiring)
			{

			}
			else
			{

			}
			break;
		case DIR::DOWN_LEFT:
			if (isFiring)
			{

			}
			else
			{

			}
			break;
		}
		break;
	case Bazuka::DIE:
		break;
	}
}

void Bazuka::Release()
{
}

void Bazuka::Check_Parachute()
{
	if (para->Get_Dead())
	{
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, para);
		SAFE_DELETE(para);
	}
}

void Bazuka::Gravity()
{
	if (isDead)
		return;

	float fY = 0.f;
	float jumpingState = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x, info.y, &fY);

	if (para && ((lineCol && info.y >= fY - info.cy * 0.5f ) || (boxCollide && info.y >= collisionY - info.cy * 0.5f)))
	{
		SAFE_DELETE(para);
	}
	else if (para)
	{
		info.y += FALL_DOWN * 0.3f;
	}
	else if (boxCollide && info.y >= collisionY - info.cy * 0.5f)
	{
		info.y = collisionY + 1;
	}
	else if (lineCol && info.y < fY - info.cy * 0.6f)
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

void Bazuka::State_Machine()
{
	if (!isMove)
		return;

	float playerX = ObjPoolMgr::Get_Instance()->Get_Player_Info().x - info.x;
	float playerY = ObjPoolMgr::Get_Instance()->Get_Player_Info().y - info.y;

	switch (state)
	{
	case Bazuka::IDLE:
		if (attack)
		{
			if (abs(playerX) > 400.f )
			{
				action = ACTION::MOVE;
				attack = false;
				isFiring = false;
				isHolding = false;
				break;
			}

			if (fireTime + 1000.f < GetTickCount())
			{
				isFiring = true;

				if (ObjPoolMgr::Get_Instance()->Get_Player_Rect().bottom < rect.top)
				{
					dir = DIR::UP;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, info.x, rect.bottom, DIR::UP, OBJ::ENEMY);
				}
				else if (playerX < 0)
				{
					dir = DIR::LEFT;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.left, info.y, DIR::LEFT, OBJ::ENEMY);
				}
				else if (playerX > 0)
				{
					dir = DIR::RIGHT;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.right, info.y, DIR::RIGHT, OBJ::ENEMY);
				}
				fireTime = GetTickCount();
			}
			else if (fireTime + 300.f < GetTickCount())
			{
				isFiring = false;
				isHolding = true;
			}
		}
		break;
	case Bazuka::MOVE:
		if (abs(playerX) < 100 && playerY > 100)
			falling = true;
		else if (playerY < 100)
			falling = false;

		if (playerX < 0)
		{
			info.x -= speed;
			dir = DIR::LEFT;
		}
		else if (playerX > 0)
		{
			info.x += speed;
			dir = DIR::RIGHT;
		}
		break;
	case Bazuka::PARACHUTE:
		if (!para)
		{
			state = STATE::IDLE;
			break;
		}

		if (fireTime + 1000.f < GetTickCount())
		{
			if (playerX < -100)
			{
				if (playerY < 200)
				{
					dir = DIR::RIGHT;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA,rect.right,info.y,DIR::RIGHT,OBJ::ENEMY);
				}
				else
				{
					dir = DIR::DOWN_RIGHT;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.right, rect.bottom, DIR::DOWN_RIGHT, OBJ::ENEMY);
				}
			}
			else if (playerX > 100)
			{
				if (playerY < 200)
				{
					dir = DIR::LEFT;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.left, info.y, DIR::LEFT, OBJ::ENEMY);
				}
				else
				{
					dir = DIR::DOWN_LEFT;
					ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, rect.left, rect.bottom, DIR::DOWN_LEFT, OBJ::ENEMY);
				}
			}
			else
			{
				dir = DIR::DOWN;
				ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::BAZUKA, info.x, rect.bottom, DIR::DOWN_LEFT, OBJ::ENEMY);
			}
			fireTime = GetTickCount();
		}
		
		break;
	case Bazuka::DIE:
		break;
	}
}
