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
    MobEnemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence);
 //   MobEnemy();
    /// <summary>
    /// �f�X�g���N�^
    /// </summary> 
    ~MobEnemy();

private:
    BehaviorTree behavior;

    //�I�u�W�F�N�g�̃C���X�^���X
    //Object3d* MobObject = nullptr;
   
    //
    //
    Texture* SearchPlayerTexture = nullptr;
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
private:
    bool deadtrigger;
    bool SearchPlayerFlag = false;
    float GetDistance() { return distance; }
    //�ڒn�t���O
    bool onGround = true;
    //�����x�N�g��
    XMVECTOR fallV;
    
public:
};

