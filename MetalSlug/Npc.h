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

private:
	ACTION::ID action;
	bool	isRopped;
	bool	isUntied;

	bool	ranout;
	bool	moving;

	bool	spawningItem;
	bool	spawnTiming;
	bool	spawned;

	bool	thank;
	
	float	speed;

	float	totalMove;

	DWORD	motionDelay;
	bool	isMotion;
};

