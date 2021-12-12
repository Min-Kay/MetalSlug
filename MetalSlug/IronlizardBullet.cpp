#include "IronlizardBullet.h"
#include "Npc.h"
#include "Manager.h"

void IronlizardBullet::Initialize()
{
	damage = 20.f;
	parentID = OBJ::PLAYER;
	animIndex = 0;
	animTimer = GetTickCount();

	speed = 0.f;

	info.cx = 60.f;
	info.cy = 30.f;

	init_CX = 170.f;
	init_CY = 170.f;

	dir = DIR::RIGHT;
	id = OBJ::BULLET;
	isDead = false;

	maxSpeed = 30.f;
	accel = 0.8f;
	isLanded = false;

	explosion = false;
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ironlizard.bmp",L"Ironlizard");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ironlizard_Explode.bmp", L"Ironlizard_Explode");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stretch_Item.bmp", L"Stretch_Item");
}

int IronlizardBullet::Update()
{
	if (isDead)
	{
		Release();
		return OBJ_DEAD;
	}

	if (!explosion)
	{
		Gravity();
		Accelerate();
	}

	Update_Rect();
	return OBJ_DEFAULT;
}

void IronlizardBullet::Late_Update()
{
	Check_Screen_Out();
}

void IronlizardBullet::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	//Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);
	if (!explosion)
	{
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Ironlizard");
		Anim_Counter(5,30.f);
		if (dir == DIR::RIGHT)
		{
			GdiTransparentBlt(_hdc,rect.left + scrollX - 70,rect.top + scrollY - 85, 200, 200,drawingDC,animIndex * 200,0,200,200,RGB(0,248,0));
		}
		else
		{
			stretchDC = BmpMgr::Get_Instance()->Find_Image(L"Stretch_Item");
			StretchBlt(stretchDC,0,0,200,200, drawingDC, animIndex * 200 + 200, 0, -200, 200,SRCCOPY);
			GdiTransparentBlt(_hdc, rect.left + scrollX - 70, rect.top + scrollY - 85, 200, 200, stretchDC, 0, 0, 200, 200, RGB(0, 248, 0));
		}
	}
	else
	{
		Anim_Counter(10, 30.f,false);
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"Ironlizard_Explode");
		GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, init_CX, init_CY, drawingDC, animIndex * 52, 0, 52, 75, RGB(0, 248, 0));
		if (animIndex == 10)
		{
			Release();
			isDead = true; 
		}
	}
}

void IronlizardBullet::Release()
{
	hits.erase(hits.begin(),hits.end());
	hits.clear();
}

void IronlizardBullet::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
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

void IronlizardBullet::Update_Rect()
{
	if (!explosion)
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

void IronlizardBullet::Gravity()
{
	if (isDead)
		return;

	float fY = 0.f;
	float blockY = 0.f;
	float jumpingState = 0.f;
	bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x, info.y, &fY);
	bool blockCol = BlockMgr::Get_Instance()->Collision_Block(this, &blockY);

	if (blockCol && info.y > blockY)
	{
		isLanded = true;
		info.y = blockY + 1;
	}
	else if (lineCol && info.y < fY - info.cy * 0.6f)
	{
		info.y += FALL_DOWN + 5.f;
	}
	else if (lineCol && info.y >= fY - info.cy * 0.5f)
	{
		isLanded = true;
		info.y = fY - info.cy * 0.5f;
	}
	else if (lineCol)
	{
		isLanded = true;
		info.y = fY - info.cy * 0.5f;
	}
	else
	{
		info.y += FALL_DOWN + 5.f;
	}
}

void IronlizardBullet::ExplodePosing()
{
	if (!explosion)
	{
		switch (dir)
		{
		case DIR::LEFT:
			info.x -= init_CX * 0.5f;
			info.y -= init_CY * 0.2f; 
			break;
		case DIR::RIGHT:
			info.x += init_CX * 0.5f;
			info.y -= init_CY * 0.2f;
			break;
		}
		Update_Rect();
		explosion = true;
	}
}

void IronlizardBullet::Accelerate()
{
	if (!isLanded)
		return;

	speed += accel;
	switch (dir)
	{
	case DIR::LEFT:
		info.x -= speed;
		break;
	case DIR::RIGHT:
		info.x += speed;
		break;
	}
}
