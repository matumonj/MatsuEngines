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
	void Initialize(DebugCamera* camera) override;

	//�X�V����
	void Update(DebugCamera* camera) override;

	//�`�揈��
	void Draw() override;

	//���S����
	void Death() override;

	void DamageParticleSet();

	void EnemyHPDraw()override;
private:
	//�U����̃N�[���^�C������
	void AttackCoolTime() override;

	//Fbx�̃A�j���[�V��������
	void FbxAnimationControl() override;

	void FbxAnimeControl(const AnimationState& animestate, bool& Judg, float nowanimeTime, float nextanimetime);

	void OBBSetParam();
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

	float NowFrameX;
	float OldFrameX;
	float FrameScalingETime;
	XMFLOAT2 FrameScl;
public:
	bool isendtime;
	std::unique_ptr<Object3d> Sword;
	std::array<std::unique_ptr<Sprite>, 3>HPFrame;
	XMFLOAT3 swordrot;
};
