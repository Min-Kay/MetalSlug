#include "Player.h"
#include "Manager.h"
#include "Weapons.h"
#include "Enemy.h"
#include "Bullets.h"

void Player::Initialize()
{
	id = OBJ::PLAYER;
	dir = DIR::RIGHT;
	onlySide = DIR::RIGHT;

	info.cx = 100.f;
	info.cy = 100.f; 
	init_CY = info.cy;

	isDead = false;
	walkSpeed = 5.f;
	speed = walkSpeed;
	sitSpeed = walkSpeed * 0.5f;

	isStab = false;

	isDying = false;
	isFiring = false;
	isGrenading = false;
	isStabbing = false;
	canRide = true; 

	isJump = false;
	jumpY = 0.f;
	jumpForce = 30.f;
	jumpTime = 0.f;

	isValid = true; 
	validTimer = GetTickCount(); 
	validTick = GetTickCount(); 
	animValidIndex = 0;

	action = ACTION::IDLE;

	Set_Weapon(new Pistol);
	DataMgr::Get_Instance()->Set_Weapon(weapon);

	BmpMgr::Get_Instance()->Insert_Bmp(PLAYER_BMP, PLAYER_KEY);
	BmpMgr::Get_Instance()->Insert_Bmp(STRETCH_BMP, STRETCH_KEY);

	superValid = false;
}

int Player::Update()
{
	if (isDead)
		return OBJ_DEAD;

	KeyInput();
	Gravity();
	Update_Rect(); 
	return OBJ_DEFAULT;
}

void Player::Late_Update()
{
	Valid(); 
	Check_WeaponState(); 
}

void Player::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	//Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);
	switch (action)
	{
	case ACTION::IDLE:
		Anim_Idle(_hdc);
		break;
	case ACTION::JUMP:
		Anim_Jumping(_hdc);
		break; 
	case ACTION::MOVE:
		Anim_Moving(_hdc);
		break;
	case ACTION::DIE:
		Anim_Dying(_hdc);
		break;
	}
}

void Player::Release()
{
}

void Player::KeyInput()
{
	if (isDead || isDying)
	{
		weapon->Set_Fire(false);
		return;
	}

	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();


	if (CKeyMgr::Get_Instance()->Key_Down('D'))
	{
		superValid = !superValid;
	}

	if (dir != DIR::DOWN)
	{
		switch (dir)
		{
		case DIR::LEFT:
			isFiring = weapon->Fire(rect.left + 5.f, info.y + 10.f, dir);
			break;
		case DIR::RIGHT:
			isFiring = weapon->Fire(rect.right - 5.f, info.y + 10.f, dir);
			break;
		case DIR::UP:
			if (onlySide == DIR::LEFT)
			{
				isFiring = weapon->Fire(info.x + 20.f, rect.top - 15.f, dynamic_cast<Ironlizard*>(weapon) ? onlySide : dir);
			}
			else
			{
				isFiring = weapon->Fire(info.x - 20.f, rect.top - 15.f, dynamic_cast<Ironlizard*>(weapon) ? onlySide : dir);
			}
			break;
		}
	}
	else if (jumping && dir == DIR::DOWN)
	{
		if (onlySide == DIR::LEFT)
		{
			isFiring = weapon->Fire(info.x + 20.f, rect.bottom + 15.f, dynamic_cast<Ironlizard*>(weapon)? onlySide : dir);
		}
		else
		{
			isFiring = weapon->Fire(info.x - 20.f, rect.bottom + 15.f, dynamic_cast<Ironlizard*>(weapon) ? onlySide : dir);
		}
	}
	else
	{
		switch (onlySide)
		{
		case DIR::LEFT:
			isFiring = weapon->Fire(rect.left + 5.f, info.y - 5.f, onlySide);
			break;
		case DIR::RIGHT:
			isFiring = weapon->Fire(rect.right - 5.f, info.y - 5.f, onlySide);
			break;
		}
	}

	action = jumping ? ACTION::JUMP : ACTION::IDLE;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		dir = DIR::UP;
	}
	else
	{
		dir = onlySide;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		if (rect.left - speed < 0 - scrollX)
			return;

		action = ACTION::MOVE;
		dir = DIR::LEFT;
		onlySide = DIR::LEFT;
		info.x -= speed;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		if (rect.right + speed > WINCX - scrollX)
			return;

		action = ACTION::MOVE;
		dir = DIR::RIGHT;
		onlySide = DIR::RIGHT;
		info.x += speed;

		if( PLAYER_X < info.x + CScrollMgr::Get_Instance()->Get_ScrollX())
			CScrollMgr::Get_Instance()->Set_ScrollX(-speed);
	}

	if (CKeyMgr::Get_Instance()->Key_Rollover(VK_JUMP, VK_UP) && !isJump)
	{
		isJump = true;
		canRide = true;
	}
	else if (!isJump && canRide)
	{
		canRide = false;
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_JUMP) && !isJump)
	{
		isJump = true;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('F'))
	{
		Set_Dying();
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		dir = DIR::DOWN;
		speed = sitSpeed;
		if (jumping)
		{
			info.cy = 100.f;
			speed = walkSpeed;
		}
		else
		{
			info.cy = 50.f;
			action = ACTION::IDLE;
		}
	}
	else
	{
		info.cy = 100.f;
		speed = walkSpeed;
	}
}

void Player::Gravity()
{
	if (isDead)
		return;

	float fY = 0.f;
	float blockY = 0.f; 
	float jumpingState = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x,info.y,&fY);
	bool blockCol = BlockMgr::Get_Instance()->Collision_Block(this,&blockY);

	if (isJump)
	{
		if (action != ACTION::DIE)	action = ACTION::JUMP;
		jumpingState = (jumpForce * jumpTime - 9.8f * jumpTime * jumpTime * 0.7f) / 3.f;
		info.y -= jumpingState;
		jumpTime += 0.2f;
		jumping = true;

		if (lineCol && info.y >= fY - init_CY * 0.5f && jumpingState < 0)
		{
			info.y = fY - info.cy * 0.5f;
			if (action != ACTION::DIE) action = ACTION::IDLE;
			jumpTime = 0.f;
			isJump = false;
		}
		else if (blockCol && info.y > blockY && jumpingState < 0)
		{
			info.y = blockY + 1;
			if (action != ACTION::DIE) action = ACTION::IDLE;
			jumpTime = 0.f;
			isJump = false;
		}
	}
	else if (blockCol && info.y > blockY)
	{
		jumping = false;
		info.y = blockY + 1;
	}
	else if (lineCol && info.y < fY - init_CY * 0.6f)
	{
		jumping = true;
		if (action != ACTION::DIE) action = ACTION::JUMP;
		info.y += FALL_DOWN;
	}
	else if (lineCol && info.y >= fY - init_CY * 0.5f)
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
	stretchDC = BmpMgr::Get_Instance()->Find_Image(STRETCH_KEY);

	if (isFiring)
	{
		switch (dir)
		{
		case DIR::RIGHT:
			Anim_Counter(ANIM::PLAYER_FIRE, 3, 30.f);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx * 2, info.cy, drawingDC, animIndexs[ANIM::PLAYER_FIRE] * 200 + 80, animIndexPos[ANIM::PLAYER_FIRE] * 200 + 70, info.cx * 0.5f * 2, info.cy * 0.5f, PLAYER_COLOR);
			break;
		case DIR::LEFT:
			Anim_Counter(ANIM::PLAYER_FIRE, 3, 30.f);
			StretchBlt(stretchDC, 0, 0, info.cx * 0.5f * 2, info.cy * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_FIRE] * 200 + 80 + info.cx, animIndexPos[ANIM::PLAYER_FIRE] * 200 + 70, -(info.cx * 0.5f * 2), info.cy * 0.5f, SRCCOPY);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx, int(rect.top + scrollY), info.cx * 2, info.cy, stretchDC, 0, 0, info.cx * 0.5f * 2, info.cy * 0.5f, PLAYER_COLOR);
			break;
		case DIR::UP:
			if (onlySide == DIR::RIGHT)
			{
				Anim_Counter(ANIM::PLAYER_FIRE, 1, 30.f, true, 7);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - info.cy, info.cx, info.cy * 2, drawingDC, animIndexs[ANIM::PLAYER_FIRE] * 200 + 80, animIndexPos[ANIM::PLAYER_FIRE] * 200 + 70 - info.cy * 0.5f, info.cx * 0.5f, info.cy * 0.5f * 2, PLAYER_COLOR);
			}
			else
			{
				Anim_Counter(ANIM::PLAYER_FIRE, 1, 30.f, true, 7);
				StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f * 2, drawingDC, animIndexs[ANIM::PLAYER_FIRE] * 200 + 80 + info.cx * 0.5f, animIndexPos[ANIM::PLAYER_FIRE] * 200 + 70 - info.cy * 0.5f, -info.cx * 0.5f, info.cy * 0.5f * 2, SRCCOPY);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - info.cy, info.cx, info.cy * 2, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f * 2, PLAYER_COLOR);
			}
			break;
		case DIR::DOWN:
			if (onlySide == DIR::RIGHT)
			{
				Anim_Counter(ANIM::PLAYER_DOWN_FIRE, 3, 30.f);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - init_CY * 0.5f, info.cx * 2, init_CY, drawingDC, animIndexs[ANIM::PLAYER_DOWN_FIRE] * 200 + 80, animIndexPos[ANIM::PLAYER_DOWN_FIRE] * 200 + 70, info.cx * 0.5f * 2, init_CY * 0.5f, PLAYER_COLOR);
			}
			else if (onlySide == DIR::LEFT)
			{
				Anim_Counter(ANIM::PLAYER_DOWN_FIRE, 3, 30.f);
				StretchBlt(stretchDC, 0, 0, info.cx * 0.5f * 2, init_CY * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_DOWN_FIRE] * 200 + 80 + info.cx, animIndexPos[ANIM::PLAYER_DOWN_FIRE] * 200 + 70, -(info.cx * 0.5f * 2), init_CY * 0.5f, SRCCOPY);

				GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx, int(rect.top + scrollY) - init_CY * 0.5f, info.cx * 2, init_CY, stretchDC, 0, 0, info.cx * 0.5f * 2, init_CY * 0.5f, PLAYER_COLOR);
			}
			break;
		}
	}
	else if (isGrenading)
	{

	}
	else if (isStabbing)
	{
		
	}
	else
	{
		switch (dir)
		{
		case DIR::RIGHT:
			Anim_Counter(ANIM::PLAYER_IDLE, 5, 100.f);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndexs[ANIM::PLAYER_IDLE] * 200 + 80, animIndexPos[ANIM::PLAYER_IDLE] * 200 + 70, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			break;
		case DIR::LEFT:
			Anim_Counter(ANIM::PLAYER_IDLE, 5, 100.f);

			StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_IDLE] * 200 + 80 + (info.cx * 0.5f), animIndexPos[ANIM::PLAYER_IDLE] * 200 + 70, -info.cx * 0.5f, info.cy * 0.5f, SRCCOPY);

			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);

			break;
		case DIR::UP:
			if (onlySide == DIR::RIGHT)
			{
				Anim_Counter(ANIM::PLAYER_UP, 1, 100.f, false, 2);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndexs[ANIM::PLAYER_UP] * 200 + 80, animIndexPos[ANIM::PLAYER_UP] * 200 + 70, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			}
			else
			{
				Anim_Counter(ANIM::PLAYER_UP, 1, 100.f, false, 2);

				StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_UP] * 200 + 80 + info.cx * 0.5f, animIndexPos[ANIM::PLAYER_UP] * 200 + 70, -info.cx * 0.5f, info.cy * 0.5f, SRCCOPY);

				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			}
			break;
		case DIR::DOWN:
			if (onlySide == DIR::RIGHT)
			{
				Anim_Counter(ANIM::PLAYER_DOWN, 1, 100.f, true, 4);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - init_CY * 0.5f, info.cx, init_CY, drawingDC, animIndexs[ANIM::PLAYER_DOWN] * 200 + 80, animIndexPos[ANIM::PLAYER_DOWN] * 200 + 70, info.cx * 0.5f, init_CY * 0.5f, PLAYER_COLOR);
			}
			else if (onlySide == DIR::LEFT)
			{
				Anim_Counter(ANIM::PLAYER_DOWN, 1, 100.f, true, 4);

				StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, init_CY * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_DOWN] * 200 + 80 + info.cx * 0.5f, animIndexPos[ANIM::PLAYER_DOWN] * 200 + 70, -(info.cx * 0.5f), init_CY * 0.5f, SRCCOPY);

				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - init_CY * 0.5f, info.cx, init_CY, stretchDC, 0, 0, info.cx * 0.5f, init_CY * 0.5f, PLAYER_COLOR);
			}
			break;
		}
	}
}

void Player::Anim_Moving(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(PLAYER_KEY);
	stretchDC = BmpMgr::Get_Instance()->Find_Image(STRETCH_KEY);

	if (isFiring)
	{
		switch (dir)
		{
		case DIR::RIGHT:
			Anim_Counter(ANIM::PLAYER_FIRE, 7, 30.f, true, 11);

			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx * 2, info.cy, drawingDC, animIndexs[ANIM::PLAYER_FIRE] * 200 + 80, animIndexPos[ANIM::PLAYER_FIRE] * 200 + 70, info.cx * 0.5f * 2, info.cy * 0.5f, PLAYER_COLOR);
			break;
		case DIR::LEFT:
			Anim_Counter(ANIM::PLAYER_FIRE, 7, 30.f, true, 11);

			StretchBlt(stretchDC, 0, 0, info.cx * 0.5f * 2, info.cy * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_FIRE] * 200 + 80 + info.cx, animIndexPos[ANIM::PLAYER_FIRE] * 200 + 70, -(info.cx * 0.5f * 2), info.cy * 0.5f, SRCCOPY);

			GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx, int(rect.top + scrollY), info.cx * 2, info.cy, stretchDC, 0, 0, info.cx * 0.5f * 2, info.cy * 0.5f, PLAYER_COLOR);
			break; 
		}
		
	}
	else if (isGrenading)
	{

	}
	else if (isStabbing)
	{

	}
	else
	{
		switch (dir)
		{
		case DIR::RIGHT:
			Anim_Counter(ANIM::PLAYER_MOVE, 11, 30.f);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndexs[ACTION::MOVE] * 200 + 80, animIndexPos[ACTION::MOVE] * 200 + 70, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			break;
		case DIR::LEFT:
			Anim_Counter(ANIM::PLAYER_MOVE, 11, 30.f);
			StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, drawingDC, animIndexs[ACTION::MOVE] * 200 + 80 + info.cx * 0.5f, animIndexPos[ACTION::MOVE] * 200 + 70, -info.cx * 0.5f, info.cy * 0.5f, SRCCOPY);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			break;
		case DIR::UP:
			if (onlySide == DIR::RIGHT)
			{
				Anim_Counter(ANIM::PLAYER_MOVE, 18, 100.f, true, 3);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndexs[ANIM::PLAYER_MOVE] * 200 + 80, animIndexPos[ANIM::PLAYER_MOVE] * 200 + 70, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			}
			else if (onlySide == DIR::LEFT)
			{
				Anim_Counter(ANIM::PLAYER_MOVE, 18, 100.f, true, 3);

				StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_MOVE] * 200 + 80 + info.cx * 0.5f, animIndexPos[ANIM::PLAYER_MOVE] * 200 + 70, -info.cx * 0.5f, info.cy * 0.5f, SRCCOPY);

				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			}
			break;
		case DIR::DOWN:
			if (onlySide == DIR::RIGHT)
			{
				Anim_Counter(ANIM::PLAYER_DOWN, 13, 100.f, true, 3);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - init_CY * 0.5f, info.cx * 0.5f, init_CY * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_DOWN] * 200 + 80, animIndexPos[ANIM::PLAYER_DOWN] * 200 + 70, info.cx * 0.5f, init_CY * 0.5f, PLAYER_COLOR);
			}
			else if (onlySide == DIR::LEFT)
			{
				Anim_Counter(ANIM::PLAYER_DOWN, 13, 100.f, true, 3);

				StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, init_CY * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_DOWN] * 200 + 80 + 50, animIndexPos[ANIM::PLAYER_DOWN] * 200 + 70, -info.cx * 0.5f, init_CY * 0.5f, SRCCOPY);

				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - init_CY * 0.5f, 50, info.cy, stretchDC, 0, 0, info.cx * 0.5f, init_CY * 0.5f, PLAYER_COLOR);
			}
			break;
		}
	}
}

void Player::Anim_Jumping(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(PLAYER_KEY);
	stretchDC = BmpMgr::Get_Instance()->Find_Image(STRETCH_KEY);

	if (isFiring)
	{
		switch (dir)
		{
		case DIR::RIGHT:
			Anim_Counter(ANIM::PLAYER_JUMP_FIRE, 7, 30.f);

			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx * 2, info.cy, drawingDC, animIndexs[ANIM::PLAYER_JUMP_FIRE] * 200 + 80, animIndexPos[ANIM::PLAYER_JUMP_FIRE] * 200 + 70, info.cx * 0.5f * 2, info.cy * 0.5f, PLAYER_COLOR);
			break;
		case DIR::LEFT:
			Anim_Counter(ANIM::PLAYER_JUMP_FIRE, 7, 30.f);

			StretchBlt(stretchDC, 0, 0, info.cx * 2 * 0.5f, info.cy * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_JUMP_FIRE] * 200 + 80 + info.cx, animIndexPos[ANIM::PLAYER_JUMP_FIRE] * 200 + 70, -(info.cx * 0.5f * 2), info.cy * 0.5f, SRCCOPY);

			GdiTransparentBlt(_hdc, int(rect.left + scrollX) - info.cx, int(rect.top + scrollY), info.cx * 2, info.cy, stretchDC, 0, 0, info.cx * 0.5f * 2, info.cy * 0.5f, PLAYER_COLOR);
			break;
		case DIR::UP:
			if (onlySide == DIR::RIGHT)
			{
				Anim_Counter(ANIM::PLAYER_JUMP_FIRE, 7, 30.f, true, 10);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - info.cy, info.cx, info.cy * 2, drawingDC, animIndexs[ANIM::PLAYER_JUMP_FIRE] * 200 + 80, animIndexPos[ANIM::PLAYER_JUMP_FIRE] * 200 + 70 - info.cy * 0.5f, info.cx * 0.5f, info.cy * 0.5f * 2, PLAYER_COLOR);
			}
			else if (onlySide == DIR::LEFT)
			{
				Anim_Counter(ANIM::PLAYER_JUMP_FIRE, 7, 30.f, true, 10);

				StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f * 2, drawingDC, animIndexs[ANIM::PLAYER_JUMP_FIRE] * 200 + 80 + info.cx * 0.5f, animIndexPos[ANIM::PLAYER_JUMP_FIRE] * 200 + 70 - info.cy * 0.5f, -info.cx * 0.5f, info.cy * 0.5f * 2, SRCCOPY);

				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY) - info.cy, info.cx, info.cy * 2, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f * 2, PLAYER_COLOR);
			}
			break;
		case DIR::DOWN:

			if (onlySide == DIR::RIGHT)
			{
				Anim_Counter(ANIM::PLAYER_JUMP_FIRE, 8, 30.f, true, 26);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy * 2, drawingDC, animIndexs[ANIM::PLAYER_JUMP_FIRE] * 200 + 80, animIndexPos[ANIM::PLAYER_JUMP_FIRE] * 200 + 70, info.cx * 0.5f, info.cy * 0.5f * 2, PLAYER_COLOR);
			}
			else if (onlySide == DIR::LEFT)
			{
				Anim_Counter(ANIM::PLAYER_JUMP_FIRE, 8, 30.f, true, 26);

				StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f * 2, drawingDC, animIndexs[ANIM::PLAYER_JUMP_FIRE] * 200 + 80 + info.cx * 0.5f, animIndexPos[ANIM::PLAYER_JUMP_FIRE] * 200 + 70, -(info.cx * 0.5f), info.cy * 0.5f * 2, SRCCOPY);

				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy * 2, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f * 2, PLAYER_COLOR);

			}
			break;
		}
	}
	else if (isGrenading)
	{

	}
	else if (isStabbing)
	{

	}
	else
	{
		switch (dir)
		{
		case DIR::UP:
			if (onlySide == DIR::RIGHT)
			{
				Anim_Counter(ANIM::PLAYER_JUMP_UP, 4, 100.f, true, 4);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndexs[ANIM::PLAYER_JUMP_UP] * 200 + 80, animIndexPos[ANIM::PLAYER_JUMP_UP] * 200 + 70, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			}
			else if (onlySide == DIR::LEFT)
			{
				Anim_Counter(ANIM::PLAYER_JUMP_UP, 4, 100.f, true, 4);

				StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_JUMP_UP] * 200 + 80 + info.cx * 0.5f, animIndexPos[ANIM::PLAYER_JUMP_UP] * 200 + 70, -info.cx * 0.5f, info.cy * 0.5f, SRCCOPY);

				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			}
			break;
		case DIR::DOWN:
			if (onlySide == DIR::RIGHT)
			{
				Anim_Counter(ANIM::PLAYER_JUMP_DOWN, 9, 100.f, true, 2);
				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndexs[ANIM::PLAYER_JUMP_DOWN] * 200 + 80, animIndexPos[ANIM::PLAYER_JUMP_DOWN] * 200 + 70, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			}
			else if (onlySide == DIR::LEFT)
			{
				Anim_Counter(ANIM::PLAYER_JUMP_DOWN, 9, 100.f, true, 2);

				StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_JUMP_DOWN] * 200 + 80 + info.cx * 0.5f, animIndexPos[ANIM::PLAYER_JUMP_DOWN] * 200 + 70, -info.cx * 0.5f, info.cy * 0.5f, SRCCOPY);

				GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			}
			break;
		case DIR::RIGHT:
			Anim_Counter(ANIM::PLAYER_JUMP, 10, 100.f);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndexs[ANIM::PLAYER_JUMP] * 200 + 80, animIndexPos[ANIM::PLAYER_JUMP] * 200 + 70, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			break;
		case DIR::LEFT:
			Anim_Counter(ANIM::PLAYER_JUMP, 10, 100.f);
			StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_JUMP] * 200 + 80 + info.cx * 0.5f, animIndexPos[ANIM::PLAYER_JUMP] * 200 + 70, -info.cx * 0.5f, info.cy * 0.5f, SRCCOPY);

			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
			break;
		}
	}
}

void Player::Anim_Dying(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(PLAYER_KEY);
	stretchDC = BmpMgr::Get_Instance()->Find_Image(STRETCH_KEY);

	Anim_Counter(ANIM::PLAYER_DIE, 12, 70.f, false);
	if (animIndexs[ANIM::PLAYER_DIE] == 12 && dyingTimer + 1000.f < GetTickCount())
	{
		Set_Dead(true);
		ObjPoolMgr::Get_Instance()->Set_Player_Dead(true);
		DataMgr::Get_Instance()->Add_Life(-1);
	}
	switch (onlySide)
	{
	case DIR::RIGHT:
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndexs[ANIM::PLAYER_DIE] * 200 + 80, animIndexPos[ANIM::PLAYER_DIE] * 200 + 70, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
		break;
	case DIR::LEFT:
		StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_DIE] * 200 + 80 + info.cx * 0.5f, animIndexPos[ANIM::PLAYER_DIE] * 200 + 70, -info.cx * 0.5f, info.cy * 0.5f, SRCCOPY);
		GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
		break;
	default:
		if(onlySide == DIR::RIGHT)
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, drawingDC, animIndexs[ANIM::PLAYER_DIE] * 200 + 80, animIndexPos[ANIM::PLAYER_DIE] * 200 + 70, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
		else 
			StretchBlt(stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, drawingDC, animIndexs[ANIM::PLAYER_DIE] * 200 + 80 + info.cx * 0.5f, animIndexPos[ANIM::PLAYER_DIE] * 200 + 70, -info.cx * 0.5f, info.cy * 0.5f, SRCCOPY);
			GdiTransparentBlt(_hdc, int(rect.left + scrollX), int(rect.top + scrollY), info.cx, info.cy, stretchDC, 0, 0, info.cx * 0.5f, info.cy * 0.5f, PLAYER_COLOR);
		break;
	}
}

void Player::Set_Weapon(Weapon* _wep)
{
	SAFE_DELETE(weapon);  
	weapon = _wep;
	weapon->Initialize(); 
	DataMgr::Get_Instance()->Set_Weapon(weapon);
}

void Player::Check_WeaponState()
{
	if (weapon && weapon->Get_Ammo() == 0)
		Set_Weapon(new Pistol);
}

void Player::Anim_Counter(ANIM::PLAYER _action, int count, float _timer, bool _roop, int start)
{
	for (int i = 0; i < ANIM::PLAYER_END; ++i)
	{
		if (i != _action)
			animIndexs[i] = 0;
		else if(i == _action && animIndexs[i] < start)
			animIndexs[i] = start;
	}

	if (animTimer + _timer < GetTickCount())
	{
		if (_roop && animIndexs[_action] >= start + count)
		{
			animIndexs[_action] = start;
		}
		else  if(animIndexs[_action] < start + count)
			++animIndexs[_action];

		animTimer = GetTickCount();
	}
}

void Player::Set_Dying()
{
	if (isValid || isDying || superValid)
		return; 

	action = ACTION::DIE;
	info.cy = init_CY;
	isDying = true;
	dyingTimer = GetTickCount(); 
}
