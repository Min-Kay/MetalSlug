#pragma once
#include "Weapon.h"
class Shotgun :
    public Weapon
{
    virtual void Initialize() override;
    virtual bool Fire(float _x, float _y, DIR::ID _dir) override;

private:
    bool    isFire;
};

