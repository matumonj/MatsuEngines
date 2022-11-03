#pragma once
#include"Object3d.h"
#include"Model.h"
#include "Enemy.h"
#include"Player.h"
#include"Texture.h"
#include"Sprite.h"

class EnemyAlpha :
    public Enemy
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    EnemyAlpha();
    /// <summary>
    /// �f�X�g���N�^
    /// </summary> 
    ~EnemyAlpha();

public:
    //������
    void Initialize(DebugCamera* camera)override;

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
    bool DeathFlag;
    int hindex;
private:
    bool nowAttack;
    float alpha = 1;
    const int ParticleSize = 20;
public:
    // bool GetFolatc() { return folatc; }
    bool isendtime;
    int particleLife = 320;
    XMFLOAT3 particlePos;

    std::unique_ptr<Object3d>Sword;
};

