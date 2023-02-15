#pragma once
#include"f_Object3d.h"
#include"ObjectManager.h"
#include"Input.h"
#include"SwordBase.h"
#include"DamageManager.h"
#include<fstream>
#include<string>
#include<sstream>
#include<array>
#include "Particle.h"

class Player : public ObjectManager
{
public:
	~Player() override;

public:
	static Player* Create(Model* model, DebugCamera* camera);
	static Player* GetIns();

public:
	//������
	void Initialize() override;
	//�X�V����
	void Update() override;
	//�`��
	void Draw() override;

public:
	//��U��
	void RecvDamage(int Damage);
	//��U����ēx�U���H�炤�܂�
	void RecvDamage_Cool();
	//�Q�[���I�[�o�[���̃p�����[�^�Z�b�g
	void ReStartSetParam();


public:
	//�ڒn���Ă邩�H
	constexpr bool GetOnGround() { return onGround; }
	//OBB�̓����蔻��Ɏg���v���C���[�̉�]�s��擾
	XMMATRIX GetMatrot()
	{
		return m_fbxObject->ExtractRotationMat(m_fbxObject->ExtractPositionMat(m_fbxObject->GetMatRot()));
	};
	//SelectSword�Ɏg���v���C���[�̎�̃��[���h�s��
	XMMATRIX GetHanMat() { return m_fbxObject->GetHandBoneMatWorld(); }

	//��̃{�[���s�񂩂�Trans�s�񔲂��o��
	XMFLOAT3 GetHandPos()
	{
		return {
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[0],
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[1],
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[2]
		};
	}

	XMMATRIX GetRotMatrix()
	{
		return m_fbxObject->ExtractRotationMat(m_fbxObject->GetMatRot());
	}

	Particle* fire;
	//�̗�
	constexpr int& GetHP() { return HP; }
	//�̗͍ő�l
	constexpr int& GetMaxHP() { return MaxHP; }
	//������~
	constexpr bool& GetStopFlag() { return StopFlag; };
	//�v���C���[�̉�]�A���O��
	constexpr XMVECTOR& GetPlayerMove() { return Gmove; }

public:
	//�v���[���[�̓����~�߂�
	void SetStopFlag(bool f) { StopFlag = f; }
	//�����ҋ@
	void SetIdle(bool f) { idlef = f; }
	//�̗̓Z�b�^�[
	void SetHP(int HP) { this->HP = HP; }
	//�v���C���[�̌������J�����ɍ��킹�邽�߂̃A���O��
	void SetAngle(float angle) { this->angle = angle; }
private:
	bool DamageEvaF;
	Input* input = Input::GetIns();
	//fbxtime����
	void FbxAnimationControl();
	//�W�����v
	void Jump();
	//�ړ������܂Ƃ�
	void Move();
	//���S
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
	//���W�̉����߂�
	void isOldPos()
	{
		Position.x = oldpos.x;
		//Position.y = oldpos.y;
		Position.z = oldpos.z;
	}

	//�_���[�W�󂯂����̃m�b�N�o�b�N
	void KnockBack(XMFLOAT3 rot, float Knock);

private:
	//���������̓y��
	std::unique_ptr<Particle> runparticle;
	//���������邩�ǂ���
	bool RunParCreate;

	//�v���C���[������
	std::unique_ptr<SwordBase> sword = nullptr;
	float angle;

	float KnockPower;
private:
	//�O���W
	XMFLOAT3 oldpos = {};
	//�̗͎���
	int HP = 0;
	//�ő�̗�
	int MaxHP = 100;
	//teisi
	bool StopFlag;
	//�ړ��̉����l
	float AddSpeed;
	//���f����
	std::string modelname;
	//�_���[�W�\�L
	std::list<std::unique_ptr<DamageManager>> dMans_;
	//���G����
	int DamageCool;

private:
	float vel = 0.0f;
	XMFLOAT3 MoveVECTOR(DirectX::XMVECTOR v, float angle);
private:
	static constexpr int HandIndex = 21;

	XMVECTOR Gmove;
public:
	enum AnimeName
	{
		ANIMENAME_NON,
		ANIMENAME_RUN,
		ANIMENAME_FIRST,
		ANIMENAME_SECOND,
		ANIMENAME_THIRD,
		ANIMENAME_EVASION,
		ANIMENAME_DEATH,
	};

	enum AttackMotion
	{
		NON=9,
		RUN=2,
		FIRST=3,
		SECOND=5,
		THIRD=8,
		EVASION=6,
		DEATH=7
	};

	AttackMotion GetAttackType() { return attackMotion; }
	AttackMotion GetAttackTypeOld() { return OldattackMotion; }

private:
	/*���������܂Ƃ߂ď���*/
	AttackMotion OldattackMotion = NON;
	AttackMotion attackMotion = NON;
	bool AnimationEndJudg_FirstAttack = false;
	bool AnimationEndJudg_SecondAttack = false;
	bool AnimationEndJudg_ThirdAttack = false;


	//�A�j���[�V�����̐�
	static constexpr auto AnimationSize = 7;

	//�e�A�j���[�V�����̊�b�p�����[�^
	struct AnimeState
	{
		AttackMotion AnimeMotion;
		double AnimationSpeed;
		bool AnimeLoop;
	};

	std::array<AnimeState, AnimationSize> _AnimeState;

	AnimeState AnimationSetParam(AttackMotion motion, double speed, bool loop);
public:
	AttackMotion GetAnimeType() { return attackMotion; }

private:
	//�ҋ@�t���O
	bool idlef;
	void AnimationContol(AnimeState state);
	void FbxAttackControls(const AnimeName& motiontype, AttackMotion number);

private:
	//�p�����[�^�ǂݍ���
	void LoadCsv();

	std::string line;
	std::stringstream popcom;
	std::ifstream file;

	bool load;

public:
	void DamageTexDisplay();
	void DamageTexDraw();
};
