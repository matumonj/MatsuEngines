#include "EnemyBeta.h"

#include "CameraControl.h"
#include"CustomButton.h"
#include"SphereCollider.h"
#include"DebugCamera.h"
#include"mHelper.h"
#include"imgui.h"
#include"ExpPointSystem.h"
#include"Collision.h"
#include "Field.h"
#include "ImageManager.h"
#include"PlayerControl.h"
#include"SceneManager.h"
/// <summary>
/// �R���X�g���N�^
/// </summary>
using namespace DirectX;

EnemyBeta::EnemyBeta()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyBeta::~EnemyBeta()
{
	Destroy_unique(DestRock);
	Destroy_unique(ThrowRockObj);
	//delete mob, MobModel;
}


void EnemyBeta::HPFrameInit()
{
	//�̗̓o�[������
	Sprite* l_frame1 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME1), {0, 0});
	Sprite* l_frame2 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME2), {0, 0});
	Sprite* l_frame3 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME4), {0, 0});
	Sprite* l_frame4 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME3), {0, 0});

	Sprite* l_enemyname = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENEMYNAME_MINIGOLEM), {0, 0});

	//�t���[��
	HPFrame[0].reset(l_frame1);
	HPFrame[1].reset(l_frame2);
	HPFrame[2].reset(l_frame3);
	HPFrame[3].reset(l_frame4);
	//�l�[���v���[�g
	EnemyName.reset(l_enemyname);

	for (int i = 0; i < 4; i++)
	{
		HPFrame[i]->SetAnchorPoint({0.0f, 0.0f});
	}
	FrameScl.x = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;

	//���Δ������̃G�t�F�N�g
	DestRock = std::make_unique<Particle>();
	DestRock->Init(64);
	DestRock->SetParScl({2.f, 2.f});
	DestRock->SetParColor({1.f, 1.f, 1.f, 1.f});
}

//����������
void EnemyBeta::Initialize()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	//�I�u�W�F�N�g�̐����Ə�����
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	//FBX����̏�����
	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetIns()->LoadModelFromFile("puchigolem"));
	m_fbxObject->LoadAnimation();
	m_fbxObject->PlayAnimation(0);

	//�΃I�u�W�F�̏�����
	ThrowRockObj = std::make_unique<Object3d>();
	ThrowRockObj->Initialize(camera);
	ThrowRockObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::THROWROCK));
	ThrowRockObj->SetScale({2.f, 2.f, 2.f});

	//�p�����[�^�̃Z�b�g
	//�̗�
	MaxHP = 100;
	EnemyHP = MaxHP;

	Rotation = {0.f, 0.f, 0.f};
	Scale = {0.025f, 0.025f, 0.025f};
	radius_adjustment = 0;

	SetCollider();
	//fbx�A�j���[�V�������Ԃ̐ݒ�
	NormalAttackTime = 1.5f;
	DeathTime = 6.f;

	nowAttack = false;
	nowDeath = false;
	m_fbxObject->SetColor({1, 0, 0, alpha});

	addRotRadians = 0.f;
	FollowRotAngleCorrect = 180.f;

	HPFrameInit();
	//�^�X�N�p�G�̎��
	ENumber = MINIGOLEM;
	//��ԏ�����
	state_mob->Initialize(this);
}

//�X�V����
void EnemyBeta::Update()
{
	if (m_fbxObject == nullptr)
	{
		return;
	}
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	state_mob->Update(this);

	if (DeathFlag && alpha > -1)
	{
		alpha -= 0.005f;
	}
	m_fbxObject->SetColor({1, 0, 0, alpha});

	//�A�j���[�V�����̃Z�b�g
	FbxAnimationControls(WALK, 1);
	FbxAnimationControls(ATTACK1, attackindex);
	FbxAnimationControls(DEATH, 2);

	//�U����̍d��
	AttackCoolTime();
	//�Ύ��̂̋���(state�̕��Ɉڂ��ׂ���)
	AttackMotion();

	m_Object->SetPosition(Position);
	m_fbxObject->SetPosition({Position.x, Position.y - 3.f, Position.z});
	m_fbxObject->SetRotation(Rotation);
	m_fbxObject->SetScale(Scale);
	m_fbxObject->SetFogPos(camera->GetEye());
	m_fbxObject->Update(m_AnimeLoop, m_AnimeSpeed, m_AnimationStop);

	m_fbxObject->SetHandBoneIndex(19);
	//�΃I�u�W�F�̍X�V
	ThrowRockObj->SetPosition(RockPos);
	ThrowRockObj->Update(camera);
	if (EnemyHP > 0)
	{
		if (animeState == DEATH)
		{
			animeState = WALK;
		}
	}
	if (Field::GetIns()->GetPedestalDown())
	{
		ReturnCreatePos = true;
	}
	//��_���\�L
	DamageTexDisplay();
	DamageParticleSet();

	AttackCol_Sideway();
	//�n�`�����蔻��
	CollisionField();
	//�̗͕\�L
	HPFrameUpda();
}

//�`�揈��
void EnemyBeta::Draw()
{
	if (m_fbxObject == nullptr)
	{
		return;
	}
	if (alpha > 0)
	{
		//���f��
		Draw_Fbx();
		Object3d::PreDraw();
		if (!turnoffdrawF)
		{
			//��
			ThrowRockObj->Draw();
		}
		Object3d::PostDraw();
		//�G�t�F�N�g�p�[�e�B�N��
		DestRock->Draw();
	}
	ImGui::Begin("k");
	ImGui::SliderInt("Rotation", &hindex, 0, 30);
	ImGui::End();
}

void EnemyBeta::Smoke(bool& createf)
{
}

void EnemyBeta::Death()
{
	if (!DeathFlag)
	{
		if (m_fbxObject->GetAnimeTime() >= m_fbxObject->GetEndTime() - 0.3f)
		{
			DeathFlag = true;
		}
	}
	if (m_fbxObject->GetAnimeTime() >= m_fbxObject->GetEndTime() - 0.3f)
	{
		alpha -= 0.02f;
	}
	movestop = false;
}

void EnemyBeta::FbxAnimationControl()
{
}

void EnemyBeta::EnemyHPDraw()
{
	if (alpha <= 0.f)
	{
		return;
	}
	//�v���C���[�̃C���X�^���X�擾
	Player* l_player = PlayerControl::GetIns()->GetPlayer();

	//��苗�����ꂽ��`��؂�
	bool TurnoffDrawJudg = Collision::GetLength(Position, l_player->GetPosition()) < 40.f;

	Sprite::PreDraw();
	if (TurnoffDrawJudg)
	{
		for (int i = 0; i < HPFrame.size(); i++)
		{
			HPFrame[i]->Draw();
		}
		EnemyName->Draw();
	}
	Sprite::PostDraw();
}


void EnemyBeta::Move()
{
}

void EnemyBeta::AttackCol_Rock()
{
	//�΂ƃv���C���[�̂�����͂�Ă�
	XMFLOAT3 l_playerpos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	const bool ColPlayer = Collision::GetLength(RockPos, l_playerpos) < 5.f;
	const bool ColGround = RockPos.y <= -32.f;

	if (ColPlayer)
	{
		if (turnoffdrawF == false && tmotion != PICK)
		{
			destF = true;
		}
	}
	if (destF)
	{
		turnoffdrawF = true;
		//PlayerControl::GetIns()->GetPlayer()->RecvDamage(10);
		DestRock->SetParF(1);
		DestRock->CreateParticle(destF, {RockPos});
		destF = false;
	}
	DestRock->Upda_B();
}

void EnemyBeta::AttackCol_Sideway()
{
	//�U����ԂłȂ���
	if (animeState != ATTACK1)
	{
		return;
	}
	//�A�j���[�V�����ԍ������U��ȊO�̂Ƃ�
	if (attackindex != 3)
	{
		return;
	}

	//�E��ƃv���C���[�̂�����͂�Ă�
	XMFLOAT3 l_playerpos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	//FBX�{�[��������W�����
	constexpr int boneindex = 10;
	constexpr float disrange = 20.f;
	constexpr int damage = 10;

	m_fbxObject->GetBoneIndexMat(boneindex, AttackHand_Right);
	RightHandPos = {
		AttackHand_Right.r[3].m128_f32[0], AttackHand_Right.r[3].m128_f32[1], AttackHand_Right.r[3].m128_f32[2]
	};

	//�������
	const bool ColPlayer = Collision::GetLength(RightHandPos, l_playerpos) < disrange;

	if (ColPlayer)
	{
		PlayerControl::GetIns()->GetPlayer()->RecvDamage(damage);
	}
}


void EnemyBeta::PickRock()
{
	//�E��ƃv���C���[�̂�����͂�Ă�
	XMFLOAT3 l_playerpos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	if (animeState != ATTACK1)
	{
		return;
	}
	if (attackindex != 0)
	{
		return;
	}
	if (tmotion == NON && m_fbxObject->GetAnimeTime() > 1.3 && m_fbxObject->GetAnimeTime() < 2.5)
	{
		tmotion = PICK;
	}


	if (tmotion == PICK)
	{
		if (m_fbxObject->GetAnimeTime() > 2.5)
		{
			tmotion = THROW;
		}
		RockPos = GetHandPos();
	}
	if (tmotion == THROW)
	{
		if (Collision::GetLength(l_playerpos, RockPos) < 20.f)
		{
			if (!turnoffdrawF)
			{
				PlayerControl::GetIns()->GetPlayer()->RecvDamage(10);
			}
		}
		XMVECTOR move = {0.0f, 0.0f, 0.1f, 0.0f};
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y));
		move = XMVector3TransformNormal(move, matRot);
		if (!destF)
		{
			//�����������ɐi��
			RockPos.y -= 0.1f;
			RockPos.x += move.m128_f32[0] * 7.f;
			RockPos.z += move.m128_f32[2] * 7.f;
		}
		if (GetAnimationTime() >= GetFbxTimeEnd() - 0.3f)
		{
			turnoffdrawF = false;
			tmotion = NON;
		}
	}
}

void EnemyBeta::AnimationContol(AnimationState name, int animenumber, double speed, bool loop)
{
	if (m_Number != animenumber)
	{
		m_AnimeLoop = loop;
		m_Number = animenumber;
		m_AnimeSpeed = speed;
		m_fbxObject->PlayAnimation(m_Number);
	}
}

void EnemyBeta::FbxAnimationControls(AnimationState motiontype, int number)
{
	if (DeathFlag)
	{
		return;
	}
	if (animeState == WALK && motiontype == WALK)
	{
		AnimationContol(WALK, number, 1.0, true);
	}
	else if (animeState != WALK && animeState == motiontype)
	{
		//FBX�^�C����������[�V�����J�n���ɍ��킹��
		AnimationContol(motiontype, number, 0.8, false);
		m_AnimationStop = true;
	}
}

void EnemyBeta::AttackMotion()
{
	if (animeState == ATTACK1 && attackNum == IDLE)
	{
		if (Collision::GetLength(Position, PlayerControl::GetIns()->GetPlayer()->GetPosition()) < 13.f)
		{
			attackNum = SIDEWAY;
		}
		else
		{
			attackNum = THROWRROCK;
		}
	}
	if (attackNum == THROWRROCK)
	{
		attackindex = 0;
		if (GetAnimationTime() >= GetFbxTimeEnd() - 0.3f)
		{
			attackNum = IDLE;
		}
	}
	else if (attackNum == SIDEWAY)
	{
		attackindex = 3;
		if (GetAnimationTime() >= GetFbxTimeEnd() - 0.3f)
		{
			attackNum = IDLE;
		}
	}
	if (SceneManager::GetIns()->GetScene() != SceneManager::MAPCREATE)
	{
		PickRock();

		AttackCol_Rock();
	}
}


void EnemyBeta::AttackCoolTime()
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

void EnemyBeta::DamageParticleSet()
{
}

XMFLOAT3 EnemyBeta::GetHandPos()
{
	return XMFLOAT3({
		m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[0], // GetPosition().x;
		m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[1], // GetPosition().x;
		m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[2] // GetPosition().x;
	});
}
