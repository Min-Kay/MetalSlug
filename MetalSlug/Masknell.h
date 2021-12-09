#pragma once
#include "Enemy.h"
class Masknell :
    public Enemy
{
public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC _hdc) override;
    virtual void Release() override;

public:
    virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);
    void State_Machine();
    virtual void Gravity();

private:
    bool    isTurning;
    bool    turned;
 

    DWORD   attackTime;

};

