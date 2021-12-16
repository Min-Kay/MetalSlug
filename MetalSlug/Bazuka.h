#pragma once
#include "Enemy.h"

class Parachute;

class Bazuka :
	public Enemy
{

public:
	Bazuka();
	virtual ~Bazuka();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	void Check_Parachute();
	virtual void Gravity();
	void State_Machine();

public:
	enum STATE {IDLE,MOVE,PARACHUTE,DIE};
private:
	float	jumpForce = 30.f;
	float	jumpTime =0.f;
	bool	jumping = false;
	bool	falling = false;

	STATE state = PARACHUTE; 
	Parachute* para = nullptr;

	DWORD	fireTime = GetTickCount(); 
};

