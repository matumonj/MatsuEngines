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
    MobEnemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence);
 //   MobEnemy();
    /// <summary>
    /// デストラクタ
    /// </summary> 
    ~MobEnemy();

private:
    BehaviorTree behavior;

    //オブジェクトのインスタンス
    //Object3d* MobObject = nullptr;
   
    //
    //
    Texture* SearchPlayerTexture = nullptr;
public:

    Object3d* colBox;
  //  void Action(Player* player);
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize( DebugCamera* camera)override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(DebugCamera* camera)override;

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw()override;

    /// <summary>
    /// 解放処理
    /// </summary>
    void Finalize()override;
  
    void Walk()override;

    void Stop()override;

    void Follow()override;
    void Death()override;
    void Attack()override;
    void AttackCoolTime()override;
    void FbxAnimationControl()override;
private:
    bool deadtrigger;
    bool SearchPlayerFlag = false;
    float GetDistance() { return distance; }
    //接地フラグ
    bool onGround = true;
    //落下ベクトル
    XMVECTOR fallV;
    
private:
   
    bool AttackFlag;
    float AttackTime;
    bool DeathFlag;
  //  float DeathTime;
private:
    bool nowAttack;
    bool nowDeath;
public:
    bool isendtime;
};

