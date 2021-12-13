#pragma once
#include "Bullet.h"
class IronlizardBullet :
	public Bullet
{
public:
	IronlizardBullet() {};
	virtual ~IronlizardBullet() { Release(); };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);
	virtual void Update_Rect();
	virtual void Gravity();

	void ExplodePosing();
	void Accelerate();

private:
	float init_CX = 0.1f;
	float init_CY = 0.1f;

	float accel = 0.1f;
	float maxSpeed = 0.1f;

	bool  explosion = false;
	bool  isLanded = false;

	vector<Obj*> hits;
};

