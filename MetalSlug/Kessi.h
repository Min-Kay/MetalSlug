#pragma once
#include "Enemy.h"
class Kessi :
	public Enemy
{
public:
	Kessi();
	virtual ~Kessi();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	enum STATE {IDLE,BURST,ROSIN,DESTROY};
private:
	void State_Machine();
	void list_Release(); 

private:
	STATE state = STATE::IDLE; 
	
	int burstGauge = 0; 
	int rosinGauge = 0;

	DWORD idleTime = GetTickCount();

	float totalMove = 0.f; 

	DWORD rosinTime = GetTickCount();
	int rosinCount = 0;
	int maxCount = 0;
	
	Obj* bazuka[2]{};
	Obj* burst[2]{};
	list<Obj*> props;

	float init_x = 0.f; 
	float init_y = 0.f;

	bool isInit = false;

	float angle = 0;

	bool burstOn = false;

	float burstMove = 0.f;
	float burstSpeed = 2.5f; 

	DWORD dyingTimer;
	DWORD effectTimer;
	DWORD effectStable;
	int effectIndex;

	float	randomX;
	float	randomY;
	float	randomCX;
	float	randomCY;
};
  
