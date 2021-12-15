#pragma once
#include "Enemy.h"
#include "Parachute.h"

class Rosin :
	public Enemy
{
public:
	Rosin();
	virtual ~Rosin();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	void Explosive();
	virtual void Gravity();
	virtual void Update_Rect();
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);
	void Explode();
private:
	Parachute* para = nullptr; 
	bool explode = false; 
	bool isInit = false; 

	float init_CX = 0.f;
	float init_CY = 0.f;

};

