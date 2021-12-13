#pragma once
#include "Enemy.h"
class Sarubia :
	public Enemy
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;


	void	State_Machine(); 
public:
	enum STATE {IDLE, MOVE, DESTROY, END};


public:
	STATE state = STATE::IDLE; 

	bool isFiring = false;
	DWORD attackTimer = GetTickCount();
	DWORD fireTimer = GetTickCount();
	int	currCount = 0;
	int maxCount = 5;

	DWORD fireEffectTimer = GetTickCount();
	int fireEffectIndex = 0;
	bool effectEnd = false;
	
};

