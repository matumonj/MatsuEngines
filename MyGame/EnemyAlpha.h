#pragma once
#include"Object3d.h"
#include"Model.h"
#include "Enemy.h"
#include"Player.h"
#include"Texture.h"
#include"Sprite.h"

class EnemyAlpha :
    public Enemy
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    EnemyAlpha();
    /// <summary>
    /// デストラクタ
    /// </summary> 
    ~EnemyAlpha();

public:
    //初期化
    void Initialize(DebugCamera* camera)override;

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

private:
    void DamageParticleSet();
private:
    bool DeathFlag;
    int hindex;
private:
    bool nowAttack;
public:
    bool isendtime;
    std::unique_ptr<Object3d>Sword;
};

