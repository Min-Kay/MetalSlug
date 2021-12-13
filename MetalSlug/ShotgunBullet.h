#pragma once
#include "Bullet.h"

class ShotgunBullet :
    public Bullet
{

public:
	ShotgunBullet() { init_CX = 0.f; init_CY = 0.f;  };
	~ShotgunBullet() { Release(); }
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir) ;
	virtual void Update_Rect();
private:
	float init_CX;
	float init_CY;

	vector<Obj*> hits; 
};

