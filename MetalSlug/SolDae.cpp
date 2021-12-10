#include "SolDae.h"
#include "Manager.h"
#include "SolDaeLaser.h"
#include "SolDaeRoot.h"

void SolDae::Initialize()
{
	info.cx = 400.f; 
	info.cy = 300.f; 

	animIndex = 0;
	animTimer = GetTickCount();

	dir = DIR::RIGHT; 
	id = OBJ::ENEMY;
	isDead = false;

	showTimeY = 300.f;
	showTime = false; 

	enemy_id = ENEMY::SOLDAE;
	state = SOLDAE::IDLE;
	hp = 1200;
	speed = 10.f;

	isMove = false;
	coll_Attack = false;
	isDying = false;

	canCollision = false;

	rootGauge = 0;
	followerGauge = 0;
	sunshineGauge = 0;
	laserGauge = 0;

	totalIdleY = 0.f;
	maxIdleY = 50.f;
	

	idleY = 0.2f; 
	laserY = 4.f;

	openingSky = false;
	crashing = false; 
	toTheIdle = false; 

	fullGauge = 10000; 

	initX = info.x; 
	initY = info.y + showTimeY;

	toSky = 0.f;
	totalSky = 0.f; 
	
	laser = nullptr;

	rootMotion = false;
	motionEnd = false; 
	currRoot = 0;
	maxRoot = 6;
	rootSpawnTimer = GetTickCount();

	initPos = false;
	effectIndex = 0; 

	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SolDae.bmp",L"SolDae");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SolDae2.bmp", L"SolDae2");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SolDae_Die.bmp", L"SolDae_Die");
	BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SolDae_Die_Effect.bmp", L"SolDae_Die_Effect");
}

int SolDae::Update()
{
	if (isDead)
		return OBJ_DEAD;

	if (!isMove && CScrollMgr::Get_Instance()->Get_ScrollLockX() <= WINCX - CScrollMgr::Get_Instance()->Get_ScrollX())
		isMove = true;

	State_Machine();
	Update_Rect();

	return OBJ_DEFAULT;
}

void SolDae::Late_Update()
{
	if (!isDying && hp <= 0)
	{
		if (laser)
		{
			ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::BULLET, laser);
			SAFE_DELETE(laser);
		}

		randomX = rand() % (int)(info.cx);
		randomY = rand() % (int)(info.cy);
		randomCX = 100 + rand() % 50;
		randomCY = 100 + rand() % 50;

		effectTimer = GetTickCount(); 
		effectStable = GetTickCount();
		isDying = true;
		state = SOLDAE::DIE;
		animIndex = 0; 
	}
}

void SolDae::Render(HDC _hdc)
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	//Rectangle(_hdc, rect.left + scrollX, rect.top + scrollY, rect.right + scrollX, rect.bottom + scrollY);

	switch (state)
	{
	case SOLDAE::IDLE:
		Anim_Counter(5, 100.f);
		if (animIndex < 3)
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC, animIndex * 220, 0, 220, 170, RGB(0, 255, 0));

		}
		else
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae2");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC, (animIndex % 3) * 220, 0, 220, 170, RGB(0, 255, 0));
		}
		
		break;
	case SOLDAE::LASER:
		Anim_Counter(5, 100.f);
		if (animIndex < 3)
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC, animIndex * 220, 0, 220, 170, RGB(0, 255, 0));

		}
		else
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae2");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC, (animIndex % 3) * 220, 0, 220, 170, RGB(0, 255, 0));
		}

		break;
	case SOLDAE::SUNSHINE:
		Anim_Counter(5, 100.f);
		if (animIndex < 3)
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC, animIndex * 220, 0, 220, 170, RGB(0, 255, 0));

		}
		else
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae2");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC, (animIndex % 3) * 220, 0, 220, 170, RGB(0, 255, 0));
		}
		break;
	case SOLDAE::ROOT:
		Anim_Counter(5, 100.f);
		if (animIndex < 3)
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC, animIndex * 220, 0, 220, 170, RGB(0, 255, 0));

		}
		else
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae2");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC, (animIndex % 3) * 220, 0, 220, 170, RGB(0, 255, 0));
		}
		motionEnd = true; 
		break;
	case SOLDAE::FOLLOWER:
		Anim_Counter(5, 100.f);
		if (animIndex < 3)
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC, animIndex * 220, 0, 220, 170, RGB(0, 255, 0));

		}
		else
		{
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae2");
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC, (animIndex % 3) * 220, 0, 220, 170, RGB(0, 255, 0));
		}
		break;
	case SOLDAE::DIE:
		drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Die");
		if (animIndex < 3)
		{
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC,animIndex * 220 + 10, 0, 220, 175, RGB(0, 255, 0));
		}
		else
		{
			GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, 400, 300, drawingDC, (animIndex % 3) * 230, 175, 230, 175, RGB(0, 255, 0));
		}

		if (effectTimer + 300.f > GetTickCount())
		{
			if (effectStable + 30.f < GetTickCount())
			{
				if (effectIndex < 13)
					++effectIndex;
				effectStable = GetTickCount();
			}
			drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Die_Effect");
			GdiTransparentBlt(_hdc, rect.left + randomX + scrollX, rect.top + randomY + scrollY, randomCX, randomCY, drawingDC, effectIndex * 45 + 3, 0, 45, 60, RGB(0, 248, 0));
		}
		else if (effectTimer + 300.f < GetTickCount())
		{
			randomX = rand() % (int)(info.cx);
			randomY = rand() % (int)(info.cy);
			randomCX = 100 + rand() % 50;
			randomCY = 120 + rand() % 50;
			effectIndex = 0;
			effectTimer = GetTickCount();
		}

		if (animIndex == 5)
		{
			isDead = true;
			DataMgr::Get_Instance()->Add_Score(10000);
		}
		Anim_Counter(5, 2500.f, false);
		break;
	}
}

void SolDae::Release()
{
}

void SolDae::State_Machine()
{
	switch (state)
	{
	case SOLDAE::IDLE:
		Idle();
		break;
	case SOLDAE::LASER:
		Laser();
		break;
	case SOLDAE::SUNSHINE:
		SunShine();
		break;
	case SOLDAE::ROOT:
		Root();
		break;
	case SOLDAE::FOLLOWER:
		Follower();
		break;
	case SOLDAE::DIE:
		Die();
		break;
	}
}

void SolDae::Idle()
{
	if (!isMove)
		return;

	if (!showTime && totalIdleY < showTimeY)
	{
		info.y += laserY * 1.5f;
		totalIdleY += laserY * 1.5f;
		if (totalIdleY >= showTimeY)
		{
			totalIdleY = 0.f;
			showTime = true;
		}
	}

	if (!showTime)
		return;

	info.y += idleY;
	totalIdleY += abs(idleY);

	if (totalIdleY >= maxIdleY)
	{
		totalIdleY = 0.f;
		idleY *= -1.f;
	}

	rootGauge += rand() % 50;
	sunshineGauge += rand() % 50;
	followerGauge += rand() % 50;
	laserGauge += rand() % 50;

	if (rootGauge >= fullGauge)
	{
		if (idleY < 0) idleY *= -1.f;
		state = SOLDAE::ROOT;
		rootGauge = 0;
		sunshineGauge = sunshineGauge >> 1;
		followerGauge = followerGauge >> 1;
		laserGauge = laserGauge >> 1;
	}
	/*else if (sunshineGauge >= fullGauge)
	{
		if (idleY < 0) idleY *= -1.f;
		state = SOLDAE::SUNSHINE;
		sunshineGauge = 0;
		rootGauge = rootGauge >> 1;
		followerGauge = followerGauge >> 1;
		laserGauge = laserGauge >> 1;
	}
	else if (followerGauge >= fullGauge)
	{
		if (idleY < 0) idleY *= -1.f;
		state = SOLDAE::FOLLOWER;
		followerGauge = 0;
		rootGauge = rootGauge >> 1;
		sunshineGauge = sunshineGauge >> 1;
		laserGauge = laserGauge >> 1;
	}*/
	else if (laserGauge >= fullGauge)
	{
		if (idleY < 0) idleY *= -1.f;
		state = SOLDAE::LASER;
		laserGauge = 0;
		rootGauge = rootGauge >> 1;
		sunshineGauge = sunshineGauge >> 1;
		followerGauge = followerGauge >> 1;
	}

}

void SolDae::Laser()
{
	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();

	if (!toTheIdle)
	{
		if (!openingSky)
		{
			toSky = info.y - 300.f;
			openingSky = true;
		}
		else if (openingSky && 300.f > totalSky)
		{
			info.y -= laserY;
			totalSky += laserY;
		}
		else if (!crashing && 300.f <= totalSky)
		{
			info.x = info.x + WINCX + info.cx;
			crashing = true;
		}
		else if (crashing && info.y < initY)
		{
			info.y += laserY;
		}
		else if (!toTheIdle && info.y >= initY)
		{
			info.x -= 12.f;

			if (!laser)
			{
				laser = new SolDaeLaser;
				laser->Initialize();
				laser->Set_Parent(this);
				ObjPoolMgr::Get_Instance()->Add_Object(OBJ::BULLET, laser);
			}

			if (laser && info.x < info.cx * 0.5f - scrollX)
			{
				ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::BULLET, laser);
				SAFE_DELETE(laser);
			}

			if (info.x < -scrollX - info.cx)
			{
				info.x = initX;
				info.y = initY - 400.f;
				openingSky = false;
				totalSky = 0.f;
				crashing = false;
				toTheIdle = true;
			}
		}
	}

	ToTheIdle();
}

void SolDae::SunShine()
{

	ToTheIdle();
}

void SolDae::Root()
{
	if (!toTheIdle)
	{
		if (!rootMotion)
			rootMotion = true;

		info.y += idleY;
		totalIdleY += abs(idleY);

		if (totalIdleY >= maxIdleY)
		{
			totalIdleY = 0.f;
			idleY *= -1.f;
		}

		if (!motionEnd)
			return;

		if (currRoot < maxRoot && rootSpawnTimer + 1500.f < GetTickCount())
		{
			ObjPoolMgr::Get_Instance()->Spawn_Bullet(BULLET::SOLDAEROOT, info.x , initY, DIR::DOWN, OBJ::ENEMY);
			++currRoot;
			rootSpawnTimer = GetTickCount();
		}

		if (currRoot == maxRoot)
		{
			currRoot = 0;
			toTheIdle = true;
		}
	}

	ToTheIdle();
}

void SolDae::Follower()
{
	ToTheIdle();
}

void SolDae::Die()
{
	if (!initPos)
	{
		info.x = initX;
		info.y = initY;
		initPos = true;
		if (idleY < 0) idleY *= -1.f;
	}

	if (initPos)
	{
		info.y += idleY;
	}

}

void SolDae::ToTheIdle()
{
	if (toTheIdle)
	{
		if (info.y < initY)
		{
			info.y += laserY;
		}
		else
		{
			animIndex = 0;
			toTheIdle = false;
			state = SOLDAE::IDLE;
		}
	}
}

