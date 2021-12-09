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

private:
	SOLDAE::STATE state;

	bool	showTime; 
	float	showTimeY;

	int		rootGauge;
	int		followerGauge;
	int		sunshineGauge;
	int		laserGauge;

	int		fullGauge;

	bool	isMinus;
	float	totalIdleY;
	float	maxIdleY; 

	float	idleY;
	float	laserY;

	float	initX;
	float	initY;

	bool	openingSky;
	bool	crashing;
	bool	toTheIdle; 

	float	toSky;
	float	totalSky;

	SolDaeLaser* laser; 

	DWORD	timer; 

	bool	initPos;
	DWORD	effectTimer; 

	DWORD	effectStable;

	int		effectIndex;
	int		randomX, randomY, randomCX, randomCY;
};

