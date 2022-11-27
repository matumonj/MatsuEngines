#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include"DebugCamera.h"
#include"Input.h"
#include"Player.h"
#include"EnemyState.h"
#include"BossEnemyState.h"
#include"Texture.h"
#include"Destroy.h"
#include"ObjectManager.h"
#include"ParticleManager.h"
#include"DebugTxt.h"
#include<list>
#include"Task.h"
#include"DamageManager.h"
class NodeBase;

/// <summary>
/// �G�L�����̊��N���X
/// </summary>
/// 
class Enemy : public ObjectManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Enemy();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Enemy() override;

public:
	//������
	virtual void Initialize(DebugCamera* camera) override=0;
	//�X�V����
	virtual void Update(DebugCamera* camera) override=0;
	//�`�揈��
	virtual void Draw() override=0;
	//���S����
	virtual void Death() = 0;
	//Fbx����
	virtual void FbxAnimationControl() = 0;
	//
	virtual void Action();

	void isRespawn();

	void DestroyJudg();
protected:
	//�̗͎���
	float EnemyHP = 0.0f; //���݂̗̑�
	float MaxHP = 0.0f; //�ő�l(�ϓ��Ȃ�)
	float OldHP = 0.0f; //�O�t���[�����̗̑�(HUD�̃C�[�W���O�p)
protected:
	float alpha = 1.0f;
	XMFLOAT3 RespawnPos = { 0.0f, 0.0f, 0.0f };
	bool RecvDamagef = false; //
	bool RecvDamagef2 = false; //
	bool RecvAttackHit = false;
	bool DamageParticleCreateF = false; //�U���󂯂�����p�[�e�B�N�������t��g
private:
	bool MoveFlag = false;
	XMFLOAT3 OldPos = { 0.0f, 0.0f, 0.0f };
	int DamageSize = 0;
	XMFLOAT3 DamageTexPos = {};
public:
	//�U���󂯂�����̔���p
	void RecvDamage(int Damage);
	void EnemyPop(int HP);
	virtual void AttackCoolTime() = 0;
	void SetMoveStop(bool f) { movestop = f; }
	bool GetMoveStop() { return movestop; }
	void DamageTexDisplay();
	void DamageTexDisplay_Draw();
protected:
	bool f_AttackFlag = false;
	bool DeathFlag = false;
	bool nowDeath = false;
	bool nowAttack = false;
	bool FalterFlag = false;
	bool SwingFlag = false;
	bool DieFlag = false;
	int cooltime = 0;

	int onGroundTime = 0;
	bool AfterAttack = false;

	int PopCount = 0; //���X�|�[���J�E���g

	bool movestop = false;
protected:
	//FBXTime����̕ϐ�
	float f_time = 0.0f; //���݂̃t���[��
	float start_time = 0.0f; //�����t���[��(0)
	float end_time = 0.0f; //�ŏI�t���[��
	float DeathTime = 0.0f; //���S���̃��[�V�����J�n�t���[��
	float NormalAttackTime = 0.0f;

	XMMATRIX HandMat = {};
	OBB HandSiteOBB = {};
	OBB playerOBB = {};

	bool damageDisF = false;
public:
	enum AnimationState
	{
		NON,
		WALK,
		ATTACK1,
		ATTACK2,
		ATTACK3,
		DEATH,
		ROAR,
		IDLE
	};

	void SetAnimeState(AnimationState state) { animeState = state; }
	int GetEnemyNumber() { return ENumber; }
protected:
	/*blender�̕����A�j���[�V�����ŗ�����S������*/
	AnimationState animeState;
	//�G�̉�]�����p
	float addRotRadians;
	float FollowRotAngleCorrect;

	//�e�A�j���[�V�����̊J�n�^�C��
	float MagicAttackTime = 0.0f;
	float FalterTime = 0.0f;
	float FalterTime_End;
	float RoarTime;
	float RoarTime_End;
	float SwingTime;
	float SwingTime_End;
	float IdleTime;
	float IdleTime_End;
	float SideWalk_LeftTime;
	float SideWalk_LeftTime_End;
	float SideWalk_RightTime;
	float SideWalk_RightTime_End;

	float DeathMotionTime_Start;
	float DeathMotionTime_End;
	float EvaTime = 0.0f;
	float AttackTime;
	float AttackTime_End;

	bool EvaMotionStart = false;
	bool MagicMotionStart = false;
	bool RoarMotionFlag = false;
	bool IdleMotionFlag = false;
	bool DeathMotionFlag = false;
	bool SideWalk_RightMotionFlag = false;
	bool SideWalk_LeftMotionFlag = false;
	/*�Q�b�^�[*/
public:
	float GetRotCorrect() { return FollowRotAngleCorrect; }
	bool GetRecvAttack() { return RecvAttackHit; }
	void SetRecvAttack(bool f) { RecvAttackHit = f; }
	//�U���󂯂�����̔���p
	bool GetRecvDamage() { return RecvDamagef; }
	//�U���󂯂�����̔���p
	bool GetRecvDamage2() { return RecvDamagef2; }
	//�����Ă��邩
	bool GetMoveFlag() { return MoveFlag; }
	//�n�ʐݒu��Ԃ��ǂ���
	bool GetonFlag() { return onGround; }
	//�̗͎���
	float GetHP() { return EnemyHP; };
	//�̗͍ő�l
	float GetMaxHP() { return MaxHP; }
	//FBXTime(���Ƀ��[�V�����ƍU�����[�V�����̊J�n�t���[���擾)
	bool GetDeathTime() { return nowDeath; }

	bool GetAttackTime() { return f_AttackFlag; }
	//���݂̌o�߃t���[��
	float GetFbxTime() { return f_time; }
	//��]�s��擾(OBB�p����Ȃ�����)
	XMMATRIX GetMatrot() { return m_fbxObject->GetMatrot(); }
	//�U���̃N�[���^�C��
	int GetCoolTime() { return cooltime; }

	XMMATRIX GetHandMat() { return HandMat; }
	OBB GetHandSiteMat() { return HandSiteOBB; }

	float GetObjAlpha() { return alpha; }
	/*�Z�b�^�[*/
public:
	float GetRotRadians() { return addRotRadians; }
	void SetRecvDamage(bool f) { RecvDamagef = f; }

	void SetRecvDamage2(bool f) { RecvDamagef2 = f; }
	void SetMoveFlag(bool f) { MoveFlag = f; }

	void SetScale(XMFLOAT3 scale) { Scale = scale; }

	void SetAttackTime(bool f) { if (f_time < NormalAttackTime) { f_AttackFlag = f; } }

	void SetRespawnPos(XMFLOAT3 position) { RespawnPos = position; }
	/*�{�X�U���p �ł���Έڂ��������A�A*/
protected: //�U���̊J�n�ƏI������p
	struct Attack_SE
	{
		bool start;
		bool end;
	};
public:
	enum EnemyNumber
	{
		GOLEM,
		FLOG,
	};
protected:
	int ENumber;
	//�U����
	static const int AtckNum = 7;
	Attack_SE Attack[AtckNum];

public:
	void SetSwingMotion(bool f) { if (f_time < SwingTime) { SwingFlag = f; } }

	void SetFalterMotion(bool f) { if (f_time < FalterTime) { FalterFlag = f; } }
	void SetRoarMotion(bool f) { if (f_time > RoarTime_End) { RoarMotionFlag = f; } }
	void SetDeathMotion(bool f) { if (f_time < DeathMotionTime_Start) { DieFlag = f; } }
	//
	void SetRSideWalkMotion(bool f) { if (f_time > SideWalk_RightTime) { SideWalk_RightMotionFlag = f; } }
	void SetLSideWalkMotion(bool f) { if (f_time < SideWalk_LeftTime) { SideWalk_LeftMotionFlag = f; } }

	//
	void SetIdleMotion(bool f) { if (f_time < IdleTime) { IdleMotionFlag = f; } }
	float GetFalterTime_End() { return FalterTime_End; }
	float GetRoarTime_End() { return RoarTime_End; }
	void SetMagicAttackTime(bool f) { if (f_time < MagicAttackTime) { MagicMotionStart = f; } }
	void SetEvaMotionTime(bool f) { if (f_time < EvaTime) { EvaMotionStart = f; } }

	bool GetAttack_Start(int Num) { return Attack[Num].start; }
	bool GetAttack_End(int Num) { return Attack[Num].end; }

	void SetAttack_Start(int Num, bool f) { Attack[Num].start = f; }
	void SetAttack_End(int Num, bool f) { Attack[Num].end = f; }

	void SetHP(int HP) { this->EnemyHP = HP; }
public: //state�؂�ւ�
	void ChangeState_Mob(EnemyState* state);
	void ChangeState_Boss(BossEnemyState* state);

public: //�U����ޗ�
	enum
	{
		CIRCLE_1,
		CIRCLE_2,
		KNOCK,
		HALF_1,
		HALF_2,
		Slam,
		Beam
	};

	/*/////	�����܂�////////*/


protected:
	std::unique_ptr<DebugTxt> Damagetxt;

	bool SearchF;
	XMFLOAT3 EaseTime_Arrow;
protected:
	EnemyState* state_mob;
	BossEnemyState* state_boss;

private:
	std::list<std::unique_ptr<DamageManager>> dMans_;

};
