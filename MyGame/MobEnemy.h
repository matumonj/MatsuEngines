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
    /// <summary>
    /// �f�X�g���N�^
    /// </summary> 
    ~MobEnemy();

public:
    //������
    void Initialize( DebugCamera* camera)override;

    //�X�V����
    void Update(DebugCamera* camera)override;

    //�`�揈��
    void Draw()override;
  
    //���S����
    void Death()override;

    //�U����̃N�[���^�C������
    void AttackCoolTime()override;

    //Fbx�̃A�j���[�V��������
    void FbxAnimationControl()override;

    void DamageTexUpdate(DebugCamera* camera)override;
private:
    void DamageParticleSet();
private:
    bool SlashF;
    bool SlashF2;
    const int ParticleSize = 20;
    //OBBCollision* colObb;
   
public:
   // bool GetFolatc() { return folatc; }
    bool isendtime;
    int particleLife = 320;
    XMFLOAT3 particlePos;
};

