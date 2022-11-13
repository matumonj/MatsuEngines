#pragma once
#include"Object3d.h"
#include"Model.h"
#include "Enemy.h"
#include"Player.h"
#include"Texture.h"
#include"Sprite.h"
class BossEnemy :
    public Enemy
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    BossEnemy();
   // BossEnemy();
    /// <summary>
    /// デストラクタ
    /// </summary> 
    ~BossEnemy();

private:
    std::unique_ptr<f_Object3d>GigaBossObj;
    //
    Texture* SearchPlayerTexture = nullptr;
public:
    //  void Action(Player* player);
      /// <summary>
      /// 初期化
      /// </summary>
    void Initialize(DebugCamera* camera)override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(DebugCamera* camera)override;

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw()override;

    void AttackCoolTime()override;
    void FbxAnimationControl()override;
    
    void Death()override;
    //bool GetArmonGround() { return ArmonGround; }
private:
    bool sf;
    bool nowAttack;
    bool nowDeath;
    float defaulttime = 1.8f;
    bool DeathFlag;
private:
    void DamageParticleSet();

private:
    bool SlashF;
    bool SlashF2;
    int hand=21;
    float et;
    const int ParticleSize = 20;
private:
    enum NowAttackMotion {
        NON,
        ROAR,
        NORMAL,
        MAGIC,
        EVASION,
        FALTER
    };
    NowAttackMotion nowMotion = NON;
    void SetMotion(bool &motionStartJudg, NowAttackMotion motion, float actionStartTime, float actionEndTime);
private:
    // bool GetFolatc() { return folatc; }
    float NormalAttackTime_Start;
    float NormalAttackTime_End;
    float MagicAttackTime_Start;
    float MagicAttackTime_End;
    float EvaTime_End;
   
    bool isendtime;
    int particleLife = 320;
    XMFLOAT3 particlePos;
    XMFLOAT3 WeaponRot;
};

