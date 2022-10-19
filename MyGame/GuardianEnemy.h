#pragma once
#include "Enemy.h"
class GuardianEnemy :
    public Enemy
{
private:

public:
    //初期化
    virtual void Initialize(DebugCamera* camera)override=0;

    //更新処理
    virtual void Update(DebugCamera* camera)override=0;

    //描画処理
    virtual void Draw()override=0;

    //死亡処理
    virtual void Death()override=0;

    //攻撃後のクールタイム挙動
    virtual void AttackCoolTime()override=0;

    //Fbxのアニメーション制御
    virtual void FbxAnimationControl()override=0;

    virtual void DamageTexUpdate(DebugCamera* camera)override=0;
};


/*さすがのこの分ファイル作るのは探すの大変だから一つにまとめ*/
/*---------------*/
/*     赤宝箱     */
/*---------------*/
class GuardianRed :
    public GuardianEnemy
{
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

    void DamageTexUpdate(DebugCamera* camera)override;
};

/*---------------*/
/*     青宝箱     */
/*---------------*/
class GuardianBlue :
    public GuardianEnemy
{
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

    void DamageTexUpdate(DebugCamera* camera)override;
};

/*---------------*/
/*     緑宝箱     */
/*---------------*/
class GuardianGreen :
    public GuardianEnemy
{
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

    void DamageTexUpdate(DebugCamera* camera)override;
};

/*---------------*/
/*    黄色宝箱    */
/*---------------*/
class GuardianYellow :
    public GuardianEnemy
{
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

    void DamageTexUpdate(DebugCamera* camera)override;
};
