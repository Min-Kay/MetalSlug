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
	virtual void Set_Collision(Obj* _opponent, DIR::ID _dir) { isDead = true; }

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
	virtual void	Update_Rect();

public:
	const INFO& Get_Info() const { return info; }
	const RECT& Get_Rect() const { return rect; }
	const bool& Get_Dead() const { return isDead; }
	const RENDER::ID& Get_RenderId() const { return render; }

protected:
	HDC		drawingDC;
	INFO	info;
	RECT	rect;
	DIR::ID dir; 
	RENDER::ID render;
	bool	isDead;
};

