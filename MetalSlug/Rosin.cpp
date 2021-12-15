#include "Rosin.h"
#include "Manager.h"
Rosin::Rosin()
{
	para = nullptr;
	init_CX = 0.f;
	init_CY = 0.f;
	explode = false;
	isInit = false;
}

Rosin::~Rosin()
{
	ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, para);
	SAFE_DELETE(para);
}

void Rosin::Initialize()
{

	id = OBJ::ENEMY;
	para = new Parachute;
	para->Initialize();
	para->Set_Parent(this);
	ObjPoolMgr::Get_Instance()->Add_Object(OBJ::ENEMY,para);

	hp = 50;
	isDying = false;
	isDead = false; 
	explode = false;
	isInit = false; 
	animIndex = 0;

	info.cx = 100.f;
	info.cy = 100.f;

	init_CX = 150.f;
	init_CY = 150.f;

	coll_Attack = false;
	canCollision = false; 

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile.bmp",L"Missile");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rosin_Explosion.bmp", L"Rosin_Explosion");
}

int Rosin::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (explode) 
		Explosive();
	else if(!explode)
		Gravity();
	Update_Rect();
	return OBJ_DEFAULT;
}

void Rosin::Late_Update()
{
	if (para && para->Get_Dead())
	{
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, para);
		SAFE_DELETE(para);
	}

	if (isDead || isDying || explode)
		return;

	if (hp <= 0)
	{
		Explode();
	}
}

void Rosin::Render(HDC _hdc)
{
	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (!explode)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Missile");
		GdiTransparentBlt(_hdc,rect.left + x,rect.top + y,info.cx,info.cy,drawingDC,800,0,200,200,RGB(255,0,255));
	}
	else
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Rosin_Explosion");
		Anim_Counter(23,50.f,false);
		GdiTransparentBlt(_hdc, rect.left + x - 25.f, rect.top + y - 25.f, 200, 200, drawingDC, 0 + 200 * animIndex, 0, 200, 200, RGB(86, 177, 222));
		if (animIndex == 23)
		{
			isDead = true; 
		}
	}
}

void Rosin::Release()
{
}

void Rosin::Explosive()
{
	coll_Attack = true;

	if (para)
	{
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, para);
		SAFE_DELETE(para);
	}

}

void Rosin::Gravity()
{
	if (isDead)
		return;

	float fY = 0.f;
	float blockY = 0.f;
	float jumpingState = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x, info.y, &fY);
	bool blockCol = BlockMgr::Get_Instance()->Collision_Block(this, &blockY);

	if (para && ((lineCol && info.y >= fY - info.cy * 0.5f) || blockCol && info.y > blockY))
	{
		ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::ENEMY, para);
		SAFE_DELETE(para);
	}
	else if (para)
	{
		info.y += 0.2f;
	}
	else if (blockCol && info.y > blockY)
	{
		Explode();
		info.y = blockY + 1;
	}
	else if (lineCol && info.y < fY - info.cy * 0.6f)
	{
		info.y += FALL_DOWN;
	}
	else if (lineCol && info.y >= fY - info.cy * 0.5f)
	{
		Explode();
		info.y = fY - info.cy * 0.5f;
	}
	else if (lineCol)
	{
		Explode();
		info.y = fY - info.cy * 0.5f;
	}
	else
	{
		info.y += FALL_DOWN;
	}
}

void Rosin::Update_Rect()
{
	if (!explode)
	{
		rect.left = info.x - info.cx * 0.5f;
		rect.right = info.x + info.cx * 0.5f;
		rect.top = info.y - info.cy * 0.5f;
		rect.bottom = info.y + info.cy * 0.5f;
	}
	else
	{
		rect.left = info.x - init_CX * 0.5f;
		rect.right = info.x + init_CX * 0.5f;
		rect.top = info.y - init_CY * 0.5f;
		rect.bottom = info.y + init_CY * 0.5f;
	}
}

void Rosin::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
	if (_id == OBJ::PLAYER)
	{
		Explode();
		_opponent->Set_Dying();
	}
}

void Rosin::Explode()
{
	if (explode)
		return;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ENEMY_BULLET);
	CSoundMgr::Get_Instance()->PlaySound(L"Kessi_Rosin_Explode.wav", CSoundMgr::ENEMY_BULLET, 1.0f);
	isDying = true;
	explode = true;
}
