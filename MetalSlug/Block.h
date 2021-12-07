#pragma once
#include "Obj.h"
class Block :
    public Obj
{
public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC _hdc) override;
    virtual void Release() override;
    virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir) {  }

public:
    bool    haveGravity;
};

