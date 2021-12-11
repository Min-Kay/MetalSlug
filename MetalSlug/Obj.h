#pragma once

#include "framework.h"
#include "Include.h"

class Obj
{

public:
	virtual void Initialize()PURE;
	virtual int Update()PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hdc)PURE;
	virtual void Release()PURE;

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir) { isDead = true; }
	virtual void Gravity();
	void	Check_Line_Collision();

public:
	void	Set_Dir(DIR::ID _dir) { dir = _dir; }
	void	Set_Info(INFO& _info) { info = _info; }
	void	Set_X(float _x) { info.x = _x; }
	void	Set_Y(float _y) { info.y = _y; }
	void	Add_X(float _x) { info.x += _x; }
	void	Add_Y(float _y) { info.y += _y; }
	void	Set_Pos(float _x, float _y) { info.x = _x; info.y = _y; }
	void	Set_Size(float _cx, float _cy) { info.cx = _cx; info.cy = _cy; }
	void	Set_Dead(bool _dead) { isDead = _dead; }
	void	Set_ID(OBJ::ID _id) { id = _id; }
	void	Set_HP(int _hp) { hp = _hp; }

	virtual void	Set_Dying() { isDying = true; }
	virtual void	Add_HP(int _add) { hp += _add; }
	virtual void	Update_Rect();

public:
	const INFO& Get_Info() const { return info; }
	const RECT& Get_Rect() const { return rect; }
	const bool& Get_Dead() const { return isDead; }
	const OBJ::ID& Get_ID()	const { return id; }
	const bool& Get_Dying() const { return isDying; }

	const int& Get_HP() const { return hp; }

public:
	void	Set_BoxCollide(bool _bool) { boxCollide = _bool; }
	void	Set_CollisionY(float _y) { collisionY = _y; }

public:
	void	Anim_Counter(int count, float _timer, bool _roop = true, int start = 0);
	void	Check_Screen_Out();

protected:
	HDC		drawingDC;
	HDC		stretchDC;

	int		animIndex = 0;
	DWORD	animTimer;

	INFO	info;
	RECT	rect;
	DIR::ID dir; 
	OBJ::ID	id;
	bool	isDead;
	bool	isDying; 

	int		hp;

	float	collisionY = 0.f;
	bool	boxCollide = false;
};

