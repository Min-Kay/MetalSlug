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
	virtual void Update_Rect(); 

private:
	float	init_CX = 0.f;
	float	init_CY = 0.f;

	float	spread = 0.1f;
	bool	minus = false;
};

