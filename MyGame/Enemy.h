#pragma once
#include <DirectXMath.h>
#include"Player.h"
#include"EnemyState.h"
#include"BossEnemyState.h"
#include"GuardianState.h"
#include"Destroy.h"
#include"ObjectManager.h"
#include"DebugTxt.h"
#include<list>
#include"Task.h"

#include"FbxLoader.h"
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
	void Initialize() override =0;
	//�X�V����
	void Update() override =0;
	//�`�揈��
	void Draw() override =0;
	//���S����
	virtual void Death() = 0;
	//Fbx����
	virtual void FbxAnimationControl() = 0;
	//
	virtual void Action();

	void isRespawn();

	void DestroyJudg();

	virtual void Move() = 0;

	virtual void Smoke(bool& createf) =0;

	virtual void EnemyHPDraw() =0;

public:
	void Respawn();
	int GetRespawnCount() { return RespawnCount; }
	void ResetRespawnCount() { RespawnCount = 0; }
	bool RespawnJudg(); //

protected:
	bool isAlive;
	//�̗͎���
	int EnemyHP = 0; //���݂̗̑�
	int MaxHP = 0; //�ő�l(�ϓ��Ȃ�)
	float OldHP = 0.0f; //�O�t���[�����̗̑�(HUD�̃C�[�W���O�p)
protected:
	//�I�u�W�F�̃A���t�@�l
	float alpha = 1.0f;
	//���X�|�[���n�_
	XMFLOAT3 RespawnPos = {0.0f, 0.0f, 0.0f};
	bool RecvDamagef = false; //HUD�p
	bool RecvDamagef2 = false; //��������p
	bool RecvAttackHit = false; //
	bool DamageParticleCreateF = false; //�U���󂯂�����p�[�e�B�N�������t��g
private:
	//�����邩�ǂ���
	bool MoveFlag = false;
	//�P�t���[���O�̍��W
	XMFLOAT3 OldPos = {0.0f, 0.0f, 0.0f};
	int DamageSize = 0;
	XMFLOAT3 DamageTexPos = {};
public:
	//�U���󂯂�����̔���p
	void RecvDamage(int Damage);

	virtual void AttackCoolTime() = 0;
	void SetMoveStop(bool f) { movestop = f; }
	bool GetMoveStop() { return movestop; }
	bool GetisAlive() { return isAlive; }
	void SetisAlive(bool f) { isAlive = f; }
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

	int RespawnCount = 0; //���X�|�[���J�E���g

	int RespawnCountMax;
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

	AnimationState GetAnimeState() { return animeState; }
	void SetAnimeState(AnimationState state) { animeState = state; }
	int GetEnemyNumber() { return ENumber; }
protected:
	/*blender�̕����A�j���[�V�����ŗ�����S������*/
	AnimationState animeState;
	//�G�̉�]�����p
	float addRotRadians;
	float FollowRotAngleCorrect;

	//�e�A�j���[�V�����̊J�n�^�C��

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
	int GetHP() { return EnemyHP; };
	//�̗͍ő�l
	int GetMaxHP() { return MaxHP; }
	//FBXTime(���Ƀ��[�V�����ƍU�����[�V�����̊J�n�t���[���擾)
	bool GetDeathTime() { return nowDeath; }

	bool GetAttackTime() { return f_AttackFlag; }
	//���݂̌o�߃t���[��
	float GetFbxTime() { return f_time; }
	//��]�s��擾(OBB�p����Ȃ�����)
	XMMATRIX GetMatrot() { return m_fbxObject->ExtractRotationMat(m_fbxObject->GetWorld()); }
	XMMATRIX GetMatrotObj() { return m_Object->GetMatrot(); }
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

	void SetRespawnCountMax(int count) { RespawnCountMax = count; }

	void SetEColor(XMFLOAT4 color) { Color = color; }
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
		MINIGOLEM,
		GUARDIAN
	};


	void SetAnimationNum(int number) { m_Number = number; }
protected:
	int ENumber;
	//�U����
	static const int AtckNum = 8;
	Attack_SE Attack[AtckNum];

public:
	bool getdie() { return DieFlag; }
	bool getdeath() { return DeathFlag; }


	void SetAnimation(int number, bool loop, double speed);
	bool GetAttack_Start(int Num) { return Attack[Num].start; }
	bool GetAttack_End(int Num) { return Attack[Num].end; }

	void SetAttack_Start(int Num, bool f) { Attack[Num].start = f; }
	void SetAttack_End(int Num, bool f) { Attack[Num].end = f; }

	void SetHP(int HP) { this->EnemyHP = HP; }
public: //state�؂�ւ�
	void ChangeState_Mob(EnemyState* state);
	void ChangeState_Boss(BossEnemyState* state);
	void ChangeState_Guardian(GuardianState* state);

	enum NowAttackMotion
	{
		SHILEDWALK_FRONT=0,
		SHIELDWALK_SIDE=1,
		BIDLE=11,
		BROAR=8,
		BNORMAL=12,
		BNORMAL2 = 15,
		MAGIC=13,
		SWING=7,
		EVASION=3,
		FALTER=10,
		BWALK=14,
		BDEATH=9,
		CHARGE=2,
		THROW=4,
		BJUMP=5
	};

	int GetNowMotion() { return m_Number; }
protected:
	NowAttackMotion attackNum = BIDLE;
	OBB ShieldOBB;
	bool GuardAction;
	int guardtime;
	int GuarPoint;
public:
	OBB GetShieldOBB() { return ShieldOBB; }
	bool GetGuardAction() { return GuardAction; }
	void SetGuardAction(bool f) { GuardAction = f; }
	int GetGuardTime() { return guardtime; }
	void SetGuardTime(int f) { guardtime = f; }
	void SetGuardPointAdd() { GuarPoint++; }

	int GetGuardPoint() { return GuarPoint; };
public: //�U����ޗ�
	enum
	{
		CIRCLE_1,
		CIRCLE_2,
		KNOCK,
		HALF_1,
		ULT,
		Slam,
		Beam,
		BRONZEATTACK_W,
		BRONZEATTACK_H
	};

	/*/////	�����܂�////////*/


protected:
	std::unique_ptr<DebugTxt> Damagetxt;

	bool SearchF;
	XMFLOAT3 EaseTime_Arrow;
protected:
	GuardianState* state_guardian;
	EnemyState* state_mob;
	BossEnemyState* state_boss;
protected:
	//����̍s��
	XMMATRIX handmat_right;
	XMMATRIX handmat_right2;
	XMMATRIX handmat_left;

	//����̍��W
	XMFLOAT3 HandPos_Right; //18
	XMFLOAT3 HandPos_Right2; //18
	XMFLOAT3 HandPos_Left; //43

	float rotadds;

	bool HelpJudg;

protected:
	std::unique_ptr<Sprite> HelpIcon;
	float HelpIconAlpha;
public:
	void HelpIconInit();
	void HelpIconShow();
	void HelpIconDraw();
	void HelpAction();
	void SetHelpIcon(bool f) { HelpJudg = f; }
	bool GetHelpJudg() { return HelpJudg; }
	float Getaddroa() { return rotadds; }
	XMFLOAT3 HandRightPos() { return HandPos_Right; }
	XMFLOAT3 HandRightPos2() { return HandPos_Right2; }
	XMFLOAT3 HandLeftPos() { return HandPos_Left; }
protected:
	std::array<std::unique_ptr<Sprite>, 4> HPFrame;
	std::unique_ptr<Sprite> EnemyName;
	float NowFrameX;
	float OldFrameX;

	float OldFrameX_Inner;
	float FrameScalingETime;


	bool InnerFrameScalingF;
	float FrameScalingETime_Inner;
	XMFLOAT2 FrameScl;
	XMFLOAT2 FrameScl_Inner;

protected:
	virtual void HPFrameInit() =0;
	void HPFrameUpda();
private:
	std::list<std::unique_ptr<DamageManager>> dMans_;
protected:
	std::unique_ptr<Object3d> ShieldObj;
	XMFLOAT3 shieldRot;

public:
	XMMATRIX GetMatWol() { return ShieldObj->ExtractRotationMat(); }
	XMFLOAT3 GetShieldPos() { return HandPos_Right; }
	XMFLOAT3 GetShieldRot() { return ShieldObj->GetRotation(); }
};
