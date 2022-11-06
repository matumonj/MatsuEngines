#pragma once
#include "SwordBase.h"
#include"ParticleManager.h"
#include"Texture.h"
#include"CollisionPrimitive.h"
class SmallSword :
    public SwordBase
{
public:
    ~SmallSword();
public:
    void Initialize(DebugCamera* camera)override;
    void Update(DebugCamera* camera)override;
    void Draw()override;

    bool GetCameraTargetEnemy() { return CameraTargetEnemy; }
    void SetCameraTargetEnemy(bool f) { CameraTargetEnemy = f; }
private:
    enum AttackMotion {
        NON,
        FIRESPHERE,
        BLIZZARD
    };
    const float Damage_Value = 2.0f;
    const float Damage_Value_S = 3.0f;
    const float CoolTime_Value = 360.0f;
    const float CoolTime_Value_S = 180.0f;

    AttackMotion attackMotion;
    DirectX::XMFLOAT3 EaseTime;
    DirectX::XMFLOAT3 magicSpherePos;
    ParticleManager* pMan;
    void MagicAttack();
    DirectX::XMFLOAT3 Correction;
    bool CameraTargetEnemy;
    XMFLOAT3 OldCameraTargetPos;
    XMFLOAT3 OldTargetEnemyPos;
    XMFLOAT3 CameraPos;
    float EaseTime_CT;
    bool ReturnMoveCameraTarget;
    /*------------*/
    /*    ���@�X   */
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
    public:
        enum Phase
        {
            NON,
            SHOTMAGICSPHERE,
            ACTIVE,
            DEST
        };
        Phase Getphase() { return phase; }
    private:
      
        Phase phase=NON;

        void DestParamReset();
    };

    /*----------------*/
    /*  ���@�a��(�͈�)  */
    /*----------------*/

    class Slash {
    private:
        static const int TexNum = 15;
        std::unique_ptr<Texture>InpactTex[TexNum];
        std::unique_ptr<Object3d>HolyObj;
        ParticleManager* pMan[2];
        bool CollideEnemy;
        bool ActFlag;
        //int IceExistence = 0;
        
        float EaseTime;
        XMFLOAT3 inpactTexPos;
        XMFLOAT3 inpactTexScl;
        XMFLOAT3 HolyScl;
       
        bool next;
        int rotCorrection_Value[TexNum];
        int posCorrection_Valuex[TexNum];
        int posCorrection_Valuey[TexNum];
        int posCorrection_Valuez[TexNum];
        XMFLOAT3 TexScale[TexNum];
        float Alpha[TexNum] = { 1.0f };
        int slashCount[TexNum] = {0};
    public:
        void Init(DebugCamera* camera);
        void Updata(DebugCamera* camera);
        void Draw();

        void SetActFlag(bool f) { ActFlag = f; }
    public:
        enum Phase
        {
            NON,
            SHOTMAGICSPHERE,
            ACTIVE,
            DEST
        };
        Phase Getphase() { return phase; }
    private:
       Phase phase = NON;
    };


 /*----------------*/
 /*  �r�[��(����)  */
 /*----------------*/

    class Beam {
    private:
        std::unique_ptr<Object3d>HolyObj;
        ParticleManager* pManBeam;

        bool CollideEnemy;
        bool ActFlag;
        
        float EaseTime;
        XMFLOAT3 HolyScl;
        XMFLOAT3 HolyRot;
        float Alpha = { 1.0f };
       
        Point targetEnemyPoints;
        Line2D BeamLine;
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

    std::unique_ptr<Beam>beam;
    std::unique_ptr<Slash>SlashArea;
    std::unique_ptr<Blizzard> Bliz;
};

