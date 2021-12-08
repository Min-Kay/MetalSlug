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

public:
	void	Change_Anim(ACTION::ID _action) { action = _action; animIndex = 0; };

private:
	bool	attacking;
	
	DWORD	attackTime;

	bool	falling; 

	bool	isJump;
	float	jumpTime;
	float	jumpY;
	bool	jumping;
	float	jumpForce;
	
};

