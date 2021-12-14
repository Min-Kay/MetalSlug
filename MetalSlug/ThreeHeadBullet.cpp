#include "ThreeHeadBullet.h"
#include "Manager.h"

ThreeHeadBullet::ThreeHeadBullet()
{
	speed = 1.0f;
	damage = 10;
	parentID = OBJ::ENEMY;
	id = OBJ::BULLET;
	isDead = false;
	isDying = false;

	target = nullptr; 
	followingSpeed = 10.f;
	angle = 0;
	totalMove = 0.f;
	follow = false;
}

ThreeHeadBullet::~ThreeHeadBullet()
{
	Release();
}

void ThreeHeadBullet::Initialize()
{
	speed = 0.2f;
	damage = 10;
	parentID = OBJ::ENEMY;
	id = OBJ::BULLET;
	isDead = false;
	isDying = false;

	info.cx = 35.f; 
	info.cy = 35.f;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Soldier_Bazuka.bmp",L"Soldier_Bazuka");
}

int ThreeHeadBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;

	Move();
	Update_Rect();
    return OBJ_DEFAULT;
}

void ThreeHeadBullet::Late_Update()
{
	Check_Screen_Out();
}

void ThreeHeadBullet::Render(HDC _hdc)
{
	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Bazuka");

	Anim_Counter(3, 100.f);

	GdiTransparentBlt(_hdc, rect.left + CScrollMgr::Get_Instance()->Get_ScrollX(), rect.top + CScrollMgr::Get_Instance()->Get_ScrollY(), 40, 40, drawingDC, 38 + animIndex * 14, 804, 14, 14, RGB(0, 255, 0));

}

void ThreeHeadBullet::Release()
{
	target = nullptr; 
}

void ThreeHeadBullet::Set_Angle()
{
	float x = (target->Get_Info().x - info.x);
	float y = (target->Get_Info().y - info.y);
	
	float slash = sqrtf(x*x + y*y);

	float fAngle = acosf(x / slash);

	angle = fAngle * 180.f / PI;
	if (target->Get_Info().y > info.y)
		angle *= -1.f; 
}

void ThreeHeadBullet::Check_Target()
{
	if (target->Get_Dying())
		target = nullptr; 
}

void ThreeHeadBullet::Set_Target(Obj* _target)
{
	target = _target;
}

void ThreeHeadBullet::Move()
{
	if (!follow)
	{
		switch (dir)
		{
		case DIR::LEFT:
			info.x -= speed;
			break;
		case DIR::RIGHT:
			info.x += speed;
			break;
		}

		totalMove += speed;


		if (totalMove > 30.f)
		{
			if (target)
				Set_Angle();
			else
				angle = 90.f; 
			follow = true;
		}
	}
	else
	{
		info.x += followingSpeed * cosf(angle * PI / 180.f);
		info.y -= followingSpeed * sinf(angle * PI / 180.f);
	}
}

