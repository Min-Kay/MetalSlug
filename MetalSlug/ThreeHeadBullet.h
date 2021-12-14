#pragma once
#include "Bullet.h"
class ThreeHeadBullet :
    public Bullet
{
public:
	ThreeHeadBullet();
	virtual ~ThreeHeadBullet();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	void Set_Angle();
	void Check_Target();
	void Set_Target(Obj* _target);
	void Move();

private:
	Obj* target;
	float followingSpeed;
	float angle;
	float totalMove;

	bool follow;
};

