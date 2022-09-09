#pragma once
#include"Object3d.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "FbxLoader.h"
#include"DirectXCommon.h"
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
public:
	void Initialize(DebugCamera* camera)override;
	void Update(DebugCamera* camera)override;
	//void Draw()override;

	void Action(TargetMarker* target, DebugCamera* camera);

	void Draw();

	XMMATRIX GetMatrot();
	void SetColors(XMFLOAT4 color);
	static Player* GetInstance();
	void SetGround(bool f) { onGround = f; }

	XMMATRIX GetHanMat() { return HandMat; };
	void isOldPos() {
		Position.x = oldpos.x; Position.z = oldpos.z;
	}
	float tempx, tempz;
	int onGroundTime = 0;
private:
	SwordBase* sword;
	XMMATRIX HandMat;
	enum class Attack {
		NormalAttack,//�ʏ�U��
		MagicAttack,//���@�U��
	};
	bool attackflag = false;
	bool stopf;
	float ex, ey, ez;
	int HP;
	const int MaxHP = 100;
	//�v���C���[�̉�]�p
	float angle;
private:
	//mEffekseer* effect = nullptr;
	//�G�t�F�N�g�`��̃t���O
	bool drawf = false;
	//
	int timer = 0;
	XMFLOAT3 Effect_Rot;
	XMFLOAT3 Effect_SCl;
	static XMFLOAT3 Effect_Pos;
	Input* input = Input::GetInstance();

private:

	XMVECTOR Gmove = { 0,0,0.1f,0 };
	//�L������4�����̂ǂ��������Ă��邩
	enum class RotationPrm {
		RIGHT,//�E
		LEFT,//��
		FRONT,//�O
		BACK,//���
	};
	bool jumpflag;
	float x, z;
	float movement = 2;
	int time;
	RotationPrm rotate;
	XMFLOAT3 EffectRot;
	//�J�����ɍ��킹���L�����N�^�[�̉�]
	float CharaRotation = 0;
	float cooldowntime = 0;
	bool coolflag = false;
	const float CoolTime = 50;

	f_Model* fbxmodel = nullptr;
	f_Object3d* object1 = nullptr;

protected:
	//�ڒn�t���O
	bool onGround = true;
	//�����x�N�g��
	XMVECTOR fallV;
public:
	XMVECTOR GetMove() { return Gmove; }
public:
	void AttackCoolTime();
	bool GetAttackFlag() { return attackflag; }
	void RotationStatus();
	void SetCharaRotation(float angle) { Rotation.y = angle; rotate = RotationPrm::FRONT; }
	XMFLOAT3 Getpoi() { return Effect_Pos; }
	void normalAttack(TargetMarker* target, std::vector<std::unique_ptr<Enemy>> enemy, DebugCamera* camera);
	void ImguiDraw();
	void RecvDamage(int Damage) { HP = HP - Damage; }
	int GetHP() { return HP; }
	void FbxAnimationControl();

	XMFLOAT3 SwordOBB_Pos() { return SwordPos; }
	XMFLOAT3 SwordOBB_Rot() { return SwordRot; }
private:
	XMFLOAT3 SwordPos;
	XMFLOAT3 SwordRot;
	float f_time;
	XMFLOAT3 oldpos;
	//imgui�\���p
	//���const�Ȃǂɒu������
	float movespeed = 3.0f;
	//�������Ƃ��n�ʂɑ������Ă��ŏI�ʒu��
	XMFLOAT3 OldPos_Onground;
	bool ReturnGround = false;
	int PosSavetime = 0;
private:
	
	float AttackTime = 1.5f;
	float DeathTime = 4.9f;
	bool AttackFlag;
	bool nowattack;
};

