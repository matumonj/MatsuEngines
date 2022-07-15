#pragma once
#include"Object3d.h"
#include"Model.h"
#include "Enemy.h"
#include"Player.h"
#include"Texture.h"
#include"Sprite.h"
#include"f_Object3d.h"
#include"f_Model.h"
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

    Enemy* mob;
    //オブジェクトのインスタンス
    //Object3d* MobObject = nullptr;
    f_Object3d* MobObject = nullptr;
    //モデルのインスタンス
    //Model* MobModel = nullptr;
    f_Model* MobModel = nullptr;
    //
    //
    Texture* SearchPlayerTexture = nullptr;
public:
  //  void Action(Player* player);
    /// <summary>
    /// 初期化
    /// </summary>
    bool Initialize( DebugCamera* camera)override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(XMFLOAT4 color, DebugCamera* camera)override;

    void CollisionField(DebugCamera* camera)override;
    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw()override;

    /// <summary>
    /// 解放処理
    /// </summary>
    void Finalize()override;
    /// <summary>
    /// 
    /// </summary>
    /// 
    void EnemySearchPlayer(Player* player)override;


    float Distance(Player* player)override;

private:
    
    bool SearchPlayerFlag = false;
    float GetDistance() { return distance; }
    //接地フラグ
    bool onGround = true;
    //落下ベクトル
    XMVECTOR fallV;
    
public:
};

