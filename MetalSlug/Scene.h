#pragma once
#include "framework.h"
#include "Include.h"

class Scene
{
public:
	Scene() {
		drawingDC = NULL; 
		stretchDC = NULL;
	};
	virtual ~Scene() {};

public:
	virtual void Initialize()PURE;
	virtual void Update()PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hdc)PURE;
	virtual void Release()PURE;

protected:
	HDC drawingDC;
	HDC stretchDC;
};

