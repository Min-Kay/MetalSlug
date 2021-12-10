#pragma once
#include "Bullet.h"
class RocketBullet :
    public Bullet
{
public:
	RocketBullet() {};
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
	float init_CX;
	float init_CY;

	float accel;
	float maxSpeed;
	float blow;

	bool  explosion;

	vector<Obj*> hits; 
};

