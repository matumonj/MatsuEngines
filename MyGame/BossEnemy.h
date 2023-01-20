#pragma once
#include"Object3d.h"
#include"Model.h"
#include "Enemy.h"
#include"Player.h"
#include"Texture.h"
#include"Sprite.h"
#include<array>

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
	~BossEnemy() override;

private:
	std::unique_ptr<f_Object3d> GigaBossObj;
	//
	Texture* SearchPlayerTexture = nullptr;

	void BomTexUpda();
	void ResourcesSet();
public:
	//  void Action(Player* player);
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	void AttackCollide();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw() override;

	void AttackCoolTime() override;
	void FbxAnimationControl() override;
	void EnemyHPDraw() override;
	void Death() override;
	void Move() override;
	void Smoke(bool& createf) override;
	//bool GetArmonGround() { return ArmonGround; }
private:
	bool sf;
	bool nowAttack;
	bool nowDeath;
	float defaulttime = 1.8f;
	bool DeathFlag;
private:
	void DamageParticleSet();

	void HPFrameInit() override;


private:
private:
	std::unique_ptr<Sprite> m_BossHP;
	std::unique_ptr<Sprite> m_BossHPFrame;
	std::unique_ptr<Sprite> m_BossHPFrame2;

	XMFLOAT2 BarPos;
	XMFLOAT2 OldBarScl;
	XMFLOAT2 BarScl;
	XMFLOAT2 LevelUpTexPos;
	XMFLOAT2 BarFramePos;
	XMFLOAT2 BarFrameScl;

	void HPGaugeBoss();
private:
	bool SlashF;
	bool SlashF2;
	int hand = 21;
	float et;
	const int ParticleSize = 20;
public:
	void SetMotion(bool& motionStartJudg, NowAttackMotion motion, float actionStartTime, float actionEndTime);
private:
	XMMATRIX handmat_right;
	XMMATRIX handmat_left;

	XMFLOAT3 HandPos_Right;//18
	XMFLOAT3 HandPos_Left;//43
	int hind;
	bool isendtime;
	int particleLife = 320;
	XMFLOAT3 particlePos;
	XMFLOAT3 WeaponRot;
	std::unique_ptr<Object3d> Sword;
	XMFLOAT3 swordrot;

private:
	int attackindex;
		void AnimationContol( int animenumber, double speed, bool loop);
		void FbxAnimationControls(NowAttackMotion motiontype, int number);

		void AttackMotion();
};
