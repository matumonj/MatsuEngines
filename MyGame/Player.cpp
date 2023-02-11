#include "Player.h"
#include <algorithm>

#include"mHelper.h"
#include <FbxLoader.h>
#include "AttackEffect.h"
#include"TargetMarker.h"
#include"PlayerAttackState.h"
#include"imgui.h"
#include"SelectSword.h"
#include"SceneManager.h"
#include"CameraControl.h"
#include "HUD.h"

using namespace DirectX;

Player::~Player()
{
	Destroy_unique(m_Object);
	Destroy_unique(m_fbxObject);
}

Player* Player::GetIns()
{
	static Player instance;

	return &instance;
}

Player* Player::Create(Model* model, DebugCamera* camera)
{
	return GetIns();
}

Player::AnimeState Player::AnimationSetParam( AttackMotion motion, double speed, bool loop)
{
	AnimeState state;

	//�A�j���[�V�������x�⃋�[�v���邩���Z�b�g
	state.AnimationSpeed = speed;
	state.AnimeLoop = loop;
	state.AnimeMotion = motion;

	//�q�b�g�X�g�b�v�̂Ƃ��̓X���[
	if(PlayerAttackState::GetIns()->GetHitStopJudg())
	{
		state.AnimationSpeed = 0.3;
	}

	return state;
}

void Player::Initialize()
{
	
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	StopFlag = false;
	//�I�u�W�F�N�g�̐����A������
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	//�����ۂ̓y��
	runparticle= std::make_unique<Particle>();
	runparticle->Init(Particle::ParticleTexName::SMOKE);
	runparticle->SetParScl({ 0.3f,0.3f });
	runparticle->SetParColor({ 1.f, 1.f, 1.f, 1.f });

	//�n�`����̃R���C�_�[�Z�b�g
	SetCollider();

	Rotation = { -90.0f, 0.0f, 0.0f };
	Position = { 0.0f, 0.0f, 0.0f };
	Scale = { 0.02f, 0.02f, 0.02f };
	
	//�ړ������p
	vel /= 5.0f;

	AttackEffect::GetIns()->Init();
}

void Player::Move()
{
	//�ړ���~�t���O�Ɖ�����[�V�������͓����Ȃ�
	if (evasionF || StopFlag || HP <= 0 || DamageEvaF)
	{
		return;
	}
	XMFLOAT3 pos = Position;
	XMFLOAT3 rot = Rotation;

	float StickX = input->GetLeftControllerX();
	float StickY = input->GetLeftControllerY();
	const float STICK_MAX = 32768.0f;

	//�X�e�B�b�N�̈ړ�����
	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f))
	{
		RunParCreate = true;
		//��Ԃ𑖂��
		attackMotion = RUN;

		//�A�j���[�V�����𑖂�ɃZ�b�g
		if ((!m_AnimationStop))
		{
			AnimationContol(_AnimeState[AnimeName::ANIMENAME_RUN]);
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
		rot.y = angle + atan2f(StickX, StickY) * (PI_180 / PI);

		//�v���C���[�̉�]�p�����
		Rotation = { rot.x, rot.y, rot.z };

		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y));
		move = XMVector3TransformNormal(move, matRot);

		//�����������ɐi��
		Position.x += move.m128_f32[0] * AddSpeed;
		Position.z += move.m128_f32[2] * AddSpeed;
		
		Gmove = move;
	} else
	{
		RunParCreate = false;
		//�Î~����
		if ((!m_AnimationStop))
		{
			AnimationContol(_AnimeState[AnimeName::ANIMENAME_NON]);
		}
	}

	//�T���X�e�[�W�ł̈ړ�����
	float l_MoveLimit_x[2];
	float l_MoveLimit_z[2];

	if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	{
		l_MoveLimit_x[0] = -300.f; l_MoveLimit_x[1] = 300.f;
		l_MoveLimit_z[0] = -200.f; l_MoveLimit_z[1] = 400.f;

		Position.x = std::clamp(Position.x, l_MoveLimit_x[0], l_MoveLimit_x[1]);
		Position.z = std::clamp(Position.z, l_MoveLimit_z[0], l_MoveLimit_z[1]);

	}
	//�{�X�G���A�ł̈ړ�����
	else if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		l_MoveLimit_x[0] = -130.f; l_MoveLimit_x[1] = 130.f;
		l_MoveLimit_z[0] = -130.f; l_MoveLimit_z[1] = 130.f;

		Position.x = std::clamp(Position.x, l_MoveLimit_x[0], l_MoveLimit_x[1]);
		Position.z = std::clamp(Position.z, l_MoveLimit_z[0], l_MoveLimit_z[1]);

	}

	//�����ԈȊO�͑����̓y���o���Ȃ�
	if(attackMotion!=RUN)RunParCreate = false;
	
}

void Player::ReStartSetParam()
{
	m_AnimationStop = false;
	AnimationContol(_AnimeState[AnimeName::ANIMENAME_NON]);
}

void Player::Death()
{
	//���S��
	if (HP > 0)
	{
		return;
	}
	//�A�j���[�V���������S��
	AnimationContol(_AnimeState[AnimeName::ANIMENAME_DEATH]);

	m_AnimationStop = true;
}


void Player::Evasion()
{
	//�����
	//��O�ݒ�
	if (HP <= 0 || DamageEvaF )
	{
		return;
	}

	//��𑬓x
	constexpr float l_EvaETimeAccVal = 0.03f;
	//������̈ړ�����
	constexpr float l_EvaEaseMin = 0.f;
	constexpr float l_EvaEaseMax = 15.f;

	//���
	if (input->TriggerButton(input->X))
	{
		evasionF = true;
	}

	if (evasionF)
	{
		//���n�q�b�g�X�g�b�v�؂�
		PlayerAttackState::GetIns()->SetHitStopJudg(false);
		//FBX�^�C����������[�V�����J�n���ɍ��킹��
		AnimationContol(_AnimeState[AnimeName::ANIMENAME_EVASION]);

		m_AnimationStop = true;

		//����̐i�ދ����̓C�[�W���O�����ƂɌv�Z
		evaTime += l_EvaETimeAccVal;
		if (evaTime < 1.0f)
		{
			//�v���C���[�̌����Ă�����ɃC�[�W���O�Ŕ��
			Position.x += Gmove.m128_f32[0] * Easing::EaseOut(evaTime, l_EvaEaseMax, l_EvaEaseMin);
			Position.z += Gmove.m128_f32[2] * Easing::EaseOut(evaTime, l_EvaEaseMax, l_EvaEaseMin);
		} else
		{
			if (m_fbxObject->GetAnimeTime() >= m_fbxObject->GetEndTime() - 0.1f) {
				AnimationContol(_AnimeState[AnimeName::ANIMENAME_NON]);
				m_AnimationStop = false;

				evasionF = false;
			}
			StopFlag = false;
		}

	} else
	{
		evaTime = 0.0f;
	}
}

void Player::Update()
{
	//�J�����̃C���X�^���X�擾
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	//��O�ݒ�
	if (m_Object == nullptr)
	{
		return;
	}

	//�A�j���[�V�����̊�b�p�����[�^�Z�b�g
	_AnimeState[AnimeName::ANIMENAME_NON] = AnimationSetParam(AttackMotion::NON, 1.0, true);
	_AnimeState[AnimeName::ANIMENAME_RUN] = AnimationSetParam(AttackMotion::RUN, 1.0, true);
	_AnimeState[AnimeName::ANIMENAME_FIRST] = AnimationSetParam(AttackMotion::FIRST, SelectSword::GetIns()->GetSword()->GetAnimationTime(), false);
	_AnimeState[AnimeName::ANIMENAME_SECOND] = AnimationSetParam(AttackMotion::SECOND, SelectSword::GetIns()->GetSword()->GetAnimationTime(), false);
	_AnimeState[AnimeName::ANIMENAME_THIRD] = AnimationSetParam(AttackMotion::THIRD, SelectSword::GetIns()->GetSword()->GetAnimationTime(), false);
	_AnimeState[AnimeName::ANIMENAME_DEATH] = AnimationSetParam(AttackMotion::DEATH, 1.0, false);


	//�e��p�����[�^��CSV����
	if (!load) {
		LoadCsv();
		load = true;
	}

		//�P�t���[���O�̍��W��ۑ�
		oldpos = Position;
		//�U���󂯂���̃N�[���^�C��
		RecvDamage_Cool();

		//�ړ�
		Move();
		//���S
		Death();

		//���
		Evasion();
		//�v���C���[�ɑ΂��Ă̔�_���[�W�\�L
		DamageTexDisplay();
		//��̃{�[���ʒu�ݒ�
		m_fbxObject->SetHandBoneIndex(HandIndex);
		m_fbxObject->SetFogPos(Position);
		//3d_fbx�X�V
		FbxAnimationControl();
		//�����蔻��
		CollisionField();

		//�I�u�W�F�N�g�̍X�V
		ParameterSet_Obj();
		ParameterSet_Fbx3();

		//������̍X�V
		SelectSword::GetIns()->Update();
		//�U���G�t�F�N�g
		AttackEffect::GetIns()->Upda();

		//����A��~���͓y���o���Ȃ�
		if (!RunParCreate||evasionF)
		{
			runparticle->EndUpda(true);
		}
		else
		{
			runparticle->EndUpda(false);
		}

		//�y���̃p�[�e�B�N���̑��x�A���t�@�l���Z�l
		constexpr float l_ParticleVelSpeed = 0.01f;
		constexpr float l_ParticleAlphaSubVal = 0.04f;

		runparticle->CreateParticle(true, { Position.x,Position.y-2.f,Position.z });
		runparticle->Upda(l_ParticleVelSpeed , l_ParticleAlphaSubVal);


}


void Player::Draw()
{
	//���f���`��
	Draw_Fbx();
	//�y���`��
	runparticle->Draw();
}

void Player::DamageTexDraw()
{
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->Draw();
	}
}
void Player::FbxAttackControls(const AnimeName& motiontype,AttackMotion number)
{
	//�����A�j���[�V�����ǂݍ��񂾂炱������


	if (attackMotion == number)
	{
		//RunParCreate = false;
		//FBX�^�C�������Ɏ�������TIME�l�ɊJ�n���ɍ��킹��
		AnimationContol(_AnimeState[motiontype]);
		m_AnimationStop = true;

		if (m_fbxObject->GetAnimeTime() > m_fbxObject->GetEndTime() - 0.05)
		{
			OldattackMotion = number;
			StopFlag = false;
			attackMotion = NON;
		} else
		{
			StopFlag = true;
		}
	}
	/*�����Ƒҋ@���[�V�����ǂ����邩*/
}

void Player::AnimationContol(AnimeState state)
{
	if (m_Number != state.AnimeMotion)
	{
		m_AnimeLoop = state.AnimeLoop;
		m_Number =state.AnimeMotion;
		m_fbxObject->PlayAnimation(m_Number);
	}
}

void Player::FbxAnimationControl()
{
	if ( HP <= 0 || DamageEvaF)
	{
		return;
	}
	if(evasionF)
	{
		attackMotion = NON;
		return;
	}
	constexpr float timespeed = 0.02f;
	if (StopFlag)
	{
		//�Î~����
		if ((!m_AnimationStop))
		{
			AnimationContol(_AnimeState[AnimeName::ANIMENAME_NON]);
		}
	}
	if (input->TriggerButton(input->B) && OldattackMotion == NON)
	{
		attackMotion = FIRST;
	}

	if (input->TriggerButton(input->B) && OldattackMotion == FIRST)
	{
		attackMotion = SECOND;
	}
	if (input->TriggerButton(input->B) && OldattackMotion == SECOND)
	{
		attackMotion = THIRD;
	}
	if (input->TriggerButton(input->B) && OldattackMotion == THIRD)
	{
		attackMotion = FIRST;
	}

	FbxAttackControls(AnimeName::ANIMENAME_FIRST,AttackMotion::FIRST);
	FbxAttackControls(AnimeName::ANIMENAME_SECOND, AttackMotion::THIRD);
	FbxAttackControls(AnimeName::ANIMENAME_THIRD, AttackMotion::SECOND );
}


void Player::KnockBack(XMFLOAT3 rot, float Knock)
{
	//if (KnockPower <= 0)return;
	KnockPower = Knock;
	if (KnockPower > 0.f) {
		//�ړ��x�N�g����y������̊p�x�ŉ�]
		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };

		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rot.y));

		move = XMVector3TransformNormal(move, matRot);

		Position.x += move.m128_f32[0] * KnockPower;
		Position.z += move.m128_f32[2] * KnockPower;

		KnockPower -= 0.1f;
	}
}

void Player::RecvDamage(int Damage)
{

	//�U���󂯂�����2�b�͖��G
	if ( evasionF||HP<0)
	{
		return;
	}
	if (CameraControl::GetIns()->GetCameraState() != CameraControl::PLAYER)
	{
		return;
	}


	//�_���[�W����������ɃJ�����V�F�C�N
	if (!HUD::GetIns()->GetRecvDamageFlag())
	{
		CameraControl::GetIns()->ShakeCamera();
	
		HUD::GetIns()->SetRecvDamageFlag(true); //�v���C���[HP��HUD�p
	}
	HP = HP - Damage;
	std::unique_ptr<DamageManager> newdTex;

	newdTex = std::make_unique<DamageManager>(
	XMFLOAT3(Position.x, Position.y + float(rand() % 5 - 2), Position.z), Damage);
	dMans_.push_back(std::move(newdTex));

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
	XMFLOAT3 pos = { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };
	return pos;
}

void Player::RecvDamage_Cool()
{
	if (HUD::GetIns()->GetRecvDamageFlag())
	{
		DamageCool= 120; //���G����
	}
	//�J�E���g�J�n
	DamageCool--;

	DamageCool = min(DamageCool, 120);
	DamageCool = max(DamageCool, 0);
}

void Player::LoadCsv()
{
		file.open("Param_CSV/CharaParam/Player_Param.csv");

		popcom << file.rdbuf();

		file.close();

		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find("HP") == 0)
			{
				std::getline(line_stream, word,',');
				int l_HP = static_cast<int>(std::atof(word.c_str()));
				MaxHP = l_HP;
			}
			if (word.find("RunSpeed") == 0)
			{
				std::getline(line_stream, word, ',');
				float l_addspeed = static_cast<float>(std::atof(word.c_str()));
				AddSpeed = l_addspeed;
			}
			if (word.find("Model") == 0)
			{
				std::getline(line_stream, word, ',');
				std::string l_modelname = static_cast<std::string>((word.c_str()));
				modelname = l_modelname;
				break;
			}
		}

		m_fbxObject = std::make_unique<f_Object3d>();
		m_fbxObject->Initialize();
		m_fbxObject->SetModel(FbxLoader::GetIns()->LoadModelFromFile(modelname));
		m_fbxObject->LoadAnimation();
		m_fbxObject->PlayAnimation(1);
	
	//�̗͏�����
	HP = MaxHP;
}
