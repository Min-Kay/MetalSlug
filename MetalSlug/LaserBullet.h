#pragma once
#include "Bullet.h"

typedef struct Tick
{
	Obj* target = nullptr;
	DWORD dealTime = GetTickCount();
	DWORD collTime = GetTickCount();
	float distance = 0.f;

}TICK;

class LaserBullet :
	public Bullet
{
public:
	LaserBullet();
	~LaserBullet();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);
	virtual void Update_Rect();
	void	Pressing(bool _bool);
	void	Check_Tick();
	void	Reset();

private:
	void	Check_Distance();


private:
	list<TICK> tick;

	bool	isFiring = false;
	bool	listClear = false;
	
	float maxSize = 0.f;
	float renderSize = 0.f;

	DIR::ID formalDir = DIR::RIGHT;
	bool	diffDir = false;
};

