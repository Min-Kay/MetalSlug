#pragma once
#include "Bullet.h"
class HeavyBullet :
	public Bullet
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	const OBJ::ID& Get_ParentID() const { return parentID; }

private:
	float	init_CX;
	float	init_CY;

	float	spread;
	bool	minus;
};

