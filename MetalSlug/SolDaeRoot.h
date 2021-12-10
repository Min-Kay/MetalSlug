#pragma once
#include "Bullet.h"

class SolDaeRoot :
    public Bullet
{

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);
	void	State_Machine();
	virtual void Gravity();

public:
	enum STATE {SPAWN,IDLE,ROOTING,EXPLODE};

private:
	STATE state;
	DWORD idleTimer;

	float totalIdle;
	float idleSpeed;
};

