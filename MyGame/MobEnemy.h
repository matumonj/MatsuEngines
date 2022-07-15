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

    Enemy* mob;
    //�I�u�W�F�N�g�̃C���X�^���X
    //Object3d* MobObject = nullptr;
    f_Object3d* MobObject = nullptr;
    //���f���̃C���X�^���X
    //Model* MobModel = nullptr;
    f_Model* MobModel = nullptr;
    //
    //
    Texture* SearchPlayerTexture = nullptr;
public:
  //  void Action(Player* player);
    /// <summary>
    /// ������
    /// </summary>
    bool Initialize( DebugCamera* camera)override;

    /// <summary>
    /// �X�V����
    /// </summary>
    void Update(XMFLOAT4 color, DebugCamera* camera)override;

    void CollisionField(DebugCamera* camera)override;
    /// <summary>
    /// �`�揈��
    /// </summary>
    void Draw()override;

    /// <summary>
    /// �������
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
    //�ڒn�t���O
    bool onGround = true;
    //�����x�N�g��
    XMVECTOR fallV;
    
public:
};

