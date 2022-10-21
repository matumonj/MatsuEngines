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
class Enemy;
class TargetMarker;
class Player :public ObjectManager
{
public:
	~Player();

public:
	static Player* Create(Model* model, DebugCamera* camera);
	
	static Player* GetInstance();

public:
	//������
	void Initialize(DebugCamera* camera)override;
	//�X�V����
	void Update(DebugCamera* camera)override;
	//�`��
	void Draw();

public:
	//��U��
	void RecvDamage(int Damage);
	//
	void RecvDamage_Cool();
	
private:
	//fbxtime����
	void FbxAnimationControl();
	void ReturnGround();
	void Jump();
	void Move();
	bool evasionF;
	float evaTime;
	XMFLOAT3 EvaPos;
	void Evasion();
public:
	//OBB�̓����蔻��Ɏg���v���C���[�̉�]�s��擾
	XMMATRIX GetMatrot();
	XMMATRIX GetHandMatRot() { return m_fbxObject->GetRotmatrix(); }
	XMFLOAT3 GetHandRotation_xyz() { return m_fbxObject->GetHandRotation(); }
	//SelectSword�Ɏg���v���C���[�̎�̃��[���h�s��
	XMMATRIX GetHanMat() { return HandMat; };
	//���W�̉����߂�
	void isOldPos() {
		Position.x = oldpos.x; Position.z = oldpos.z;
	}
	void SetStopFlag(bool f) { StopFlag = f; }
private:
	//�v���C���[������
	SwordBase* sword;
	XMFLOAT3 SwordPos;
	XMFLOAT3 SwordRot;
	//�O���W
	XMFLOAT3 oldpos;

	XMFLOAT3 onGroundPos;
	//��s��
	XMMATRIX HandMat;
	//�̗͎���
	int HP;
	const int MaxHP = 100;
	//
	int CoolTime;
	//�v���C���[�̉�]�p
	enum class RotationPrm {
		RIGHT,//�E
		LEFT,//��
		FRONT,//�O
		BACK,//���
	};
	float angle;
	//�J�����ɍ��킹���L�����N�^�[�̉�]
	float CharaRotation = 0;
	XMMATRIX rot2;
	XMVECTOR v;

	RotationPrm rotate;
	bool StopFlag;
private:
	float vel;
	XMFLOAT3 MoveVECTOR(DirectX::XMVECTOR v, float angle);

	Input* input = Input::GetInstance();
	f_Model* fbxmodel = nullptr;
	f_Object3d* object1 = nullptr;

private:
	int nogroundtime;
	bool jumpflag;
	float movespeed = 9.0f;
	XMVECTOR Gmove;
public:
	void RotationStatus();
	
	void SetCharaRotation(float angle) { Rotation.y = angle; rotate = RotationPrm::FRONT; }
	
	int GetHP() { return HP; }
	
	int GetMaxHP() { return MaxHP; }

	float GetFbxTime() { return f_time; }

	bool GetNowAttack() { return nowattack; }
	enum AttackMotion {
		NON,
		RUN,
		FIRST,
		SECOND
	};
	AttackMotion GetAttackType() { return attackMotion; }
private:
	
	AttackMotion attackMotion=NON;
	bool SecAttack;
	bool nowSecAttack;
	float sectime=1.8f;
	float f_time;
	float AttackTime = 0.571f;
	float DeathTime =20.1f;
	bool AttackFlag;
	bool nowattack;

	int hindex=15;
};

