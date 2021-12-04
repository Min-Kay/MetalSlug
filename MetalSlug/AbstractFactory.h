#pragma once

#include "Obj.h"
#include "Enum.h"

template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory(){}
	~CAbstractFactory(){}

public:
	static Obj*		Create()
	{
		Obj*		pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static Obj*		Create(float _fX, float _fY, DIR::ID eDir = DIR::RIGHT)
	{
		Obj*		pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Dir(eDir);
		pObj->Update_Rect();

		return pObj;
	}


};

