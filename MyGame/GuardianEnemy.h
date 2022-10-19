#pragma once
#include "Enemy.h"
class GuardianEnemy :
    public Enemy
{
private:

public:
    //������
    virtual void Initialize(DebugCamera* camera)override=0;

    //�X�V����
    virtual void Update(DebugCamera* camera)override=0;

    //�`�揈��
    virtual void Draw()override=0;

    //���S����
    virtual void Death()override=0;

    //�U����̃N�[���^�C������
    virtual void AttackCoolTime()override=0;

    //Fbx�̃A�j���[�V��������
    virtual void FbxAnimationControl()override=0;

    virtual void DamageTexUpdate(DebugCamera* camera)override=0;
};


/*�������̂��̕��t�@�C�����̂͒T���̑�ς������ɂ܂Ƃ�*/
/*---------------*/
/*     �ԕ�     */
/*---------------*/
class GuardianRed :
    public GuardianEnemy
{
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
};

/*---------------*/
/*     ��     */
/*---------------*/
class GuardianBlue :
    public GuardianEnemy
{
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
};

/*---------------*/
/*     �Ε�     */
/*---------------*/
class GuardianGreen :
    public GuardianEnemy
{
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
};

/*---------------*/
/*    ���F��    */
/*---------------*/
class GuardianYellow :
    public GuardianEnemy
{
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
};
