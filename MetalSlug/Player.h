#pragma once
#include "Obj.h"
#include "Weapon.h"

class  Player:
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
	virtual void	Gravity();
	void	Valid();
	void	Offset();

private:
	void	Anim_Idle(HDC _hdc);
	void	Anim_Spawning(HDC _hdc);
	void	Anim_Moving(HDC _hdc);
	void	Anim_Jumping(HDC _hdc);
	void	Anim_Dying(HDC _hdc);

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir) override;
	void	Set_BoxCollide(bool _bool) { boxCollide = _bool; }
	void	Set_CollisionY(float _y) { collisionY = _y; }
public:
	void	Set_Weapon(Weapon* _wep);
	void	Check_WeaponState();
	void	Anim_Counter(ANIM::PLAYER _action, int count, float _timer, bool _roop = true, int start = 0);
	void	Set_Dying(DIR::ID _dir);

private:
	ACTION::ID	action;
	DIR::ID		onlySide;

	float	init_CY;

	bool	isStab;

	bool	isDying; 
	bool	isFiring;
	bool	isGrenading;
	bool	isStabbing;
	bool	canRide; 

	float	speed;
	float	walkSpeed;
	float	sitSpeed;

	bool	isJump;
	float	jumpTime;
	float	jumpY;
	bool	jumping;
	float	jumpForce;

	float	collisionY;
	bool	boxCollide;

	float   m_iOffSetX;
	float   m_iOffSetY;

	bool	isValid;
	DWORD	validTimer;
	DWORD	validTick;

	DWORD	animTimer;
	int		animValidIndex;

	DWORD	dyingTimer;

	int		animIndexs[ANIM::PLAYER_END]{};
	int		animIndexPos[ANIM::PLAYER_END]{0,2,10,3,8,9,13,5,6,7,11,4,12};

	
	Weapon* weapon;
};

