#pragma once
#include "Bullet.h"
class RocketBullet :
    public Bullet
{
public:
	RocketBullet();
	virtual ~RocketBullet() { Release(); };
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);
	virtual void Update_Rect();

	void Rocket(); 
	void ExplodePosing();
private:
	float init_CX = 0.f;
	float init_CY = 0.f;

	float accel = 0.f;
	float maxSpeed = 0.f;
	float blow =0.f;

	bool  explosion = false;

	vector<Obj*> hits; 
};

