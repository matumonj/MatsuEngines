#include "MobEnemy.h"
#include"CustomButton.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include"mHelper.h"
#include"imgui.h"
#include"Collision.h"
#include"PlayerControl.h"
#include<iomanip>
#include"PlayerAttackState.h"
#include"ImageManager.h"
#include"mHelper.h"
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
}

//����������
void MobEnemy::Initialize(DebugCamera* camera)
{
	//�I�u�W�F�N�g�̐����Ə�����
	Sword = std::make_unique<Object3d>();
	Sword->Initialize(camera);
	Sword->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGSWORD));
	Sword->SetRotation({0, 0 + 30, 0 + 100});

	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo"));
	m_fbxObject->PlayAnimation();

	MaxHP = 100.0f;

	EnemyHP = MaxHP;
	//�p�����[�^�̃Z�b�g
	Rotation = {114.0f, 118.0f, 165.0f};
	Scale = {0.04f, 0.04f, 0.04f};

	//�R���C�_�[����
	radius_adjustment = 0;
	SetCollider();

	//FBX�؂�ւ��̃^�C���w��
	NormalAttackTime = 0.9f;
	DeathTime = 6.9f;

	nowAttack = false;
	nowDeath = false;

	//state������
	state_mob->Initialize(this);

	addRotRadians = 0;
	FollowRotAngleCorrect = 180;

	HPFrame[0].reset(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME1));
	HPFrame[1].reset(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME2));
	HPFrame[2].reset(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME3));
	for (int i = 0; i < 3; i++)
	{
		HPFrame[i]->SetAnchorPoint({0.0f, 0.0f});
	}
	FrameScl.x = Percent::GetParcent(MaxHP, EnemyHP) * 2.0f;
	ENumber = GOLEM;
}

#include"SceneManager.h"
//�X�V����
void MobEnemy::Update(DebugCamera* camera)
{
	state_mob->Update(this);

	if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE)
	{
		m_fbxObject->SetFogPos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	}
	if (SceneManager::GetInstance()->GetScene() != SceneManager::BOSS)
	{
		CollisionField(camera);
	}
	FbxAnimationControl();

	//EnemyPop(150);

	AttackCoolTime();

	DamageTexDisplay();
	ParameterSet_Fbx(camera);


	m_fbxObject->SetColor({1.0f, 1.0f, 1.0f, alpha});
	m_fbxObject->SetHandBoneIndex(25);

	Sword->Setf(FALSE);
	Sword->SetRotation({-23, 43, 83});
	Sword->Update(m_fbxObject->GetHandBoneMatWorld(), {1.0f, 1.0f, 1.0f, 1.0f}, camera);

	HandMat = m_fbxObject->GetHandBoneMatWorld();

	OBBSetParam();
	m_fbxObject->SetFbxTime(f_time);

	XMVECTOR tex2DPos[3];
	for (int i = 0; i < 3; i++)
	{
		tex2DPos[i] = {Position.x, Position.y + 12.0f, Position.z};
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewMatrix(), false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetProjectionMatrix(), true);
		tex2DPos[i] = MatCal::WDivi(tex2DPos[i], false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewPort(), false);

		HPFrame[i]->SetPosition({tex2DPos[i].m128_f32[0] - 40.0f, tex2DPos[i].m128_f32[1]});
	}
	if (RecvDamagef)
	{
		NowFrameX = Percent::GetParcent(MaxHP, EnemyHP) * 2.0f;
		FrameScalingETime += 0.02f;
		FrameScl.x = Easing::EaseOut(FrameScalingETime, OldFrameX, NowFrameX);

		if (FrameScalingETime >= 1.0f)
		{
			RecvDamagef = false;
		}
	}

	else
	{
		OldFrameX = Percent::GetParcent(MaxHP, EnemyHP) * 2.0f;
		FrameScalingETime = 0.0f;
	}
	HPFrame[2]->SetSize({FrameScl.x, 15});
	HPFrame[1]->SetSize({200.0f, 15.0f});
	HPFrame[0]->SetSize({200.0f, 15.0f});
}

void MobEnemy::OBBSetParam()
{
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

	HandSiteOBB.SetOBBParam_Pos(Sword->GetMatWorld());
	HandSiteOBB.SetOBBParam_Rot(Sword->GetMatWorld());
	HandSiteOBB.SetOBBParam_Scl({8.0f, 30.0f, 8.0f});

	playerOBB.SetOBBParam_Pos(l_player->GetPosition());
	playerOBB.SetOBBParam_Rot(l_player->GetMatrot());
	playerOBB.SetOBBParam_Scl({1.0f, 9.0f, 1.0f});

	//���ガ�U��
	if (atcktype == SIDEAWAY)
	{
		//�A�j���[�V���������t���[���z������
		if (f_time >= NormalAttackTime + 1.0f)
		{
			if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true)
			{
				l_player->RecvDamage(5);
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
				l_player->RecvDamage(5);
			}
		}
	}
}


//�`�揈��
void MobEnemy::Draw()
{
	if (alpha >= 0.0f)
	{
		Draw_Fbx();

		// 3D�I�u�W�F�N�g�`��O����
		Object3d::PreDraw();
		Sword->Draw();
		Object3d::PostDraw();
	}
}

void MobEnemy::EnemyHPDraw()
{
	Sprite::PreDraw();
	for (int i = 0; i < 3; i++)
	{
		HPFrame[i]->Draw();
	}
	Sprite::PostDraw();
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

void MobEnemy::FbxAnimationControl()
{
	if (DeathFlag)
	{
		return;
	}
	//�A�j���[�V�����X�s�[�h
	float fbxanimationTime;

	if (nowAttack)
	{
		//�U����
		fbxanimationTime = 0.02f;
	}
	else
	{
		//�����Ƃ����S��
		fbxanimationTime = 0.01f;
	}

	//�q�b�g�X�g�b�v��
	if (PlayerAttackState::GetInstance()->GetHitStopJudg())
	{
		fbxanimationTime = 0.002f;
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
		if (cooltime > 480)
		{
			AfterAttack = false;
		}
	}
	else
	{
		cooltime = 0;
	}
}
