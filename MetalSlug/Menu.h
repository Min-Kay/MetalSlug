#pragma once
#include "Scene.h"
class Menu :
	public Scene
{
public:
	Menu() {}
	~Menu() { Release(); }
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

private:
	DWORD	m_LogoTimer = GetTickCount();
	bool	isShow = false;
};

