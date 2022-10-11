#pragma once
#include"CollisionPrimitive.h"
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
    XMMATRIX GetMatrot() {return m_Object->ExtractRotationMat(); }
protected:
    int Damage;
    int CoolTime;
public:
    int GetDamage() { return Damage; }
    int GetCoolTime() { return CoolTime; }
};

