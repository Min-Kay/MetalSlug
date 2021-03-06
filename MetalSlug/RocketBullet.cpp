#include "RocketBullet.h"
#include "Manager.h"
#include "Enemy.h"
#include "Player.h"
#include "Npc.h"

RocketBullet::RocketBullet()
{
	init_CX = 0.f;
	init_CY = 0.f;

	accel = 0.f;
	maxSpeed = 0.f;
	blow = 0.f;

	explosion = false;
}

void RocketBullet::Initialize()
{
	damage = 20.f;
	parentID = OBJ::PLAYER;
	animIndex = 0;
	animTimer = GetTickCount();

	speed = 0.f; 
	blow = 0.f;

	info.cx = 50.f;
	info.cy = 50.f;

	init_CX = 130.f;
	init_CY = 130.f;

	dir = DIR::RIGHT;
	id = OBJ::BULLET;
	isDead = false;

	maxSpeed = 30.f;
	accel = 1.0f;

	explosion = false;
	
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rocket_Right.bmp",L"Rocket_Right");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rocket_Up.bmp", L"Rocket_Up");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rocket_Down.bmp", L"Rocket_Down");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp", L"Stretch_Item");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/medium_explosion.bmp" , L"medium_explosion");
}

int RocketBullet::Update()
{
	if (isDead)
	{
		Release();
		return OBJ_DEAD;
	}

	if (!explosion)
	{
		Rocket();
	}
	
	Update_Rect();
	return OBJ_DEFAULT;
}

void RocketBullet::Late_Update()
{
	Check_Screen_Out();
}

void RocketBullet::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	//Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);
	if (!explosion)
	{
		Anim_Counter(1, 100.f);
		switch (dir)
		{
		case DIR::UP:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Rocket_Up");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, init_CY, drawingDC, 0, 50 * animIndex, 20, 50, RGB(0, 248, 0));
			break;
		case DIR::DOWN:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Rocket_Down");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, init_CY, drawingDC, 0, 50 * animIndex, 20, 50, RGB(0, 248, 0));
			break;
		case DIR::LEFT:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Rocket_Right");
			stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
			StretchBlt(stretchDC, 0, 0, 50, 20, drawingDC, 50 * animIndex + 50, 0, -50, 20, SRCCOPY);
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, init_CX, info.cy, stretchDC, 0, 0, 50, 20, RGB(0, 248, 0));
			break;
		case DIR::RIGHT:
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Rocket_Right");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, init_CX, info.cy, drawingDC, 50 * animIndex, 0, 50, 20, RGB(0, 248, 0));
			break;
		}
	}
	else
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"medium_explosion");
		Anim_Counter(27,20.f,false);
		GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, init_CX, init_CY, drawingDC, 50 * animIndex, 0, 50, 50, RGB(255, 0, 255));
		if (animIndex == 27)
		{
			Release();
			isDead = true;
		}
	}
}

void RocketBullet::Release()
{
	hits.erase(hits.begin(),hits.end());
	hits.clear();
}

void RocketBullet::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	switch (_id)
	{
	case OBJ::BLOCK:
		if (parentID == OBJ::PLAYER && !_opponent->Get_Dying())
		{
			ExplodePosing();
			if (find(hits.begin(), hits.end(), _opponent) == hits.end())
			{
				hits.push_back(_opponent);
				_opponent->Add_HP(-damage);
			}
		}
		break;
	case OBJ::ENEMY:
		if (_id == parentID || _opponent->Get_Dying())
			return;

		ExplodePosing();
		if (find(hits.begin(), hits.end(), _opponent) == hits.end())
		{
			DataMgr::Get_Instance()->Add_Score(25);
			hits.push_back(_opponent);
			_opponent->Add_HP(-damage);
		}
		break;
	case OBJ::NPC:
		if (!static_cast<Npc*>(_opponent)->Get_Ropped())
			return;

		ExplodePosing();
		static_cast<Npc*>(_opponent)->Set_Ropped();

		break;
	case OBJ::PLAYER:
		if (_opponent->Get_Dying() || _id == parentID)
			return;

		ExplodePosing();
		if (find(hits.begin(), hits.end(), _opponent) == hits.end())
		{
			hits.push_back(_opponent);
			_opponent->Set_Dying();
		}
		break;
	}

}

void RocketBullet::Update_Rect()
{
	if (!explosion)
	{
		switch (dir)
		{
		case DIR::UP:
			rect.left = info.x - info.cx * 0.5f;
			rect.right = info.x + info.cx * 0.5f;
			rect.top = info.y - init_CY * 0.5f;
			rect.bottom = info.y + init_CY * 0.5f;
			break;
		case DIR::DOWN:
			rect.left = info.x - info.cx * 0.5f;
			rect.right = info.x + info.cx * 0.5f;
			rect.top = info.y - init_CY * 0.5f;
			rect.bottom = info.y + init_CY * 0.5f;
			break;
		case DIR::LEFT:
			rect.left = info.x - init_CX * 0.5f;
			rect.right = info.x + init_CX * 0.5f;
			rect.top = info.y - info.cy * 0.5f;
			rect.bottom = info.y + info.cy * 0.5f;
			break;
		case DIR::RIGHT:
			rect.left = info.x - init_CX * 0.5f;
			rect.right = info.x + init_CX * 0.5f;
			rect.top = info.y - info.cy * 0.5f;
			rect.bottom = info.y + info.cy * 0.5f;
			break;
		}
	}
	else
	{
		rect.left = info.x - init_CX * 0.5f;
		rect.right = info.x + init_CX * 0.5f;
		rect.top = info.y - init_CY * 0.5f;
		rect.bottom = info.y + init_CY * 0.5f;
	}
}

void RocketBullet::Rocket()
{
	if (maxSpeed > speed)
	{
		speed += accel;
		blow = speed * 0.3f;
	}

	switch (dir)
	{
	case DIR::UP:
		info.y -= speed;
		break;
	case DIR::DOWN:
		info.y += speed;
		break;
	case DIR::LEFT:
		info.x -= speed;
		break;
	case DIR::RIGHT:
		info.x += speed;
		break;
	}
	info.y -= blow;
	info.y += FALL_DOWN * 0.5f;
}

void RocketBullet::ExplodePosing()
{
	if (!explosion)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BULLET);
		CSoundMgr::Get_Instance()->PlaySound(L"Rocket_Explode.wav", CSoundMgr::BULLET,2.f);
		switch (dir)
		{
		case DIR::UP:
			info.y -= init_CY * 0.5f;
			break;
		case DIR::DOWN:
			info.y += init_CY * 0.5f;
			break;
		case DIR::LEFT:
			info.x -= init_CX * 0.5f;
			break;
		case DIR::RIGHT:
			info.x += init_CX * 0.5f;
			break;
		}
		Update_Rect();
		explosion = true;
	}
}
