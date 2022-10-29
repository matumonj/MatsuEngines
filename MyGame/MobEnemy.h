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
 
    void FbxAnimeControl(const AnimationState& animestate,bool& Judg, const float nowanimeTime, const float nextanimetime);
    void DamageParticleSet();
private:
    enum AttackType {
        VERTICAL,
        SIDEAWAY
    };
    AttackType atcktype;
    float rand_Attackindex;
    bool atc;
    bool SlashF;
    bool SlashF2;
    int HandIndex;
    const int ParticleSize = 20;
    //OBBCollision* colObb;
   
public:
   // bool GetFolatc() { return folatc; }
    bool isendtime;
    int particleLife = 320;
    XMFLOAT3 particlePos;
    std::unique_ptr<Object3d>Sword;
    XMFLOAT3 swordrot;
};

