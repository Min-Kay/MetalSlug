#pragma once
#include "Enemy.h"
class Arabian :
	public Enemy
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

private:
	void	Anim_Idle(HDC _hdc);
	void	Anim_Move(HDC _hdc);
	void	Anim_Die(HDC _hdc);

protected:
	virtual void	State_Machine();
	virtual void	Check_Hp();

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);
	virtual void Gravity();
	virtual void Add_HP(int _hp) { if (!isMove || isDead || isDying) return; hp += _hp; }

public:
	void	Change_Anim(ACTION::ID _action) { action = _action; animIndex = 0; };

private:
	bool	attacking =  false;
	
	DWORD	attackTime = GetTickCount();

	bool	falling = false; 

	bool	isJump = false;
	float	jumpTime = 0.f;
	float	jumpY = 0.f;
	bool	jumping = false;
	float	jumpForce = 40.f;
	
};

