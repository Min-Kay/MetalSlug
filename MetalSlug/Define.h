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
#define		PLAYER_COLOR	RGB(75, 169, 218)

extern		HWND			g_hwnd;
