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
class NodeBase;
/// <summary>
/// �G�L�����̊��N���X
/// </summary>
/// 
class Enemy :public ObjectManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Enemy();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Enemy();

	static Enemy* Create(Model* model, DebugCamera* camera);

public:
	//������
	virtual void Initialize(DebugCamera* camera)override;
	//�X�V����
	virtual void Update(DebugCamera* camera)override;
	//�`�揈��
	virtual void Draw()override;
	//���S����
	virtual void Death() = 0;
	//Fbx����
	virtual void FbxAnimationControl() = 0;
	//
	virtual void Action();

	virtual void DamageTexUpdate(DebugCamera* camera)=0;

	void isRespawn();
protected:
	//�̗͎���
	float EnemyHP;//���݂̗̑�
	float MaxHP;//�ő�l(�ϓ��Ȃ�)
	float OldHP;//�O�t���[�����̗̑�(HUD�̃C�[�W���O�p)
protected:
	float alpha = 1.0f;
	XMFLOAT3 RespawnPos;
	bool RecvDamagef;//
	bool RecvAttackHit;
	bool DamageParticleCreateF;//�U���󂯂�����p�[�e�B�N�������t��g

private:
	bool MoveFlag = false;
	XMFLOAT3 OldPos;
public:
	//�U���󂯂�����̔���p
	void RecvDamage(int Damage);
	void EnemyPop(int HP);
	virtual void AttackCoolTime() = 0;

protected:
	bool f_AttackFlag;
	bool DeathFlag;
	bool nowDeath;
	int cooltime = 0;

	int onGroundTime = 0;
	bool AfterAttack;

	int PopCount;//���X�|�[���J�E���g

protected:
	//FBXTime����̕ϐ�
	float f_time;//���݂̃t���[��
	float start_time;//�����t���[��(0)
	float end_time;//�ŏI�t���[��
	float DeathTime;//���S���̃��[�V�����J�n�t���[��
	float AttackTime = 0;


	/*�Q�b�^�[*/
public:
	bool GetRecvAttack() { return RecvAttackHit;}
	void SetRecvAttack(bool f) { RecvAttackHit = f; }
	//�U���󂯂�����̔���p
	bool GetRecvDamage() { return RecvDamagef; }
	//�����Ă��邩
	bool GetMoveFlag() { return MoveFlag; }
	//�n�ʐݒu��Ԃ��ǂ���
	bool GetonFlag() { return onGround; }
	//�̗͎���
	float GetHP() { return EnemyHP; };
	float GetMaxHP() { return MaxHP; }
	//FBXTime(���Ƀ��[�V�����ƍU�����[�V�����̊J�n�t���[���擾)
	bool GetDeathTime() { return DeathFlag; }
	bool GetAttackTime() { return f_AttackFlag; }
	//���݂̌o�߃t���[��
	float GetFbxTime() { return f_time; }
	//��]�s��擾(OBB�p����Ȃ�����)
	XMMATRIX GetMatrot() { return m_fbxObject->GetMatrot(); }
	//�U���̃N�[���^�C��
	int GetCoolTime() { return cooltime; }

	/*�Z�b�^�[*/
public:
	void SetRecvDamage(bool f) { RecvDamagef = f; }
	//
	void SetMoveFlag(bool f) { MoveFlag = f; }

	void SetScale(XMFLOAT3 scale) { Scale = scale; }

	void SetAttackTime(bool f) { if (f_time < AttackTime) { f_AttackFlag = f; } }

	void SetRespawnPos(XMFLOAT3 position) { RespawnPos = position; }
/*�{�X�U���p �ł���Έڂ��������A�A*/
protected://�U���̊J�n�ƏI������p
	struct Attack_SE {
		bool start;
		bool end;
	};
	//�U����
	static const int AtckNum = 7;
	Attack_SE Attack[AtckNum];

public:
	bool GetAttack_Start(int Num) { return Attack[Num].start; }
	bool GetAttack_End(int Num) { return Attack[Num].end; }

	void SetAttack_Start(int Num, bool f) { Attack[Num].start = f; }
	void SetAttack_End(int Num, bool f) { Attack[Num].end = f; }

public://state�؂�ւ�
	void ChangeState_Mob(EnemyState* state);
	void ChangeState_Boss(BossEnemyState* state);

	public://�U����ޗ�
		enum {
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
	Texture* SlashTex;
	std::unique_ptr<DebugTxt>Damagetxt;
	XMFLOAT3 SlashRot;
	XMFLOAT3 SlashPos;
	float SlashAlpha = 1.0f;
protected:
	EnemyState* state_mob;
	BossEnemyState* state_boss;
	ParticleManager* particleMan = nullptr;
	ParticleManager* particleMan2 = nullptr;

};
