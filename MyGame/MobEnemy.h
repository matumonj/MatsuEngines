#pragma once
#include"Object3d.h"
#include"Model.h"
#include "Enemy.h"
#include"Player.h"
#include"Texture.h"
#include"Sprite.h"
#include<array>

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
	~MobEnemy() override;

public:
	//������
	void Initialize() override;

	//�X�V����
	void Update() override;

	//�`�揈��
	void Draw() override;

	//���S����
	void Death() override;

	void Move() override;
	void Smoke(bool& createf) override;

	void DamageParticleSet();

	void EnemyHPDraw() override;
private:
	//�U����̃N�[���^�C������
	void AttackCoolTime() override;

	void HPFrameInit() override;
	//Fbx�̃A�j���[�V��������
	void FbxAnimationControl() override;

	void FbxAnimeControl(const AnimationState& animestate, bool& Judg, float nowanimeTime, float nextanimetime);

	void OBBSetParam();

	void HPFrameScaling();
private:
	enum AttackType
	{
		VERTICAL,
		SIDEAWAY
	};

	AttackType atcktype;
	float imt;
	float rand_Attacktype;
	int HandIndex;

	int attackinter;
public:
	bool isendtime;
	std::unique_ptr<Object3d> Sword;
	XMFLOAT3 swordrot;
};
