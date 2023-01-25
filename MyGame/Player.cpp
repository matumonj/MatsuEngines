#include "Player.h"
#include <algorithm>
#include"Collision.h"
#include"PlayerAttackState.h"
#include"imgui.h"
#include"CustomButton.h"
#include"SelectSword.h"
#include"SceneManager.h"
#include"CameraControl.h"
#include "EnemyControl.h"
#include "HUD.h"

using namespace DirectX;

Player::~Player()
{
	Destroy_unique(m_Object);
	Destroy_unique(m_fbxObject);
}

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

Player* Player::Create(Model* model, DebugCamera* camera)
{
	return GetInstance();
}

void Player::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	StopFlag = false;
	//�I�u�W�F�N�g�̐����A������
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("ko"));
	m_fbxObject->LoadAnimation();
	m_fbxObject->PlayAnimation(1);

	//�n�`����̃R���C�_�[�Z�b�g
	SetCollider();

	Rotation = {-90.0f, 0.0f, 0.0f};
	Position = {0.0f, 0.0f, 0.0f};
	Scale = {0.02f, 0.02f, 0.02f};

	//�̗͏�����
	HP = MaxHP;
	//�ړ������p
	vel /= 5.0f;

	AttackEffect::GetIns()->Init();
}

//�W�����v
void Player::Jump()
{
	//�ڒn���̂�
	if (onGround)
	{
		if (CustomButton::GetInstance()->GetJumpAction())
		{
			onGround = false;
			const float jumpVYFist = 0.3f;
			fallV = {0.0f, jumpVYFist, 0.0f, 0.0f};
		}
	}


	if (DamageEvaF)
	{
		//FBX�^�C����������[�V�����J�n���ɍ��킹��
		AnimationContol(DEATH, 7, 15.0, false);
		m_AnimationStop = true;


		if (onGround && m_fbxObject->GetAnimeTime() >= m_fbxObject->GetEndTime() - 0.3f) {
			//attackMotion = NON;
			AnimationContol(IDLE, 9, 1, false);
			m_AnimationStop = false;

			DamageEvaF = false;
		}
	}
}
//�W�����v
void Player::DamageJump(bool judg,float knockpower)
{
	//�ڒn���̂�
	if (onGround)
	{
		onGround = false;
		const float jumpVYFist = 1.3f;
		fallV = { 0.0f, jumpVYFist, 0.0f, 0.0f };
	}

}
void Player::Move()
{
	//�ړ���~�t���O�Ɖ�����[�V�������͓����Ȃ�
	if (evasionF || StopFlag || HP <= 0 || DamageEvaF)
	{
		return;
	}
	if (attackMotion != RUN && attackMotion != NON)
	{
		//	return;
	}
	XMFLOAT3 pos = Position;
	XMFLOAT3 rot = Rotation;

	float StickX = input->GetLeftControllerX();
	float StickY = input->GetLeftControllerY();
	const float pi = 3.14159f;
	const float STICK_MAX = 32768.0f;
	
	//�X�e�B�b�N�̈ړ�����
	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f))
	{
		//��Ԃ𑖂��
		attackMotion = RUN;

		//�A�j���[�V�����𑖂�ɃZ�b�g
		if ((!m_AnimationStop))
		{
			AnimationContol(RUNNING, 2, 0.7, true);
		}

		//�����
		if (input->TiltPushStick(Input::L_UP, 0.0f))
		{
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, vel, 0 }, angle);
		}
		//������
		if (input->TiltPushStick(Input::L_DOWN, 0.0f))
		{
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, -vel, 0 }, angle);
		}
		//�E����
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f))
		{
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ vel, 0, 0, 0 }, angle);
		}
		//������
		if (input->TiltPushStick(Input::L_LEFT, 0.0f))
		{
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ -vel, 0, 0, 0 }, angle);
		}

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};

		vel.x = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		rot.y = angle + atan2f(StickX, StickY) * (180.0f / pi);

		//�v���C���[�̉�]�p�����
		Rotation = { rot.x, rot.y, rot.z };

		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y));
		move = XMVector3TransformNormal(move, matRot);

		//�����������ɐi��
		Position.x += move.m128_f32[0] * movespeed;
		Position.z += move.m128_f32[2] * movespeed ;
		Gmove = move;
	} else
	{
		//�Î~����
		if ((!m_AnimationStop))
		{
			AnimationContol(IDLE, 9, 1, true);
		}
	}

	//�T���X�e�[�W�ł̈ړ�����
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		Position.x = std::clamp(Position.x, -615.f, 600.f);
		Position.z = std::clamp(Position.z, -385.f, 822.f);
	}
	//�{�X�G���A�ł̈ړ�����
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		Position.x = std::clamp(Position.x, -90.f, 90.f);
		Position.z = std::clamp(Position.z, -90.f, 90.f);
	}

}

void Player::ReStartSetParam()
{
	m_AnimationStop = false;
	AnimationContol(IDLE, 8, 1.0, false);
}

void Player::Death()
{
	//���S��
	if (HP > 0)
	{
		return;
	}
	//�A�j���[�V���������S��
	AnimationContol(DEATH, 7, 1.0, false);

	m_AnimationStop = true;
}

#include"mHelper.h"

void Player::Evasion()
{
	//�����
	//��O�ݒ�
	if (HP <= 0||DamageEvaF)
	{
		return;
	}

	//���
	if (input->TriggerButton(input->X))
	{
		evasionF = true;
	}

	if (evasionF)
	{
		//FBX�^�C����������[�V�����J�n���ɍ��킹��
		AnimationContol(EVASION, 6, 1.0,false);

		m_AnimationStop = true;

		//����̐i�ދ����̓C�[�W���O�����ƂɌv�Z
		if (evaTime < 1.0f)
		{
			//�v���C���[�̌����Ă�����ɃC�[�W���O�Ŕ��
			evaTime += 0.03f;
			Position.x += Gmove.m128_f32[0] * Easing::EaseOut(evaTime, 15.0f, 0.0f);
			Position.z += Gmove.m128_f32[2] * Easing::EaseOut(evaTime, 15.0f, 0.0f);
		}
		else
		{
			if (m_fbxObject->GetAnimeTime() >= m_fbxObject->GetEndTime()-0.3f) {
				
				AnimationContol(IDLE, 9, 1, false);
				m_AnimationStop = false;
				
				evasionF = false;
			}
		}
		
	}
	else
	{
		evaTime = 0.0f;
	}
}

void Player::Update()
{
	//�J�����̃C���X�^���X�擾
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	//��O�ݒ�
	if (m_Object == nullptr || m_fbxObject == nullptr)
	{
		return;
	}

	//�P�t���[���O�̍��W��ۑ�
	oldpos = Position;
	//�U���󂯂���̃N�[���^�C��
	RecvDamage_Cool();

	//�ړ�
	Move();
	// �W�����v����
	Jump();
	//���S
	Death();


	//�U�����v���C���[����ԋ߂��G�����悤��
	if (TargetMarker::GetInstance()->GetNearIndex() != -1 && EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE).size() > 0) {
		//��ԋ߂��G�̔z��C���f�b�N�X���
		nearindex = TargetMarker::GetInstance()->GetNearIndex();
		Enemy* NearEnemy = EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[nearindex].get();

		//�G���v�G���C���[�̕���������
		XMVECTOR positionA = {
		Position.x,Position.y,Position.z
		};
		XMVECTOR positionB = { NearEnemy->GetPosition().x, NearEnemy->GetPosition().y,  NearEnemy->GetPosition().z };

		
		if (CustomButton::GetInstance()->GetAttackAction() &&
			Collision::GetLength(Position, NearEnemy->GetPosition()) < 30.f) {
			Rotation.y = FollowRot::FollowA_B(positionA, positionB) * 60 + 180.f;
		}
	}
	

	//���
	Evasion();
	//�v���C���[�ɑ΂��Ă̔�_���[�W�\�L
	DamageTexDisplay();
	//��̃{�[���ʒu�ݒ�
	m_fbxObject->SetHandBoneIndex(hindex);
	m_fbxObject->SetFogPos(Position);
	//3d_fbx�X�V
	FbxAnimationControl();
	//�����蔻��
	CollisionField();

	//�I�u�W�F�N�g�̍X�V
	ParameterSet_Obj();
	ParameterSet_Fbx3();

	//������̍X�V
	SelectSword::GetInstance()->Update();
	//�U���G�t�F�N�g
	AttackEffect::GetIns()->Upda();


	
}


void Player::Draw()
{
	Draw_Fbx();
	ImGui::Begin("fg");
	ImGui::SliderInt("num", &hindex, 0, 27);
	ImGui::Text("%d", TargetMarker::GetInstance()->GetNearIndex());
	ImGui::Text("%f", Position.z);
	ImGui::End();
	AttackEffect::GetIns()->Draw();
}

void Player::ParticleDraw()
{
	SelectSword::GetInstance()->SwordDraw();
}
void Player::DamageTexDraw()
{
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->Draw();
	}
}
void Player::FbxAnimationControls(const AttackMotion& motiontype, const AttackMotion nextmotiontype, AnimeName name,
                                  int number)
{
	//�����A�j���[�V�����ǂݍ��񂾂炱������
	
	
	if (attackMotion == motiontype)
	{
		
		//FBX�^�C����������[�V�����J�n���ɍ��킹��
		AnimationContol(name, number, SelectSword::GetInstance()->GetSword()->GetAnimationTime(), false);
		m_AnimationStop = true;

		if (m_fbxObject->GetAnimeTime() > m_fbxObject->GetEndTime() - 0.05)
		{
			OldattackMotion = motiontype;
			StopFlag = false;
			attackMotion = NON;
		}
		else
		{
			StopFlag = true;
		}
	}
	/*�����Ƒҋ@���[�V�����ǂ����邩*/
}

void Player::AnimationContol(AnimeName name, int animenumber, double speed, bool loop)
{
	if (PlayerAttackState::GetInstance()->GetHitStopJudg() == true)
	{
		m_AnimeSpeed = 0.3;
	} else {
		m_AnimeSpeed = speed;
	}

	if (m_Number != animenumber)
	{
		m_AnimeLoop = loop;
		m_Number = animenumber;
		
		m_fbxObject->PlayAnimation(m_Number);
	}
}

void Player::FbxAnimationControl()
{
	if (evasionF || HP <= 0||DamageEvaF)
	{
		return;
	}
	float timespeed = 0.02f;
	if (StopFlag)
	{
		//�Î~����
		if ((!m_AnimationStop))
		{
			AnimationContol(IDLE, 9, 1, true);
		}
	}
	if (CustomButton::GetInstance()->GetAttackAction() == true && OldattackMotion == NON)
	{
		attackMotion = FIRST;
	}

	if (CustomButton::GetInstance()->GetAttackAction() == true && OldattackMotion == FIRST)
	{
		attackMotion = SECOND;
	}
	if (CustomButton::GetInstance()->GetAttackAction() == true && OldattackMotion == SECOND)
	{
		attackMotion = THIRD;
	}
	if (CustomButton::GetInstance()->GetAttackAction() == true && OldattackMotion == THIRD)
	{
		attackMotion = FIRST;
	}

	FbxAnimationControls(FIRST, SECOND, ATTACK1, 3);
	FbxAnimationControls(THIRD, FIRST, ATTACK2, 5);
	FbxAnimationControls(SECOND, THIRD, ATTACK3, 8);
}


XMMATRIX Player::GetMatrot()
{
	return m_fbxObject->GetMatrot();
}

void Player::RecvDamage(int Damage)
{
	//�U���󂯂�����2�b�͖��G
	if (CoolTime != 0 || evasionF)
	{
		return;
	}
	if (CameraControl::GetInstance()->GetCameraState() != CameraControl::PLAYER)
	{
		return;
	}


	//�_���[�W����������ɃJ�����V�F�C�N
	if (!HUD::GetInstance()->GetRecvDamageFlag())
	{
		CameraControl::GetInstance()->ShakeCamera();
		HUD::GetInstance()->SetRecvDamageFlag(true); //�v���C���[HP��HUD�p
	}

	if (HP >= 0)
	{
		HP = HP - Damage/2;
		std::unique_ptr<DamageManager> newdTex;
		
			newdTex = std::make_unique<DamageManager>(
				XMFLOAT3(Position.x, Position.y + rand() % 5 -2, Position.z), Damage);
		
		dMans_.push_back(std::move(newdTex));

	}
}

void Player::DamageTexDisplay()
{
	//�_���[�W�X�v���C�g����
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->DamageDisPlay_Green(1, { 1, 1, 1, 1 });
	}

	//�A���t�@�l���ȉ��Ȃ�����j��
	dMans_.remove_if([](std::unique_ptr<DamageManager>& dTex)
		{
			return dTex->GetAlpha() <= 0.1f;
		});
}
XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle)
{
	XMMATRIX rot2 = {};
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = {v.m128_f32[0], v.m128_f32[1], v.m128_f32[2]};
	return pos;
}

void Player::RecvDamage_Cool()
{
	if (HUD::GetInstance()->GetRecvDamageFlag())
	{
		CoolTime = 120; //���G����
	}
	//�J�E���g�J�n
	CoolTime--;

	CoolTime = min(CoolTime, 120);
	CoolTime = max(CoolTime, 0);
}
