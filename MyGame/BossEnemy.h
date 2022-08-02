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
    /// �R���X�g���N�^
    /// </summary>
    BossEnemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence);
   // BossEnemy();
    /// <summary>
    /// �f�X�g���N�^
    /// </summary> 
    ~BossEnemy();

private:

    Enemy* mob;
    //�I�u�W�F�N�g�̃C���X�^���X
    Object3d* MobObject = nullptr;
    //���f���̃C���X�^���X
    Model* MobModel = nullptr;
    //
    Texture* SearchPlayerTexture = nullptr;
public:
    //  void Action(Player* player);
      /// <summary>
      /// ������
      /// </summary>
    void Initialize(DebugCamera* camera)override;

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
    void Walk()override;

    void Stop()override;

    void Follow()override;

    void Death()override;
private:

    bool SearchPlayerFlag = false;
    float GetDistance() { return distance; }
    //�ڒn�t���O
    bool onGround = true;
    //�����x�N�g��
    XMVECTOR fallV;
public:
    void FieldCollision(DebugCamera* camera);
};

