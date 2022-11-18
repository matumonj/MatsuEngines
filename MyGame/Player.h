#pragma once
#include"Object3d.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "FbxLoader.h"
#include"f_Model.h"
#include"f_Object3d.h"
#include"ObjectManager.h"
#include"Input.h"
#include"SwordBase.h"
#include"AttackEffect.h"
class Enemy;
class TargetMarker;

class Player : public ObjectManager
{
public:
	~Player() override;

public:
	static Player* Create(Model* model, DebugCamera* camera);
	static Player* GetInstance();

public:
	//������
	void Initialize(DebugCamera* camera) override;
	//�X�V����
	void Update(DebugCamera* camera) override;
	//�`��
	void Draw() override;

public:
	//��U��
	void RecvDamage(int Damage);
	//��U����ēx�U���H�炤�܂�
	void RecvDamage_Cool();

private:
	Input* input = Input::GetInstance();
	//fbxtime����
	void FbxAnimationControl();
	//�����h�~
	void ReturnGround();
	//�W�����v
	void Jump();
	//�ړ������܂Ƃ�
	void Move();

private:
	//����t���O
	bool evasionF = false;
	//�C�[�W���O�p
	float evaTime = 0.0f;
	//�����̍��W
	XMFLOAT3 EvaPos = {};
	//�������
	void Evasion();
public:
	//OBB�̓����蔻��Ɏg���v���C���[�̉�]�s��擾
	XMMATRIX GetMatrot();
	//SelectSword�Ɏg���v���C���[�̎�̃��[���h�s��
	XMMATRIX GetHanMat() { return m_fbxObject->GetHandBoneMatWorld(); }
	//���W�̉����߂�
	void isOldPos()
	{
		Position.x = oldpos.x;
		Position.z = oldpos.z;
	}

	//�v���[���[�̓����~�߂�
	void SetStopFlag(bool f) { StopFlag = f; }
private:
	//�v���C���[������
	std::unique_ptr<SwordBase> sword = nullptr;
	XMFLOAT3 SwordPos = {};
	XMFLOAT3 SwordRot = {};

private:
	//�O���W
	XMFLOAT3 oldpos = {};
	//�ڒn����
	XMFLOAT3 onGroundPos = {};
	//�̗͎���
	int HP = 0;
	//�ő�̗�
	const int MaxHP = 100;
	//
	int CoolTime = 0;
	//
	float angle;
	//teisi
	bool StopFlag;
	//
	int FallGroundTime = 0;
public:
	int GetHP() { return HP; }
	int GetMaxHP() { return MaxHP; }

private:
	float vel = 0.0f;
	bool jumpflag = false;
	float movespeed = 7.0f;
	XMFLOAT3 MoveVECTOR(DirectX::XMVECTOR v, float angle);
private:
	bool noAttack;

	XMVECTOR Gmove;
public:
	XMVECTOR GetPlayerMove() { return Gmove; }
	void Setangle(float angle) { this->angle = angle; }

	float GetFbxTime() { return f_time; }

	bool GetNowAttack() { return nowattack; }

	enum AttackMotion
	{
		NON,
		RUN,
		FIRST,
		SECOND,
		THIRD
	};

	AttackMotion GetAttackType() { return attackMotion; }
	AttackMotion GetAttackTypeOld() { return OldattackMotion; }
	void FbxAnimationControls(const AttackMotion& motiiontype, float attacktime = 0, float nextAnimation = 0);
	void SetnoAttack(bool f) { noAttack = f; }
	bool GetnoAttack() { return noAttack; }
private:
	/*���������܂Ƃ߂ď���*/
	AttackMotion OldattackMotion = NON;
	AttackMotion attackMotion = NON;
	bool AnimationEndJudg_FirstAttack = false;
	bool AnimationEndJudg_SecondAttack = false;
	bool AnimationEndJudg_ThirdAttack = false;

	int ComboCoolT = 0;
	bool SecAttack = false;
	bool nowSecAttack = false;
	bool AttackFlag = false;
	bool nowattack = false;
	//�e�A�j���[�V�����̃X�^�[�g����
	const float AttackThiTime = 238.00f / 60.00f;
	const float AttackSecTime = 138.00f / 60.00f;
	const float AttackFirTime = 35.00f / 60.00f;

	const float EvaTime_Start = 428.00f / 60.00f;
	const float EvaTime_End = 496.00f / 60.00f;

	const float DeathTime = 20.1f;

	float f_time = 0.0f;

	//��̃{�[���C���f�b�N�X
	int hindex = 35;

public:
	float GetFbxTime_FirstAtack() { return AttackFirTime; }
	float GetFbxTime_SecondAtack() { return AttackSecTime; }
	float GetFbxTime_ThirdAtack() { return AttackThiTime; }
};
