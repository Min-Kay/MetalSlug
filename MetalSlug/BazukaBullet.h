#pragma once
#include "Bullet.h"
class BazukaBullet :
	public Bullet
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

};

