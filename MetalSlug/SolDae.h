#pragma once
#include "Enemy.h"

class SolDaeLaser;

class SolDae :
	public Enemy
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual void Add_HP(int _hp) { if (!isMove || isDying || isDead) return; hp += _hp; }

private:
	void	State_Machine(); 
	void	Idle();
	void	Laser();
	void	SunShine();
	void	Root();
	void	Die();
	void	ToTheIdle();

private:
	SOLDAE::STATE state = SOLDAE::IDLE;

private:
	bool	showTime = false; 
	float	showTimeY = 0.f;

	int		rootGauge = 0;
	int		followerGauge = 0;
	int		sunshineGauge = 0;
	int		laserGauge = 0;

	int		fullGauge = 0;

	float	totalIdleY = 0.f;
	float	maxIdleY = 0.f;

	float	idleY = 0.f;
	float	laserY = 0.f;

	float	initX = 0.f;
	float	initY = 0.f;

private:
	bool	openingSky = false;
	bool	crashing = false;
	bool	toTheIdle = false;

	float	toSky = 0.f;
	float	totalSky = 0.f;

	SolDaeLaser* laser = nullptr; 

private:
	bool	rootMotion = false;
	bool	motionEnd = false;

	DWORD	rootMotionTimer = GetTickCount();
	int		rootAnim = 0;
	int		currRoot = 0;
	int		maxRoot = 0;
	DWORD	rootSpawnTimer = GetTickCount();


	bool	initPos = false;
	DWORD	effectTimer = GetTickCount();

	DWORD	effectStable = GetTickCount();

	int		effectIndex = 0;
	int		randomX = 0, randomY = 0, randomCX = 0, randomCY = 0;
};

