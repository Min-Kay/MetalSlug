#include "Player.h"
#include "Manager.h"

void Player::Initialize()
{
	render = RENDER::OBJECT; 
	dir = DIR::RIGHT;
	info.cx = 50.f;
	info.cy = 50.f; 
	isDead = false;
	speed = 5.f;

	isJump = false;
	jumpY = 0.f;
	jumpForce = 20.f;
	jumpTime = 0.f;

	isValid = true; 
	validTimer = GetTickCount(); 
	animValidIndex = 0;

	action = ACTION::IDLE;

	BmpMgr::Get_Instance()->Insert_Bmp(PLAYER_ALIVE_BMP, PLAYER_ALIVE_KEY);
	BmpMgr::Get_Instance()->Insert_Bmp(PLAYER_BMP, PLAYER_KEY);

}

int Player::Update()
{
	if (isDead)
		return OBJ_DEAD;
	KeyInput();
	Jump();
	Update_Rect(); 
	return OBJ_DEFAULT;
}

void Player::Late_Update()
{
	Valid(); 
}

void Player::Render(HDC _hdc)
{
	switch (action)
	{
	case ACTION::IDLE:
		if(isValid)
			Anim_Spawning(_hdc);
		else
		{
			Anim_Idle(_hdc);
		}
		break;
	case ACTION::ATTACK:
		if (isJump)
		{
			//Rectangle(_hdc,rect.left,rect.top,rect.right,rect.bottom);
		}
		else
		{
			//Rectangle(_hdc, rect.left, rect.top, rect.right, rect.bottom);

		}
		break;
	case ACTION::MOVE:
		if (dir == DIR::RIGHT)
		{
			//Rectangle(_hdc, rect.left, rect.top, rect.right, rect.bottom);
		}
		else
		{
			//Rectangle(_hdc, rect.left, rect.top, rect.right, rect.bottom);
		}
		break;
	case ACTION::RUN:
		break;
	case ACTION::DIE:
		break;
	}
	Rectangle(_hdc, rect.left, rect.top, rect.right, rect.bottom);

}

void Player::Release()
{
}

void Player::KeyInput()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		if (rect.left - speed < 0)
			return; 

		action = ACTION::MOVE;
		dir = DIR::LEFT;
		info.x -= speed;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		if (rect.right + speed > WINCX)
			return;

		action = ACTION::MOVE;
		dir = DIR::RIGHT;
		info.x += speed;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
	{
		isJump = true;
	}
}

void Player::Jump()
{
	float fY = 0.f;
	float jumpingState = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x,info.y,&fY);

	if (isJump)
	{
		action = ACTION::JUMP;
		jumpingState = (jumpForce * jumpTime - 9.8f * jumpTime * jumpTime * 0.5f) / 3.f;
		info.y -= jumpingState;
		jumpTime += 0.2f;

		if (lineCol && info.y >= fY - info.cy * 0.5f && jumpingState < 0 )
		{
			info.y = fY - info.cy * 0.5f;
			action = ACTION::IDLE;
			jumpTime = 0.f;
			isJump = false;
		}
	}
	else if (lineCol && info.y < fY - info.cy)
	{
		action = ACTION::JUMP;
		info.y += FALL_DOWN;
	}
	else if (lineCol && info.y >= fY - info.cy * 0.5f)
	{
		if (action == ACTION::JUMP)
			action = ACTION::IDLE;
		info.y = fY - info.cy * 0.5f;
	}
	else if (lineCol)
	{
		if (action == ACTION::JUMP)
			action = ACTION::IDLE;
		info.y = fY - info.cy * 0.5f;
	}
	else
	{
		action = ACTION::JUMP;
		info.y += FALL_DOWN;
	}
	
}

void Player::Valid()
{
	if (isValid && validTimer + 2000.f < GetTickCount())
	{
		isValid = false;
		validTimer = GetTickCount();
	}
}

void Player::Anim_Idle(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(PLAYER_KEY);
	GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), (int)info.cx, (int)info.cy, drawingDC, 0, 0, 0, 0, PLAYER_COLOR);
}

void Player::Anim_Spawning(HDC _hdc)
{
	
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(PLAYER_ALIVE_KEY);

	GdiTransparentBlt(_hdc,int(rect.left + scrollX),int(rect.top + scrollY),(int)info.cx,(int)info.cy,drawingDC,animValidIndex * 42,245,0,0, PLAYER_COLOR);

	if (animTimer + 100.f < GetTickCount())
	{
		if (animValidIndex == 8)
			return;

		++animValidIndex;
		animTimer = GetTickCount();
	}
}

void Player::Anim_Moving(HDC _hdc)
{
}

void Player::Anim_Attacking(HDC _hdc)
{
}

void Player::Anim_Dying(HDC _hdc)
{
}

void Player::Set_Collision(Obj* _opponent, DIR::ID _dir)
{
}
