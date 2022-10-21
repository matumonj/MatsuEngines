#pragma once
#include "SwordBase.h"
#include"ParticleManager.h"
#include"Texture.h"
class SmallSword :
    public SwordBase
{
public:
    ~SmallSword();
public:
    void Initialize(DebugCamera* camera)override;
    void Update(DebugCamera* camera)override;
    void Draw()override;

private:
    enum AttackMotion {
        NON,
        FIRESPHERE,
        BLIZZARD
    };
    AttackMotion attackMotion;
    DirectX::XMFLOAT3 EaseTime;
    DirectX::XMFLOAT3 magicSpherePos;
    ParticleManager* pMan;
    void MagicAttack();

    /*------------*/
    /*    –‚–@•X   */
    /*------------*/
    class Blizzard {
    private:
        std::unique_ptr<Object3d>IceCrystalObj;
        std::unique_ptr<Texture>IceMagicTex;
    private:
        bool CollideEnemy;
        bool ActFlag;
        int IceExistence = 0;
        float DestTime;
        float DestAlpha=1.0f;
        float TexAlpha=1.0f;
        float EaseTime;
        XMFLOAT3 IcePos;
        XMFLOAT3 IceScl;
        XMFLOAT3 TexScl;
    public:
        void Init(DebugCamera* camera);
        void Updata(DebugCamera* camera);
        void Draw();

        void SetActFlag(bool f) { ActFlag = f; }
    private:
        enum Phase
        {
            NON,
            SHOTMAGICSPHERE,
            ACTIVE,
            DEST
        }phase=NON;
        void DestParamReset();
    };

    /*----------------*/
    /*  –‚–@ŽaŒ‚(”ÍˆÍ)  */
    /*----------------*/

    class Slash {
    private:
        std::unique_ptr<Texture>InpactTex[5];
        bool CollideEnemy;
        bool ActFlag;
        //int IceExistence = 0;
        float Alpha[5] = { 1.0f };
        float EaseTime;
        XMFLOAT3 inpactTexPos;
        XMFLOAT3 inpactTexScl;

        int rotCorrection_Value[5];
        int posCorrection_Value[5];
    public:
        void Init(DebugCamera* camera);
        void Updata(DebugCamera* camera);
        void Draw();

        void SetActFlag(bool f) { ActFlag = f; }
    private:
        enum Phase
        {
            NON,
            SHOTMAGICSPHERE,
            ACTIVE,
            DEST
        }phase = NON;
    };


    std::unique_ptr<Slash>SlashArea;
    std::unique_ptr<Blizzard> Bliz;
};

