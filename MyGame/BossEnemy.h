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
    BossEnemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence);
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

    /// <summary>
    /// 解放処理
    /// </summary>
    void Finalize()override;
    void Walk()override;

    void Stop()override;

    void Follow()override;

    void Death()override;
private:

    bool SearchPlayerFlag = false;
    float GetDistance() { return distance; }
    //接地フラグ
    bool onGround = true;
    //落下ベクトル
    XMVECTOR fallV;
public:
    void FieldCollision(DebugCamera* camera);
};

