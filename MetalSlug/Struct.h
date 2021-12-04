#pragma once

#include "Enum.h"

typedef struct tagInfo
{
	float x;
	float y;
	float cx;
	float cy;
}INFO;

typedef struct DisDir
{
	float dis;
	DIR::ID dir;

	DisDir() {};
	DisDir(float _dis, DIR::ID _dir) : dis(_dis), dir(_dir) { }
}DISDIR;


typedef	struct	tagLinePos
{
	float		fX;
	float		fY;

	tagLinePos()
	{
		ZeroMemory(this, sizeof(tagLinePos));
	}
	tagLinePos(float _X, float _Y)
		: fX(_X), fY(_Y)
	{

	}

}LINEPOS;

typedef struct tagLineInfo
{
	LINEPOS		tLeftPos;
	LINEPOS		tRightPos;

	tagLineInfo() { ZeroMemory(this, sizeof(tagLineInfo)); }
	tagLineInfo(LINEPOS& _tLeft, LINEPOS& _tRight)
		: tLeftPos(_tLeft), tRightPos(_tRight)
	{	}
}LINEINFO;
