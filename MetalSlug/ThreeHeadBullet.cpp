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
}

ThreeHeadBullet::~ThreeHeadBullet()
{
	target = nullptr;
}

void ThreeHeadBullet::Initialize()
{
	speed = 1.0f;
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

    return OBJ_DEFAULT;
}

void ThreeHeadBullet::Late_Update()
{
}

void ThreeHeadBullet::Render(HDC _hdc)
{

	drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Soldier_Bazuka");

	Anim_Counter(3, 100.f);

	GdiTransparentBlt(_hdc, rect.left + CScrollMgr::Get_Instance()->Get_ScrollX(), rect.top + CScrollMgr::Get_Instance()->Get_ScrollY(), 40, 40, drawingDC, 38 + animIndex * 14, 804, 14, 14, RGB(0, 255, 0));

}

void ThreeHeadBullet::Release()
{
}
