#pragma once
#include "ObjectManager.h"
#include<memory>
#include"Texture.h"
class GigaBossEnemy :
    public ObjectManager
{
public:

private:
    std::unique_ptr<Texture>FissureTex;

    enum Attack {
        NON,
        BEAM,
        SLAM,
        NORMAL
    };
    float f_time;
    Attack nowAttack;

    float disrad;
    float fissureAlpha;
    bool stopanimation;
    bool ArmonGround;
public:
    void Initialize(DebugCamera* camera)override;
    void Update(DebugCamera* camera)override;
    void Draw()override;

    static GigaBossEnemy* GetInstance();
private:
    void fbxtimeControl();
    void FissureTexUpdate(DebugCamera* camera);
};

