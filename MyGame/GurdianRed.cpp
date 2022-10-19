#include"GuardianEnemy.h"
#include"Collision.h"
#include"PlayerControl.h"
void GuardianRed::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	EnemyHP = 300.0f;
	MaxHP = 300.0f;
	//�p�����[�^�̃Z�b�g
	Rotation = { -70.0f,180.0f,0.0f };
	Scale = { 0.02f, 0.02f, 0.02f };

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem"));
	m_fbxObject->PlayAnimation();
	m_fbxObject->SetColor({ 1,0,0,alpha });
	//�R���C�_�[����
	radius_adjustment = 0;
	SetCollider();

	//FBX�؂�ւ��̃^�C���w��
	AttackTime = 1.5f;
	DeathTime = 4.9f;

	//nowAttack = false;
	nowDeath = false;

	//state������
	state_mob->Initialize(this);

	//
	Texture::LoadTexture(93, L"Resources/ParticleTex/slash.png");
	SlashTex = Texture::Create(93, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
	SlashTex->CreateTexture();
	SlashTex->SetAnchorPoint({ 0.5f,0.5f });
	SlashPos = { Position.x,Position.y,Position.z };

	//�p�[�e�B�N���Z�b�g
	ParticleManager::LoadTexture(4, L"Resources/ParticleTex/Normal.png");
	particleMan = ParticleManager::Create(4, L"Resources/ParticleTex/Attack.png");
	ParticleManager::LoadTexture(6, L"Resources/ParticleTex/Attack.png");
	particleMan2 = ParticleManager::Create(6, L"Resources/ParticleTex/Attack.png");

}

void GuardianRed::Update(DebugCamera* camera)
{
	state_mob->Update(this);

	if (EnemyHP <= 0) {
		alpha -= 0.005f;
	}
	HandMat = m_fbxObject->GetRot();

	HandSiteOBB.SetOBBParam_Pos(HandMat);
	HandSiteOBB.SetOBBParam_Rot(HandMat);
	HandSiteOBB.SetOBBParam_Scl({ 2.0f,2.0f,2.0f });

	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());
	playerOBB.SetOBBParam_Scl({ 1.0f,1.0f,1.0f });

	if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true) {
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
	}
	FbxAnimationControl();

	EnemyPop(150);

	AttackCoolTime();

	ParameterSet_Fbx(camera);

	CollisionField(camera);


	//DamageParticleSet();
}

void GuardianRed::Draw()
{
	Draw_Fbx();
	// 3D�I�u�W�F�N�g�`��O����
	ParticleManager::PreDraw();
	// 3D�I�u�N�W�F�N�g�̕`��
	particleMan->Draw();
	particleMan2->Draw();
	// 3D�I�u�W�F�N�g�`��㏈��
	ParticleManager::PostDraw();
	Texture::PreDraw();

	Texture::PostDraw();
}

//���S����
void GuardianRed::Death()
{
	if (f_time < DeathTime) {
		DeathFlag = true;
	}
}

//�U����̃N�[���^�C������
void GuardianRed::AttackCoolTime()
{
	if (f_time >= DeathTime - 1) {
		AfterAttack = true;
	}
	if (AfterAttack) {
		cooltime++;
		if (cooltime > 480) {
			AfterAttack = false;
		}
	} else {
		cooltime = 0;
	}
}

//Fbx�̃A�j���[�V��������
void GuardianRed::FbxAnimationControl()
{
	//1�t���[���i�߂�
	f_time += 0.02f;

	if (f_AttackFlag) {
		f_time = AttackTime;
		f_AttackFlag = false;
		nowAttack = true;
	} else {
		if (nowDeath == false) {
			if (!nowAttack && f_time >= AttackTime) {
				f_time = 0.0f;
			}
		}
	}

	if (DeathFlag) {
		f_time = DeathTime;
		nowDeath = true;
		DeathFlag = false;

	}
	if (f_time > DeathTime) {
		nowAttack = false;
	}

	m_fbxObject->SetFbxTime(f_time);
}

void GuardianRed::DamageTexUpdate(DebugCamera* camera)
{

}