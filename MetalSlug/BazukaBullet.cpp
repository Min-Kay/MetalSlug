#include "BazukaBullet.h"
#include "Manager.h"
void BazukaBullet::Initialize()
{
	id = OBJ::BULLET;
	parentID = OBJ::ENEMY;
	speed = 6.f;
	info.cx = 40.f;
	info.cy = 40.f;
	damage = 20;
	animIndex = 0;
	animTimer = GetTickCount();

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier_Bazuka.bmp",L"Soldier_Bazuka");
}

int BazukaBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;

	float spd = sqrtf(speed* speed);
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
	case DIR::UP_RIGHT:
		info.x += spd;
		info.y -= spd;
		break;
	case DIR::UP_LEFT:
		info.x -= spd;
		info.y -= spd;
		break;
	case DIR::DOWN_RIGHT:
		info.x += spd;
		info.y += spd;
		break;
	case DIR::DOWN_LEFT:
		info.x -= spd;
		info.y += spd;
		break;
	}

	Update_Rect();
	return OBJ_DEFAULT;
}

void BazukaBullet::Late_Update()
{
	Check_Screen_Out();
}

void BazukaBullet::Render(HDC _hdc)
{
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Bazuka");

	Anim_Counter(3,100.f);

	GdiTransparentBlt(_hdc,rect.left + CScrollMgr::Get_Instance()->Get_ScrollX(),rect.top + CScrollMgr::Get_Instance()->Get_ScrollY(),40,40,drawingDC,38 + animIndex * 14, 804,14,14,RGB(0,255,0) );
	
}

void BazukaBullet::Release()
{
}
