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
    BossEnemy();
   // BossEnemy();
    /// <summary>
    /// �f�X�g���N�^
    /// </summary> 
    ~BossEnemy();

private:
    std::unique_ptr<f_Object3d>GigaBossObj;

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

    void AttackCoolTime()override;
    void FbxAnimationControl()override;
    
    void Death()override;
    void DamageTexUpdate(DebugCamera* camera)override;
private:
    bool gigantic=true;
    bool sf;
    bool nowAttack;
    bool nowDeath;
    float defaulttime = 1.8f;
    bool DeathFlag;
private:
    void SummonGigaBoss(DebugCamera* camera);
    void DamageParticleSet();
private:
    bool SlashF;
    bool SlashF2;
   float alpha = 1;
    const int ParticleSize = 20;
public:
    // bool GetFolatc() { return folatc; }
    bool isendtime;
    int particleLife = 320;
    XMFLOAT3 particlePos;
};

