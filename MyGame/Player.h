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
	void Initialize() override;
	//�X�V����
	void Update() override;
	//�`��
	void Draw() override;

	void ParticleDraw();
public:
	//��U��
	void RecvDamage(int Damage);
	//��U����ēx�U���H�炤�܂�
	void RecvDamage_Cool();
	void ReStartSetParam();
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


	void Death();
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

	XMFLOAT3 GetHandPos()
	{
		return {
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[0],
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[1],
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[2]
		};
	}

	//���W�̉����߂�
	void isOldPos()
	{
		Position.x = oldpos.x;
		//Position.y = oldpos.y;
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

public:
	void SetHP(int HP) { this->HP = HP; }
	int GetHP() { return HP; }
	int GetMaxHP() { return MaxHP; }
	bool GetStopFlag() { return StopFlag; };

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

	void SetnoAttack(bool f) { noAttack = f; }
	bool GetnoAttack() { return noAttack; }
private:
	/*���������܂Ƃ߂ď���*/
	AttackMotion OldattackMotion = NON;
	AttackMotion attackMotion = NON;
	bool AnimationEndJudg_FirstAttack = false;
	bool AnimationEndJudg_SecondAttack = false;
	bool AnimationEndJudg_ThirdAttack = false;

	bool onGround_Boss;

	int ComboCoolT = 0;
	bool SecAttack = false;
	bool nowSecAttack = false;
	bool AttackFlag = false;
	bool nowattack = false;

	float f_time = 0.0f;

	//��̃{�[���C���f�b�N�X
	int hindex = 21;
	float RotY;
	int nearindex;
	Enemy* NearEnemy;

public:
	enum AnimeName
	{
		IDLE,
		RUNNING,
		ATTACK1,
		ATTACK2,
		ATTACK3,
		EVASION,
		DEATH
	};

	AttackMotion GetAnimeType() { return attackMotion; }
	void SetIdle(bool f) { idlef = f; }
private:
	AnimeName animationName;

	bool idlef;
	void AnimationContol(AnimeName name, int animenumber, double speed, bool loop);
	void FbxAnimationControls(const AttackMotion& motiiontype, AttackMotion nextmotiontype, AnimeName name, int number);
};
