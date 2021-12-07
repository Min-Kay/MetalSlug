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

	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);
public:
	void Set_Class(SOLDIER::CLASS _sol) { sol_Class = _sol;  }
	void Jump();

public:
	const SOLDIER::CLASS& Get_Class() const { return sol_Class; }
	
private:
	void	Anim_Idle(HDC _hdc);
	void	Anim_Move(HDC _hdc);
	void	Anim_Jump(HDC _hdc);
	void	Anim_Die(HDC _hdc);

protected:
	virtual void	State_Machine();

protected:
	SOLDIER::CLASS sol_Class;
	DIR::ID	onlySide;

	bool	ranAway;
	bool	isJump;
	float	jumpForce;
	float	jumpTime; 
	bool	jumping;

	int		idleNum;

	bool	isDying;


	DWORD	fireTime;
};

