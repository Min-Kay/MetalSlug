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

	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir) { isDead = true; }
public:
	void	Set_HP(int _hp) { hp = _hp; }
	void	Add_HP(int _hp) { hp += _hp; }
	void	Set_Move(bool _move) { isMove = _move; }
	void	Set_CollMode(bool _coll) { canCollision = _coll; }
	
public:
	const int& Get_HP() const {return hp; }
	const ACTION::ID& Get_Action() const { return action;  }
	const ENEMY::ID& Get_EnemyID() const { return enemy_id; }
	const bool& Get_CollMode() const { return canCollision; }
	const bool& Get_CollAttack() const { return coll_Attack; }
	
public:
	virtual void	Check_Hp() { if (hp <= 0) Set_Dead(true);  }
	
protected:
	ENEMY::ID enemy_id;
	ACTION::ID action;
	int		hp;
	float	speed;

	bool	isMove;
	bool	coll_Attack;
	bool	isDying;

	bool	canCollision;
};

