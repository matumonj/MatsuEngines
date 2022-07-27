#pragma once

#include"Object3d.h"
#include"mEffekseer.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "FbxLoader.h"
#include"DirectXCommon.h"
#include"f_Model.h"
#include"f_Object3d.h"
#include"Input.h"
class Enemy;
class TargetMarker;
class Player :public Object3d
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	static Player* Create(Model* model,DebugCamera* camera);
public:
	bool Initialize(DebugCamera* camera)override;

	void Update(XMFLOAT4 color, DebugCamera* camera)override;
	
	void OnCollision(const CollisionInfo& info)override;
	
	void Action( TargetMarker* target, DebugCamera* camera);

	void Draw();

	static Player* GetInstance();
	void SetGround(bool f) { onGround = f; }

	void isOldPos() { position = oldpos; }
private:

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
	Object3d* sw;
	Model* swm;


	XMVECTOR move = { 0,0,0.1f,0 };
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
	void CollisionField( DebugCamera* camera);

public:
	XMFLOAT3 GetPosition() { return position;}
	XMFLOAT3 GetRotation() { return rotation; }
	void AttackCoolTime();
	bool GetAttackFlag() { return attackflag; }
	void RotationStatus();
	void SetCharaRotation(float angle) { rotation.y = angle; rotate = RotationPrm::FRONT; }
	XMFLOAT3 Getpoi() { return Effect_Pos; }
	void normalAttack(TargetMarker* target, std::vector<std::unique_ptr<Enemy>> enemy, DebugCamera* camera);
	void ImguiDraw();
	void RecvDamage(int Damage) { HP = HP - Damage; }
	int GetHP() { return HP; }

private:
	XMFLOAT3 oldpos;
	//imgui�\���p
	//���const�Ȃǂɒu������
	float movespeed = 3.0f;
	//�������Ƃ��n�ʂɑ������Ă��ŏI�ʒu��
	XMFLOAT3 OldPos_Onground;
	bool ReturnGround = false;
	int PosSavetime = 0;
};

