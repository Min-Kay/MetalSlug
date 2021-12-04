#pragma once
#include "Scene.h"
class Title :
	public Scene
{
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;
};

