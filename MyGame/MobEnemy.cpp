#include "MobEnemy.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"DebugCamera.h"
#include"mHelper.h"
#include"imgui.h"
#include"Collision.h"
#include"PlayerControl.h"
#include "CameraControl.h"
#include"PlayerAttackState.h"
#include"ImageManager.h"
#include "SceneManager.h"
/// <summary>
/// �R���X�g���N�^
/// </summary>
using namespace DirectX;

MobEnemy::MobEnemy()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MobEnemy::~MobEnemy()
{
	Destroy(state_mob);
	Destroy_unique(Sword);
	//Destroy_unique(m_fbxObject);

	for (int i = 0; i < 3; i++)
	{
		HPFrame[i].reset();
	}
}

void MobEnemy::HPFrameInit()
{
	//�̗̓o�[������
	Sprite* l_frame1 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME1), {0, 0});
	Sprite* l_frame2 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME2), {0, 0});
	Sprite* l_frame3 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME4), {0, 0});
	Sprite* l_frame4 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME3), {0, 0});

	Sprite* l_enemyname = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENEMYNAME_GOLEM), {0, 0});

	HPFrame[0].reset(l_frame1);
	HPFrame[1].reset(l_frame2);
	HPFrame[2].reset(l_frame3);
	HPFrame[3].reset(l_frame4);

	EnemyName.reset(l_enemyname);

	for (int i = 0; i < 4; i++)
	{
		HPFrame[i]->SetAnchorPoint({0.0f, 0.0f});
	}
	FrameScl.x = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;
}

//����������
void MobEnemy::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	//�I�u�W�F�N�g�̐����Ə�����
	Sword = std::make_unique<Object3d>();
	Sword->Initialize(camera);
	Sword->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGSWORD));
	Sword->SetRotation({0, 0 + 30, 0 + 100});

	//���f��������
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo"));
	m_fbxObject->PlayAnimation();

	//�̗͐ݒ�
	MaxHP = 100;
	EnemyHP = MaxHP;
	//�p�����[�^�̃Z�b�g
	Rotation = {114.0f, 118.0f, 165.0f};
	Scale = {0.05f, 0.05f, 0.05f};

	//�R���C�_�[����
	radius_adjustment = 0;
	SetCollider();

	//FBX�؂�ւ��̃^�C���w��
	NormalAttackTime = 0.9f;
	DeathTime = 6.9f;

	//�U�����Ǝ��S���[�V�������̃t���O
	nowAttack = false;
	nowDeath = false;
	DeathFlag = false;
	//state������
	state_mob->Initialize(this);

	//�ǐՂ̉�]�p�̒����p
	addRotRadians = 0;
	FollowRotAngleCorrect = 180;

	HPFrameInit();

	//�K�ԍ��F�S�[����
	ENumber = GOLEM;

	attackinter = rand() % 120 + 90;
}

//�X�V����
void MobEnemy::Update()
{
	//�J�����̃C���X�^���X�����Ă���
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	//state�X�V
	state_mob->Update(this);

	if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE)
	{
		m_fbxObject->SetFogPos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	}

	if (SceneManager::GetInstance()->GetScene() != SceneManager::BOSS)
	{
		CollisionField();
	}
	//FBX�A�j���[�V�����X�V
	FbxAnimationControl();
	//HP�o�[�X�V
	HPFrameUpda();
	//EnemyPop(150);

	AttackCoolTime();

	DamageTexDisplay();
	ParameterSet_Fbx();


	//�G�̐F
	m_fbxObject->SetColor({1.0f, 1.0f, 1.0f, alpha});
	//�G�̎�̃{�[���C���f�b�N�X
	m_fbxObject->SetFbxTime(f_time);
	HandIndex = 23;
	m_fbxObject->SetHandBoneIndex(HandIndex);
	//�����Ă镀�̍X�V
	Sword->Setf(FALSE);
	Sword->SetRotation({-23, 43, 83});
	Sword->Update(m_fbxObject->GetHandBoneMatWorld(), {1.0f, 1.0f, 1.0f, 1.0f}, camera);

	HandMat = m_fbxObject->GetHandBoneMatWorld();

	//���ƃv���C���[�̂����蔻��
	OBBSetParam();
}


void MobEnemy::OBBSetParam()
{
	//�v���C���[�̃C���X�^���X�����o��
	Player* l_player = PlayerControl::GetInstance()->GetPlayer();

	if (SceneManager::GetInstance()->GetScene() == SceneManager::MAPCREATE)
	{
		return;
	}
	if (l_player == nullptr)
	{
		return;
	}
	if (DeathFlag || Collision::GetLength(Position, l_player->GetPosition()) > 40)
	{
		return;
	}

	//��̃��[���h�s�񂩂�e���������o��
	HandSiteOBB.SetOBBParam_Pos(Sword->ExtractPositionMat());
	HandSiteOBB.SetOBBParam_Rot(Sword->ExtractRotationMat());
	HandSiteOBB.SetOBBParam_Scl({10.0f, 10.0f, 8.0f});

	//�v���C���[
	playerOBB.SetOBBParam_Pos(l_player->GetPosition());
	playerOBB.SetOBBParam_Rot(l_player->GetMatrot());
	playerOBB.SetOBBParam_Scl({3.0f, 9.0f, 3.0f});

	//���ガ�U��
	if (atcktype == SIDEAWAY)
	{
		//�A�j���[�V���������t���[���z������
		if (f_time >= NormalAttackTime + 1.f)
		{
			if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true)
			{
				l_player->RecvDamage(10);
			}
		}
	}
	//�c�U��U��
	if (atcktype == VERTICAL)
	{
		if (f_time >= 4.8f && f_time <= 5.2f)
		{
			if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true)
			{
				l_player->RecvDamage(10);
			}
		}
	}
}


//�`�揈��
void MobEnemy::Draw()
{
	//�A���t�@�l���O�Ȃ�����a���؂�
	if (alpha >= 0.0f)
	{
		Draw_Fbx();

		// 3D�I�u�W�F�N�g�`��O����
		Object3d::PreDraw();
		Sword->Draw();
		Object3d::PostDraw();
	}
	ImGui::Begin("hindex");
	ImGui::SliderInt("ind", &HandIndex, 0, 40);
	ImGui::End();
}

void MobEnemy::EnemyHPDraw()
{
	if (alpha <= 0.f)return;
	Player* l_player = PlayerControl::GetInstance()->GetPlayer();

	if (Collision::GetLength(Position, l_player->GetPosition()) < 40)
	{
		Sprite::PreDraw();
		for (int i = 0; i < 4; i++)
		{
			HPFrame[i]->Draw();
		}
		EnemyName->Draw();
		Sprite::PostDraw();
	}
}

#include"ExpPointSystem.h"

void MobEnemy::Death()
{
	if (!DeathFlag)
	{
		DeathFlag = true;
		//�A�j���[�V���������S���ɍ��킹��
		if (f_time < DeathTime)
		{
			f_time = DeathTime;
		}
	}
	//�ŏI�t���[���������瓧����
	if (f_time >= m_fbxObject->GetEndTime())
	{
		alpha -= 0.02f;
	}
	if (f_time <= DeathTime + 1.5f)
	{
		PlayerAttackState::GetInstance()->SetHitStopJudg(true);
	}

	f_time += 0.01f;

	//	PlayerAttackState::GetInstance()->SetHitStopJudg(TRUE);

	movestop = false;
}

void MobEnemy::Move()
{
}

void MobEnemy::Smoke(bool& createf)
{
}

void MobEnemy::FbxAnimationControl()
{
	if (DeathFlag)
	{
		return;
	}
	//�A�j���[�V�����X�s�[�h
	float fbxanimationTime;


	//�q�b�g�X�g�b�v��
	if (PlayerAttackState::GetInstance()->GetHitStopJudg())
	{
		fbxanimationTime = 0.002f;
	}
	else
	{
		if (nowAttack)
		{
			//�U����
			fbxanimationTime = 0.015f;
		}
		else
		{
			//�����Ƃ����S��
			fbxanimationTime = 0.01f;
		}
	}
	//�A�j���[�V�����J�E���g�i�߂�
	f_time += fbxanimationTime;

	//�U�����@�����_���Ō���
	if (f_AttackFlag)
	{
		//50�ȉ��ł͉��ガ
		rand_Attacktype = static_cast<float>(rand() % 100);
		if (rand_Attacktype <= 50)
		{
			atcktype = SIDEAWAY;
			f_time = NormalAttackTime;
		}
		else
		{
			//�ȏ�͏c�U��
			atcktype = VERTICAL;
			f_time = 3.7f;
		}

		nowAttack = true;
		f_AttackFlag = false;
	}
	else
	{
		if (!nowAttack && f_time >= NormalAttackTime)
		{
			f_time = 0.0f;
		}
	}

	if (atcktype == SIDEAWAY)
	{
		if (f_time > 3.7f)
		{
			nowAttack = false;
		}
	}
	else if (atcktype == VERTICAL)
	{
		if (f_time > DeathTime)
		{
			nowAttack = false;
		}
	}
}


void MobEnemy::AttackCoolTime()
{
	if (f_time >= DeathTime - 1)
	{
		AfterAttack = true;
	}
	if (AfterAttack)
	{
		cooltime++;
		if (cooltime > attackinter)
		{
			AfterAttack = false;
		}
	}
	else
	{
		cooltime = 0;
	}
}
