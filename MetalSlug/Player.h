#pragma once
#include "Obj.h"
#include "Weapon.h"

class  Player:
	public Obj
{
public:
	Player();
	virtual ~Player();
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

private:
	void	Anim_Idle(HDC _hdc);
	void	Anim_Moving(HDC _hdc);
	void	Anim_Jumping(HDC _hdc);
	void	Anim_Dying(HDC _hdc);

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir) override {};
	virtual void Set_Dying();

public:
	void	Set_Weapon(Weapon* _wep);
	void	Delete_Weapon() { SAFE_DELETE(weapon) }
	void	Check_WeaponState();
	void	Anim_Counter(ANIM::PLAYER _action, int count, float _timer, bool _roop = true, int start = 0);

private:
	ACTION::ID	action = ACTION::IDLE;
	DIR::ID		onlySide = DIR::RIGHT;

	float	init_CY = 0.f;

	bool	isStab = false;

	bool	isFiring = false;
	bool	isGrenading = false;
	bool	isStabbing = false;
	bool	canRide = false;

	float	speed = 0.f;
	float	walkSpeed = 0.f;
	float	sitSpeed = 0.f;

	bool	isJump = false;
	float	jumpTime = 0.f;
	float	jumpY = 0.f;
	bool	jumping = false;
	float	jumpForce = 0.f;

	bool	superValid = false;
	bool	isValid = false;
	DWORD	validTimer = GetTickCount();
	DWORD	validTick = GetTickCount();

	DWORD	animTimer = GetTickCount();
	int		animValidIndex = 0;

	DWORD	dyingTimer = GetTickCount();

	int		animIndexs[ANIM::PLAYER_END]{};
	int		animIndexPos[ANIM::PLAYER_END]{0,2,10,3,8,9,13,5,6,7,11,4,12};

	Weapon* weapon = nullptr;
	bool sit = false;
};

