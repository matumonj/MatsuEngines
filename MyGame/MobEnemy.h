#pragma once
#include"Object3d.h"
#include"Model.h"
#include "Enemy.h"
#include"Player.h"
#include"Texture.h"
#include"Sprite.h"
class MobEnemy :
    public Enemy
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    MobEnemy();
    /// <summary>
    /// デストラクタ
    /// </summary> 
    ~MobEnemy();

public:
    //初期化
    void Initialize( DebugCamera* camera)override;

    //更新処理
    void Update(DebugCamera* camera)override;

    //描画処理
    void Draw()override;
  
    //死亡処理
    void Death()override;

    //攻撃後のクールタイム挙動
    void AttackCoolTime()override;

    //Fbxのアニメーション制御
    void FbxAnimationControl()override;

    void DamageTexUpdate(DebugCamera* camera)override;
private:
    void DamageParticleSet();
private:
    bool SlashF;
    bool SlashF2;
    const int ParticleSize = 20;
    //OBBCollision* colObb;
   
public:
   // bool GetFolatc() { return folatc; }
    bool isendtime;
    int particleLife = 320;
    XMFLOAT3 particlePos;
};

