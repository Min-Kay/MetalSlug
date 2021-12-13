#pragma once
#include "Bullet.h"
class SolDaeLaser :
	public Bullet
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);
	void	Set_Parent(Obj* _parent) { parent = _parent; }

private:
	void	Set_Position();

private:
	Obj* parent = nullptr;
	
	float	totalCY = 0.f;
	float	increaseCY = 0.f;

};

