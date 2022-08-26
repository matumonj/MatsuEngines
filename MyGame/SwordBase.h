#pragma once
#include "ObjectManager.h"
class SwordBase :
    public ObjectManager
{
public:
     SwordBase();
    virtual ~SwordBase();
public:
    virtual void Initialize(DebugCamera* camera)override;
    virtual void Update(DebugCamera* camera)override;
    virtual void Draw()override;
protected:
    float Damage;
    float CoolTime;

public:
    float GetDamage() { return Damage; }
    float GetCoolTime() { return CoolTime; }
};

