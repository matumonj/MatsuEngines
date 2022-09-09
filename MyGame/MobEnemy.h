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
    /// �R���X�g���N�^
    /// </summary>
    MobEnemy();
 //   MobEnemy();
    /// <summary>
    /// �f�X�g���N�^
    /// </summary> 
    ~MobEnemy();

private:

    //�I�u�W�F�N�g�̃C���X�^���X
    //Object3d* MobObject = nullptr;
   
    //
public:

    Object3d* colBox;
  //  void Action(Player* player);
    /// <summary>
    /// ������
    /// </summary>
    void Initialize( DebugCamera* camera)override;

    /// <summary>
    /// �X�V����
    /// </summary>
    void Update(DebugCamera* camera)override;

    /// <summary>
    /// �`�揈��
    /// </summary>
    void Draw()override;

    /// <summary>
    /// �������
    /// </summary>
    void Finalize()override;
  
    void Death()override;
    void AttackCoolTime()override;
    void FbxAnimationControl()override;
private:
    bool deadtrigger;
    bool SearchPlayerFlag = false;
    float GetDistance() { return distance; }
    //�ڒn�t���O
    bool onGround = true;
    //�����x�N�g��
    XMVECTOR fallV;
    
private:
   
    bool AttackFlag;
   bool DeathFlag;
private:
    bool nowAttack;
    float alpha=1;
public:
   // bool GetFolatc() { return folatc; }
    bool isendtime;
};

