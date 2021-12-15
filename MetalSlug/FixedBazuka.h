#pragma once
#include "Enemy.h"
class FixedBazuka :
	public Enemy
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	void Attack();
	void Set_Move() { isMove = true; fireTime = GetTickCount(); }
private:
	DWORD	fireTime = GetTickCount();
};

