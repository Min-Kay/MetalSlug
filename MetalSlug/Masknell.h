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
    void State_Machine();
    virtual void Gravity();
    virtual void Add_HP(int _hp) { if (isDying || isDead) return; hp += _hp; }

private:
    bool    isTurning;
    bool    turned;
 

    DWORD   attackTime;

};

