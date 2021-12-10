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
	void	Follower(); 
	void	Die();
	void	ToTheIdle();

private:
	SOLDAE::STATE state;

private:
	bool	showTime; 
	float	showTimeY;

	int		rootGauge;
	int		followerGauge;
	int		sunshineGauge;
	int		laserGauge;

	int		fullGauge;

	float	totalIdleY;
	float	maxIdleY; 

	float	idleY;
	float	laserY;

	float	initX;
	float	initY;

private:
	bool	openingSky;
	bool	crashing;
	bool	toTheIdle; 

	float	toSky;
	float	totalSky;

	SolDaeLaser* laser; 

private:
	bool	rootMotion;
	bool	motionEnd;

	int		currRoot;
	int		maxRoot;
	DWORD	rootSpawnTimer;


	bool	initPos;
	DWORD	effectTimer; 

	DWORD	effectStable;

	int		effectIndex;
	int		randomX, randomY, randomCX, randomCY;
};

