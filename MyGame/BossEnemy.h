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

    Enemy* mob;
    //オブジェクトのインスタンス
    Object3d* MobObject = nullptr;
    //モデルのインスタンス
    Model* MobModel = nullptr;
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

public:
    void FieldCollision(DebugCamera* camera);

    bool DeathFlag;
private:
    bool nowAttack;
    bool nowDeath;
    float defaulttime = 1.8f;

private:
    void DamageParticleSet();
};

