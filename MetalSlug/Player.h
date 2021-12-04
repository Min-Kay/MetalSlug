#pragma once
#include "Obj.h"
class Player :
	public Obj
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	void	KeyInput();
	void	Jump();
	void	Valid();

private:
	void	Anim_Idle(HDC _hdc);
	void	Anim_Spawning(HDC _hdc);
	void	Anim_Moving(HDC _hdc);
	void	Anim_Attacking(HDC _hdc);
	void	Anim_Dying(HDC _hdc);

public:
	virtual void Set_Collision(Obj* _opponent, DIR::ID _dir) override;

private:
	ACTION::ID	action;

	float	speed;
	bool	isJump;
	float	jumpTime;
	float	jumpY;
	float	jumpForce;

	bool	isValid;
	DWORD	validTimer;

	DWORD	animTimer;
	int		animValidIndex;
};

