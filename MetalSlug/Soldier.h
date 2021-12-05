#pragma once
#include "Enemy.h"
class Soldier :
	public Enemy
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual void Set_Collision(Obj* _opponent, DIR::ID _dir);
public:
	void Set_Class(SOLDIER::CLASS _sol) { sol_Class = _sol;  }

public:
	const SOLDIER::CLASS& Get_Class() const { return sol_Class; }

protected:
	virtual void	State_Machine();

protected:
	SOLDIER::CLASS sol_Class;
};

