#pragma once
#include "Obj.h"
class Block :
    public Obj
{
public:
    virtual void Initialize() PURE;
    virtual int Update() PURE;
    virtual void Late_Update() PURE;
    virtual void Render(HDC _hdc) PURE;
    virtual void Release() PURE;
    virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir) {  }

public:
    void Set_Gravity(bool _bool) { haveGravity = _bool; }

public:
    bool    haveGravity = true;
};

