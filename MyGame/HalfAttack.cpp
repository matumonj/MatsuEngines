#include "HalfAttack.h"
#include"EnemyControl.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include "MobEnemy.h"

HalfAttack::~HalfAttack()
{
	//delete HalfAreaTex;
}

HalfAttack* HalfAttack::GetInstance()
{
	static HalfAttack instance;
	return &instance;
	
}

void HalfAttack::Init()
{
	SummonInit();
	phase = PHASEONE;

	summonEnemyCreate = true;
	
}

bool HalfAttack::SummonEnemy()
{
	if (phase == PHASETWO)
	{
		return true;
	}
	return false;
}
void (HalfAttack::* HalfAttack::actionTable[])() = {
	nullptr,
	&HalfAttack::BossLeaveGround,
	&HalfAttack::SummonUpdate,
	&HalfAttack::BossReturnGround,
	&HalfAttack::SummonAttackEnd,
};
void HalfAttack::Upda()
{
	//�{�X�̃C���X�^���X�����Ă���
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	if (boss == nullptr)return;

	(this->*actionTable[phase])();
}

void HalfAttack::SummonInit()
{
	//�{�X����������U�R�G
	for (int i = 0; i < EnemySize; i++)
	{
		SummonEnemys[i] = std::make_unique<MobEnemy>();
		SummonEnemys[i]->Initialize();
		SummonEnemys[i]->SetPosition({ 0, -20, 20 });

	}
	//�G�����t���O
	summonEnemyCreate = false;
	//�G���S�t���O
	SummonEnemysDeath = false;
	//�G�o��ς݃t���O
	SummonEnemysApper = false;

	SummonEPos = { 1.f, 1.f, 1.f };

	Shieldalpha = 0.0f;

	//���e�N�X�`��
	Texture* l_shield[2];
	Texture::LoadTexture(101, L"Resources/2d/attackEffect/ma1b-skyblue.png");
	for (int i = 0; i < 2; i++)
	{
		l_shield[i] = Texture::Create(101, { 1, 1, 1 }, { 0, 0, 0 }, { 1, 1, 1, 1 });
		ShieldTex[i].reset(l_shield[i]);
		ShieldTex[i]->CreateTexture();
		ShieldTex[i]->SetAnchorPoint({ 0.5f, 0.5f });
		ShieldTex[i]->SetRotation({ 90, 0, 0 });
	}
	
}

void HalfAttack::BossLeaveGround()
{
	/*�U�����e�̏����Ȃ̂Ō�ōU����p�̃N���X�Ɉڂ�*/
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	//�{�X���W
	XMFLOAT3 bosspos=boss->GetPosition();

	const double JumpEndTime = 1.2;
	const float JumpSpeed = 4.f;

	if (boss->GetNowMotion() != boss->BJUMP)return;

		if(boss->GetAnimationTime()>JumpEndTime)
		{
			bosspos.y+=JumpSpeed;
		}

	//�{�X�����ȏ��Ɉړ�������
	bool nextphase = bosspos.y > 80.f;

	boss->SetPosition(bosspos);

	if(nextphase)
	{
		phase = PHASETWO;
	}
}

void HalfAttack::SummonUpdate()
{
	//�J�����̃C���X�^���X�����Ă���
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	/*�U�����e�̏����Ȃ̂Ō�ōU����p�̃N���X�Ɉڂ�*/
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();

	
	SummonEPos.y += 0.1f; //���X�ɏ��

	for (int i = 0; i < EnemySize; i++)
	{
		if (SummonEnemys[i] == nullptr)
			{
				continue;
			}
		//�o�ꂵ�؂�܂�
			if (SummonEPos.y < 18.0f)
			{
				ShieldTexPos[0] = { boss->GetPosition().x + 30.f,19.f,boss->GetPosition().z };
				ShieldTexPos[1] = { boss->GetPosition().x -30.f,19.f,boss->GetPosition().z };
				//�G���v���C���[���W�܂Ō��ꂽ��
				Shieldalpha += 0.01f;
				//�������ɏo�Ă���ۂ͓����~�߂Ă���
				SummonEnemys[i]->SetMoveStop(true);
				SummonEnemys[0]->SetPosition({ShieldTexPos[0].x, SummonEPos.y,ShieldTexPos[0].z });
				SummonEnemys[1]->SetPosition({ ShieldTexPos[1].x, SummonEPos.y,ShieldTexPos[1].z });
			} else
			{
				Shieldalpha -= 0.05f;
				
				//�����~�߂Ă����̂�����
				SummonEnemys[i]->SetMoveStop(false);
			}
			//�X�V
			SummonEnemys[i]->SetColors({ 1.0f, 0.2f, 0.2f, 1.0f });

			SummonEnemys[i]->Update();
		}
	

	//�U�R�G�����Ƃ��|������
	if (SummonEnemys[0] == nullptr && SummonEnemys[1] == nullptr)
	{
		phase = PHASETHREE;
		//���e�N�X�`������
		SummonEnemysDeath = true;
	}
	//���e�N�X�`���̃A���t�@�l�����ȉ��Ȃ�����e�N�X�`���C���X�^���X�͏���
	if (Shieldalpha < -1.0f)
	{
		for (int i = 0; i < 2; i++)
		{
			Destroy_unique(ShieldTex[i]);
		}
	}
	//�G�o��ς݂̃t���O=�G���v���C���[��Y���W�܂ŏオ���Ă�����
	SummonEnemysApper = SummonEPos.y >= 10.0f;

	for (int i = 0; i < 2; i++)
	{
		if (ShieldTex[i] == nullptr)
		{
			continue;
		}
		//�e�N�X�`����
		ShieldRot++;
		ShieldTex[i]->SetBillboard(false);
		ShieldTex[i]->SetColor({ 1.0f, 1.0f, 1.0f, Shieldalpha });
		ShieldTex[i]->SetPosition({ ShieldTexPos[i] });
		ShieldTex[i]->SetScale({ 5.0f, 5.0f, 1.0f });
		ShieldTex[i]->SetRotation({ 90.f,0.f,ShieldRot });
		ShieldTex[i]->Update(camera);
	}

	//�U�R�G�̊J������
	for (int i = 0; i < 2; i++)
	{
		if (SummonEnemys[i] == nullptr)
		{
			continue;
		}
		if (SummonEnemys[i]->GetObjAlpha() <= 0)
		{
			Destroy_unique(SummonEnemys[i]);
		}
	}

	//���e�N�X�`���̃A���t�@�l�̏���Ɖ���
	Shieldalpha = min(Shieldalpha, 1);
	Shieldalpha = max(Shieldalpha, 0);
	SummonEPos.y = min(SummonEPos.y, 18);
}

void HalfAttack::BossReturnGround()
{
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	XMFLOAT3 bosspos = boss->GetPosition();

	bosspos.y--;
	
	if(bosspos.y<14.7f)
	{
		phase = PHASEFOUR;
	}
	boss->SetPosition(bosspos);

}


void HalfAttack::Draw_SummonEnemyHP()
{
	for (int i = 0; i < SummonEnemys.size(); i++)
	{
		if (SummonEnemys[i] == nullptr)
		{
			continue;
		}
		if (SummonEnemys[i]->GetPosition().y > 17.f) {
			SummonEnemys[i]->EnemyHPDraw();
		}
	}
}

void HalfAttack::Draw()
{
	//�����G�̕`��
	Object3d::PreDraw();
	for (int i = 0; i < EnemySize; i++)
	{
		if (SummonEnemys[i] == nullptr)
		{
			continue;
		}
		SummonEnemys[i]->Draw();
	}
	Object3d::PostDraw();
	//�{�X�̓\��V�[���h�e�N�X�`��
	Texture::PreDraw();

	for (int i = 0; i < 2; i++)
	{
		ShieldTex[i]->Draw();
	}

	Texture::PostDraw();
}

void HalfAttack::SummonEnemyResetParam()
{
	for (int i = 0; i < EnemySize; i++)
	{
		SummonEnemys[i].reset(new MobEnemy());
		SummonEnemys[i]->Initialize();

		SummonEnemys[i]->SetPosition({ 0, -20, 20 });
	}
	//�G�����t���O
	summonEnemyCreate = false;
	//�G���S�t���O
	SummonEnemysDeath = false;
	//�G�o��ς݃t���O
	SummonEnemysApper = false;

	SummonEPos = { 1, 1, 1 };
	Shieldalpha = 0.0f;

	phase = PHASEONE;
}

void HalfAttack::SummonAttackEnd()
{
	
}
