#pragma once
#include "Obj.h"
class Npc :
	public Obj
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

private:
	void	StateMachine();

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);
	const bool& Get_Ropped() const { return isRopped; }
	void Set_Ropped();
private:
	ACTION::ID action = ACTION::IDLE;
	bool	isRopped =true;
	bool	isUntied = false;

	bool	ranout = false;
	bool	moving = false;

	bool	spawningItem = false;
	bool	spawnTiming = false;
	bool	spawned = false;

	bool	thank = false;
	
	float	speed =0.f;

	float	totalMove = 0.f;

	DWORD	motionDelay = GetTickCount();
	bool	isMotion = false;
};

