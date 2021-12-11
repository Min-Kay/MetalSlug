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
	STATE state; 

	bool isFiring;
	DWORD attackTimer;
	DWORD fireTimer; 
	int	currCount; 
	int maxCount;

	DWORD fireEffectTimer;
	int fireEffectIndex;
	bool effectEnd;
	
};

