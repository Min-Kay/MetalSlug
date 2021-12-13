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

	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir) {};
public:
	void Set_Class(SOLDIER::CLASS _sol) { sol_Class = _sol;  }

public:
	const SOLDIER::CLASS& Get_Class() const { return sol_Class; }
	
private:
	void	Anim_Idle(HDC _hdc);
	void	Anim_Move(HDC _hdc);
	void	Anim_Die(HDC _hdc);

protected:
	virtual void	State_Machine();
	virtual void	Check_Hp();
	virtual void	Gravity(); 
	virtual void Add_HP(int _hp) { if (!isMove || isDead || isDying) return; hp += _hp; }

public:
	void	Change_Anim(ACTION::ID _action) { action = _action; animIndex = 0; };

protected:
	SOLDIER::CLASS sol_Class = SOLDIER::SERGENT;
	DIR::ID	onlySide = DIR::RIGHT;

	bool	ranAway = false;
	bool	isJump =false ;
	float	jumpForce = 0.f;
	float	jumpTime = 0.f;
	bool	jumping = false;

	int		idleNum = 0;

	bool	falling = false;

	bool	attack = false;
	bool	isHolding = false;
	bool	isHold = false;
	bool	isFiring = false;
	DWORD	fireTime = GetTickCount();
};

