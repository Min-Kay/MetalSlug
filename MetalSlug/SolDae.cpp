#include "SolDae.h"
#include "Manager.h"
#include "SolDaeLaser.h"

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
	isMinus = true;

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

	initPos = false;
	timer = GetTickCount(); 

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
		if (timer + 5000.f < GetTickCount())
			state = SOLDAE::IDLE;
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
		if (timer + 5000.f < GetTickCount())
			state = SOLDAE::IDLE;
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
		if (timer + 5000.f < GetTickCount())
			state = SOLDAE::IDLE;
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

	float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	switch (state)
	{
	case SOLDAE::IDLE:
		
		if (!isMove)
			return;
		
		if (!showTime && totalIdleY < showTimeY)
		{
			info.y += laserY * 1.5f;
			totalIdleY += laserY * 1.5f;
			if (totalIdleY >= showTimeY)
				showTime = true; 
		}

		if (!showTime)
			return;

		if (totalIdleY >= maxIdleY)
		{
			totalIdleY = 0.f;
			isMinus = !isMinus; 
		}

		if (isMinus)
		{
			info.y += idleY;
			totalIdleY += idleY;
		}
		else
		{
			info.y -= idleY;
			totalIdleY += idleY;
		}

		rootGauge += rand() % 50;
		sunshineGauge += rand() % 50;
		followerGauge += rand() % 50;
		laserGauge += rand() % 50;

		/*if (rootGauge >= fullGauge)
		{
			state = SOLDAE::ROOT;
			rootGauge = 0;
			sunshineGauge = sunshineGauge >> 1;
			followerGauge = followerGauge >> 1;
			laserGauge = laserGauge >> 1;
		}
		else if (sunshineGauge >= fullGauge)
		{
			state = SOLDAE::SUNSHINE;
			sunshineGauge = 0;
			rootGauge = rootGauge >> 1;
			followerGauge = followerGauge >> 1;
			laserGauge = laserGauge >> 1;
		}
		else if (followerGauge >= fullGauge)
		{
			state = SOLDAE::FOLLOWER;
			followerGauge = 0;
			rootGauge = rootGauge >> 1;
			sunshineGauge = sunshineGauge >> 1;
			laserGauge = laserGauge >> 1;
		}
		else*/ if (laserGauge >= fullGauge)
		{
			state = SOLDAE::LASER;
			laserGauge = 0;
			rootGauge = rootGauge >> 1;
			sunshineGauge = sunshineGauge >> 1;
			followerGauge = followerGauge >> 1;
		}

		break;
	case SOLDAE::LASER:
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
				ObjPoolMgr::Get_Instance()->Add_Object(OBJ::BULLET,laser);
			}

			if(laser && info.x < info.cx * 0.5f - scrollX)
			{
				ObjPoolMgr::Get_Instance()->Delete_Object(OBJ::BULLET, laser);
				SAFE_DELETE(laser);
			}

			if (info.x < -scrollX - info.cx)
			{
				info.x = initX;
				info.y = initY - 400.f; 
				toTheIdle = true; 
			}
		}
		else if (toTheIdle)
		{
			if (info.y < initY)
			{
				info.y -= laserY;
			}
			else
			{
				animIndex = 0;
				openingSky = false;
				totalSky = 0.f;
				crashing = false;
				toTheIdle = false;
				state = SOLDAE::IDLE;
			}
		}
		
		break;
	case SOLDAE::SUNSHINE:
		break;
	case SOLDAE::ROOT:
		break;
	case SOLDAE::FOLLOWER:
		break;
	case SOLDAE::DIE:
		if (!initPos)
		{
			info.x = initX;
			info.y = initY;
			initPos = true; 
		}
		
		if (initPos)
		{
			info.y += idleY; 
		}

		break;
	}
}

