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
	STATE state;
	int currCount;
	int maxCount;
	bool isFiring;
	bool isTurning; 
	bool fire;
	DWORD	attackTimer;
	DWORD	fireTimer;
	DWORD	fireEffectTimer;
};

