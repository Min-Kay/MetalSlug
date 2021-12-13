#pragma once
#include "Enemy.h"
class DiCokka :
	public Enemy
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	void State_Machine();

public:
	enum STATE {IDLE,MOVE,DESTROY};
private:
	STATE state = STATE::IDLE;
	int currCount = 0;
	int maxCount = 3;
	bool isFiring = false;
	bool isTurning = false;
	bool fire = false;
	DWORD	attackTimer = GetTickCount();
	DWORD	fireTimer = GetTickCount();
	DWORD	fireEffectTimer = GetTickCount();
};

