#include "Player.h"
#include"Input.h"
#include"DebugTxt.h"
#include"SphereCollider.h"
#include"TargetMarker.h"
#include"Collision.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"PlayerAttackState.h"
#include"imgui.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"CustomButton.h"
#include"BigSword.h"
#include"SelectSword.h"
#include"SceneManager.h"
#include"HUD.h"
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

void Player::Initialize(DebugCamera* camera)
{
	StopFlag = false;
	//�I�u�W�F�N�g�̐����A������
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("playerGolem"));
	m_fbxObject->PlayAnimation();

	//�n�`����̃R���C�_�[�Z�b�g
	SetCollider();

	Rotation = { -163.0f, -62.0f, 103.0f };
	Position = { 0.0f, 0.0f, 0.0f };
	Scale = { 0.02f, 0.02f, 0.02f };

	//�̗͏�����
	HP = MaxHP;
	//�ړ������p
	vel /= 5.0f;


	AttackEffect::GetIns()->Init();
}

//80,145
void Player::Jump()
{
	if(onGround){
		if (CustomButton::GetInstance()->GetJumpAction())
		{
			onGround = false;
			const float jumpVYFist = 0.3f;
			fallV = { 0.0f, jumpVYFist, 0.0f, 0.0f };
		}
	}
}

void Player::Move()
{
	//�ړ���~�t���O�Ɖ�����[�V�������͓����Ȃ�
	if (StopFlag || evasionF)
	{
		return;
	}
	if (attackMotion != RUN && attackMotion != NON)return;
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
		if (input->TiltPushStick(Input::L_UP, 0.0f))
		{
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, vel, 0 }, angle);
		}
		if (input->TiltPushStick(Input::L_DOWN, 0.0f))
		{
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, -vel, 0 }, angle);
		}
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f))
		{
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ vel, 0, 0, 0 }, angle);
		}
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
		Rotation = { rot.x, rot.y - 63.0f, rot.z };
		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y + 63.0f));
		move = XMVector3TransformNormal(move, matRot);

		Position.x += move.m128_f32[0] * movespeed ;
		Position.z += move.m128_f32[2] * movespeed;
		Gmove = move;

		//�����ꂩ�̃X�e�B�b�N���|����Ă���FBX�̃^�C�����ŏ��̍U�����[�V�����̃^�C�����
		//�Z���������Ԃ�RUN��
		if (f_time < AttackFirTime)
		{
			attackMotion = RUN;
		}
	} else
	{
		//�X�e�B�b�N���|����Ă��Ȃ�������ҋ@
		if (f_time < AttackFirTime)
		{
			attackMotion = NON;
		}
	}


}

#include"mHelper.h"

void Player::Evasion()
{
	if (evasionF)
	{
		if (evaTime < 1.0f)
		{
			//�v���C���[�̌����Ă�����ɃC�[�W���O�Ŕ��
			evaTime += 0.05f;
			Position.x += Gmove.m128_f32[0] * Easing::EaseOut(evaTime, 15.0f, 0.0f);
			Position.z += Gmove.m128_f32[2] * Easing::EaseOut(evaTime, 15.0f, 0.0f);
		}
		//FBX�^�C����������[�V�����J�n���ɍ��킹��
		if (f_time <= EvaTime_Start)
		{
			f_time = EvaTime_Start;
		}
		if (f_time >= EvaTime_End)
		{
			evasionF = false;
		}
		f_time += 0.02f;
	}
	else
	{
		m_fbxObject->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		evaTime = 0.0f;
	}
}

void Player::Update(DebugCamera* camera)
{
	if (m_Object == nullptr || m_fbxObject == nullptr)
	{
		return;
	}
	//�P�t���[���O�̍��W��ۑ�
	oldpos = Position;
	//�U���󂯂���̃N�[���^�C��
	RecvDamage_Cool();

	Move();
	// �W�����v����
	Jump();
	if (!onGround)
	{
		falltime++;
		if (falltime > 180)
		{
			Position = oldpos;
			falltime = 0;
		}
	} else
	{
		falltime = 0;
		savetime++;
		if (savetime % 60 == 0)
		{
			oldpos = Position;
			savetime = 0;
		}
	}
	if (input->TriggerButton(input->X))
	{
		evasionF = true;
	}
	// �s��̍X�V�Ȃ�
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		if (Collision::GetLength(Position, { 0,-19,0 }) > 90)
		{
			RecvDamage(10);
		}
	}
	//���
	Evasion();
	//��̃{�[���ʒu�ݒ�
	m_fbxObject->SetHandBoneIndex(hindex);
	m_fbxObject->SetFogPos(Position);
	//3d_fbx�X�V
	FbxAnimationControl();
	//fbx�̃^�C�}�[����
	m_fbxObject->SetFbxTime(f_time);
	//�����蔻��
		CollisionField(camera);


	ParameterSet_Obj(camera);
	ParameterSet_Fbx(camera);
	//������̍X�V
	SelectSword::GetInstance()->Update();
	//�U���G�t�F�N�g
	AttackEffect::GetIns()->Upda();
}


void Player::Draw()
{
	ImGui::Begin("pos");

	ImGui::Text("%f", Position.x);
	ImGui::Text("%f", Position.y);

	ImGui::Text("%f", Position.z);
	ImGui::End();
	Draw_Fbx();

	AttackEffect::GetIns()->Draw();
}
void Player::ParticleDraw()
{
	SelectSword::GetInstance()->SwordDraw();
}

void Player::FbxAnimationControls(const AttackMotion& motiontype, const float attacktime, const float nextAnimationtime)
{
	//�����A�j���[�V�����ǂݍ��񂾂炱������
	//if (evasionF)return;
	if (attackMotion == motiontype)
	{
		if (f_time <= attacktime)
		{
			f_time = attacktime;
		}
		if (f_time >= nextAnimationtime)
		{
			attackMotion = RUN;
		}
	}

	/*�����Ƒҋ@���[�V�����ǂ����邩*/
	if (attackMotion == NON)
	{
		f_time = 0.0f;
	}
	if (attackMotion == RUN)
	{
		if (f_time >= AttackFirTime)
		{
			f_time = 0.0f;
		}
	}
}

void Player::FbxAnimationControl()
{
	if (evasionF || noAttack||StopFlag)
	{
		return;
	}
	float timespeed = 0.02f;


	if (attackMotion == FIRST && f_time >= AttackSecTime - 0.2f)
	{
		AnimationEndJudg_FirstAttack = true;
	} else if (attackMotion == SECOND && f_time >= AttackThiTime - 0.2f)
	{
		AnimationEndJudg_SecondAttack = true;

	} else if (attackMotion == THIRD && f_time >= EvaTime_Start - 0.2f)
	{
		AnimationEndJudg_ThirdAttack = true;

	}

	/*�A�j���[�V�����J�ڏ���  �����Ђǂ��̂ŕ����A�j���[�V�����ǂݍ��񂾂����*/
	if (attackMotion == RUN || attackMotion == NON) {
		if (OldattackMotion == NON)
		{
			if (CustomButton::GetInstance()->GetAttackAction() == true)
			{
				attackMotion = FIRST;
				OldattackMotion = FIRST;
			}
		}

		if (OldattackMotion == FIRST && AnimationEndJudg_FirstAttack)
		{
			if (CustomButton::GetInstance()->GetAttackAction() == true)
			{
				AnimationEndJudg_FirstAttack = false;
				attackMotion = SECOND;
				OldattackMotion = SECOND;
			}
		}

		if (OldattackMotion == SECOND && AnimationEndJudg_SecondAttack)
		{
			if (CustomButton::GetInstance()->GetAttackAction() == true)
			{
				AnimationEndJudg_SecondAttack = false;
				attackMotion = THIRD;
				OldattackMotion = THIRD;
			}
		}


		if (OldattackMotion == THIRD && AnimationEndJudg_ThirdAttack)
		{
			if (CustomButton::GetInstance()->GetAttackAction() == true)
			{
				AnimationEndJudg_ThirdAttack = false;
				attackMotion = NON;
				OldattackMotion = NON;
			}
		}
	}
	if (PlayerAttackState::GetInstance()->GetHitStopJudg())
	{
		timespeed = 0.005f;
	}
	else {
		if (attackMotion != RUN) {
			timespeed = SelectSword::GetInstance()->GetSword()->GetAnimationTime();
		} else {
			timespeed = 0.02f;
		}
	}
	f_time += timespeed;
	FbxAnimationControls(FIRST, AttackFirTime, AttackSecTime);
	FbxAnimationControls(THIRD, AttackThiTime, EvaTime_Start);
	FbxAnimationControls(SECOND, AttackSecTime, AttackThiTime);
}


XMMATRIX Player::GetMatrot()
{
	return m_fbxObject->GetMatrot();
}

#include"CameraControl.h"

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

	if (!HUD::GetInstance()->GetRecvDamageFlag())
	{

		CameraControl::GetInstance()->ShakeCamera();
		HUD::GetInstance()->SetRecvDamageFlag(true); //�v���C���[HP��HUD�p
	}
	if (HP >= 0)
	{
		HP = HP - Damage;
	}
}

XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle)
{
	XMMATRIX rot2 = {};
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };
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
