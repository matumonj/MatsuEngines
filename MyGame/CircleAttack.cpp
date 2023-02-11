#include "CircleAttack.h"
#include"mHelper.h"
#include"Nail.h"
#include"BossSpell.h"
#include "CameraControl.h"
#include"Collision.h"
#include "Enemy.h"
#include "EnemyControl.h"
#include"imgui.h"
#include "PlayerControl.h"


CircleAttack::~CircleAttack()
{
	//delete ImpactAreaTex;
}

CircleAttack* CircleAttack::GetIns()
{
	static CircleAttack instance;
	return &instance;
}

void CircleAttack::Init()
{
	Texture::LoadTexture(23, L"Resources/2d/icon/enemyicon.png");

	ImpactAreaTex = Texture::Create(23, {0.0f, 0.0f, 0.0f}, {100.0f, 100.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
	ImpactAreaTex->CreateTexture();
	ImpactAreaTex->SetAnchorPoint({0.5f, 0.5f});

	Direction[NORTH] = {0.0f, 0.0f, 60.0f};
	Direction[SOUTH] = {0.0f, 0.0f, -60.0f};
	Direction[EAST] = {60.0f, 0.0f, 0.0f};
	Direction[WEST] = {-60.0f, 0.0f, 0.0f};

	phase = PHASENON;
	CircleSize = {0, 0};
	TexAlpha = 0.0f;
}

void CircleAttack::Upda()
{
	Enemy* boss = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0].get();
	switch (phase)
	{
	case PHASENON:
		break;
	case PHASEONE:
		//�B�h����܂�
		PierceNail();
		break;
	case PHASETWO:
		//�_���[�W�G���A�̉~�L����܂�
		DamageAreaTexSet();
		break;
	case PHASETHREE:
		//�n�ʂ���B�o�Ă��܂�
		ProtrudeNail();
		break;
	case PHASEFOUR:

		//�U���I���ł�
		EndAttackAction();
		break;
	default:
		break;
	}
	rotY += 1.0f;
	if (rotY > 360.0f)
	{
		rotY = 0.0f;
	}
	//�_���[�W�G���A�e�N�X�`���̊e��p�����[�^

	ImpactAreaTex->Update(CameraControl::GetIns()->GetCamera());
	ImpactAreaTex->SetScale({CircleSize.x, CircleSize.y, 3.0f});
	ImpactAreaTex->SetRotation({90.0f, 0.0f, rotY});
	ImpactAreaTex->SetColor({1.0f, 1.0f, 1.0f, TexAlpha});

	ImpactAreaTex->SetPosition({boss->GetPosition().x, 18, boss->GetPosition().z});

	//�B�I�u�W�F�̍X�V
	for (int i = 0; i < NailObj.size(); i++)
	{
		NailObj[i]->SetScale({3.0f, 3.0f, 3.0f});
		NailObj[i]->Update(CameraControl::GetIns()->GetCamera());
	}
	TexAlpha = min(TexAlpha, 1.0f);
	TexAlpha = max(TexAlpha, 0.0f);

	Nail::GetIns()->Update();
}

void CircleAttack::Draw()
{
	Texture::PreDraw();

	ImpactAreaTex->Draw();

	Texture::PostDraw();
	if (phase == PHASETHREE)
	{
		for (int i = 0; i < NailObj.size(); i++)
		{
			Object3d::PreDraw();
			NailObj[i]->Draw();
			Object3d::PostDraw();
		}
	}
	Nail::GetIns()->Draw();
}

void CircleAttack::CollisonNailPlayer()
{
	const int Damage = 20;

	if (Collision::GetLength(NailObj[0]->GetPosition(), PlayerControl::GetIns()->GetPlayer()->GetPosition()) < 300)
	{
		//PlayerControl::GetIns()->GetPlayer()->RecvDamage(300);
	}
}

void CircleAttack::PierceNail()
{
	TexAlpha = 1.0f;

	//�B����
	NailObj.resize(1);
	for (int i = 0; i < NailObj.size(); i++)
	{
		NailObj[i] = std::make_unique<Object3d>();
		//�t�B�[���h�Ƀ��f�����蓖��
		NailObj[i]->Initialize(CameraControl::GetIns()->GetCamera());
		NailObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NAIL));
	}
	//�r���{�[�h�؂�(�W����true�Ȃ̂ŁA�A)
	ImpactAreaTex->SetBillboard(false);

	//Direction.y->�B��Y���W�Ɠ���
	if (Direction[Area1].y > -17.0f)
	{
		Direction[Area1].y--;
		Direction[Area2].y--;
	}
	else
	{
		phase = PHASETWO;
	}
	//���W���킹��
	NailObj[0]->SetPosition(Direction[Area1]);
}

void CircleAttack::DamageAreaTexSet()
{
	const float EaseC = 0.01f;
	const XMFLOAT2 DamageAreaTex_Max = {10.0f, 10.0f};
	if (Nail::GetIns()->GetEndAction_Circle())
	{
		Nail::GetIns()->SetEndAction_Circle(false);
	}
	//�_���[�W�G���A�̉~�Ђ낪��
	CircleAreaTime += EaseC;

	if (CircleAreaTime <= 1.0f)
	{
		CircleSize.x = Easing::EaseOut(CircleAreaTime, 0.0f, DamageAreaTex_Max.x);
		CircleSize.y = Easing::EaseOut(CircleAreaTime, 0.0f, DamageAreaTex_Max.y);
	}
	AttackCount = 0;

	if (CircleAreaTime >= 2.0f)
	{
		phase = PHASETHREE; //�~�̑傫�����ő咴�����玟��
	}
}

void CircleAttack::ProtrudeNail()
{
	Nail::GetIns()->CircleAttack(Area1, Area2);

	CollisonNailPlayer();

	CircleAreaTime = 0;
	Direction[Area1].y++;
	Direction[Area2].y++;

	XMFLOAT3 Bpos = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
	//float Ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition().x;
	XMFLOAT3 Ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	NailObj[0]->SetPosition(Direction[Area1]);
	TexAlpha -= 0.01f;
	if (Nail::GetIns()->GetEndAction_Circle())
	{
		phase = PHASEFOUR;
	}
}

void CircleAttack::EndAttackAction()
{
	TexAlpha = 0.5f;

	Direction[NORTH] = {0.0f, 0.0f, 60.0f};
	Direction[SOUTH] = {0.0f, 0.0f, -60.0f};
	Direction[EAST] = {60.0f, 0.0f, 0.0f};
	Direction[WEST] = {-60.0f, 0.0f, 0.0f};

	NailObj[0]->SetPosition(Direction[Area1]);

	CircleSize = {0.0f, 0.0f};
	NailObj.clear();
}

