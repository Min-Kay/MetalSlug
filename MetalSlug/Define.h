#pragma once
#include "framework.h"

//WINAPI
#define		WINCX			800
#define		WINCY			600

//MACRO
#define		SAFE_DELETE(p)	if(p){ delete p; p = nullptr; }

//COMMON
#define		PURE			= 0

//OBJECT
#define		OBJ_DEFAULT		0 
#define		OBJ_DEAD		1

#define		FALL_DOWN		10.f

// MATH
#define		PI					3.141592f

// KEY
#define		VK_MAX				0xff


// Blt Color 
#define		PLAYER_COLOR	RGB(86, 177, 222)
#define		ITEM_COLOR		 RGB(0, 248, 0)
#define		MAP_COLOR		RGB(248,0, 248)

// Key Setting
#define		VK_ATTACK		'X'
#define		VK_JUMP			'Z'
#define		VK_GRENADE		'A'
#define		VK_INTERACTION	'S'

extern		HWND			g_hwnd;
