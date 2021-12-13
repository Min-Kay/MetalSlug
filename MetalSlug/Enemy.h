#pragma once
#include "Obj.h"
class Enemy :
	public Obj
{
public:
	virtual void Initialize()PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hdc) PURE;
	virtual void Release()PURE;

	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir) {}
public:
	void	Set_Move(bool _move) { isMove = _move; }
	void	Set_CollMode(bool _coll) { canCollision = _coll; }
	
public:
	const ACTION::ID& Get_Action() const { return action;  }
	const ENEMY::ID& Get_EnemyID() const { return enemy_id; }
	const bool& Get_CollMode() const { return canCollision; }
	const bool& Get_CollAttack() const { return coll_Attack; }
	
protected:
	ENEMY::ID enemy_id = ENEMY::SOLDIER;
	ACTION::ID action = ACTION::IDLE;
	float	speed = 0.f;

	bool	isMove =false;
	bool	coll_Attack =false;

	bool	canCollision = false;
};

