#include "EnemyBullet.h"
#include "Manager.h"

void EnemyBullet::Initialize()
{
	id = OBJ::BULLET;
	parentID = OBJ::ENEMY;
	speed = 7.f;
	info.cx = 30.f;
	info.cy = 30.f;
	damage = 10;

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy_Bullet.bmp",L"Enemy_Bullet");
}

int EnemyBullet::Update()
{
	if (isDead)
		return OBJ_DEAD;

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

	Update_Rect();

	return OBJ_DEFAULT;
}

void EnemyBullet::Late_Update()
{
	Check_Screen_Out();
}

void EnemyBullet::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Enemy_Bullet");
	Anim_Counter(1,50.f);
	GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, info.cy, drawingDC, animIndex * 15, 0, 15, 15, RGB(255, 255, 255));

}

void EnemyBullet::Release()
{
}
