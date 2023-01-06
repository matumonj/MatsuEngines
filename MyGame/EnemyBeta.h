#pragma once
#include "Enemy.h"
#include"Object3d.h"
#include "Particle.h"
#include"Player.h"

class EnemyBeta :
	public Enemy
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	EnemyBeta();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary> 
	~EnemyBeta() override;

public:
	//������
	void Initialize() override;

	//�X�V����
	void Update() override;

	//�`�揈��
	void Draw() override;

	//���S����
	void Death() override;

	//�U����̃N�[���^�C������
	void AttackCoolTime() override;

	//Fbx�̃A�j���[�V��������
	void FbxAnimationControl() override;

	void EnemyHPDraw() override;

	void Move() override;
	void Smoke(bool& createf) override;
private:
	void HPFrameInit() override;

	void DamageParticleSet();

	XMFLOAT3 GetHandPos();

private:
	bool nowAttack;
	bool isendtime;
	std::unique_ptr<Object3d> Sword;
	int rand_Attacktype;
	int hindex;

	enum AttackNumber
	{
		IDLE,
		THROWRROCK,
		SIDEWAY,
	} attackNum;

	int attackindex;

	XMMATRIX AttackHand_Right;
	XMFLOAT3 RightHandPos;
private:
	std::unique_ptr<Object3d> ThrowRockObj;

	enum ThrowMotion
	{
		NON,
		PICK,
		THROW
	} tmotion;

	XMFLOAT3 RockPos;
	std::unique_ptr<Particle> DestRock;
	bool destF;
	bool turnoffdrawF;

	void AttackCol_Rock();
	void AttackCol_Sideway();
	void PickRock();
	void AnimationContol(AnimationState name, int animenumber, double speed, bool loop);
	void FbxAnimationControls(AnimationState motiontype, int number);

	void AttackMotion();
};
