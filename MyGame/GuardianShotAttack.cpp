#include "GuardianShotAttack.h"
#include"CameraControl.h"
#include"EnemyControl.h"
#include "PlayerControl.h"
#include"Field.h"
#define PI 3.14f

GuardianShotAttack* GuardianShotAttack::GetIns()
{
	static GuardianShotAttack ins;
	return &ins;
}

void GuardianShotAttack::Finalize()
{
	DamageTex.reset(nullptr);
	for (int i = 0; i < ArmObjNum; i++)
	{
		ArmObj[i].reset(nullptr);
		ArmEffect[i].reset(nullptr);
	}
	PlayerDamageEffect.reset(nullptr);

}


void GuardianShotAttack::TexSet()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	Texture::LoadTexture(70, L"Resources/2d/icon/enemyicon.png");

	//�e�N�X�`���Z�b�g
	Texture* l_tex = Texture::Create(70);
	DamageTex.reset(l_tex);
	DamageTex->CreateTexture();
	DamageTex->SetAnchorPoint({0.5f, 0.5f});

	//������
	TexAlpha = 0.0f;
	TexScl = {0.0f, 0.f};

	for (int i = 0; i < ArmObjNum; i++)
	{
		ArmObj[i] = std::make_unique<Object3d>();
		ArmObj[i]->Initialize(camera);
		ArmObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::MISSILE));
		ArmAlpha[i] = 1.f;
		ArmShotF[i] = false;

		ArmEffect[i] = std::make_unique<Particle>();
		ArmEffect[i]->Init(64);
		ArmEffect[i]->SetParScl({1.f, 1.f});
		ArmEffect[i]->SetParColor({1.f, 0.2f, 0.2f, 1.f});
	}

	//�~�T�C���������̃G�t�F�N�g

	PlayerDamageEffect = std::make_unique<Particle>();
	PlayerDamageEffect->Init(64);
	PlayerDamageEffect->SetParScl({2.f, 2.f});
	PlayerDamageEffect->SetParColor({1.f, 0.2f, 0.2f, 1.f});

	DestroyEffectF = false;
	isEndAttack = false;
	phase = NON;
}

void GuardianShotAttack::Upda()
{
	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	switch (phase)
	{
	case NON:
		TexScl = {0.f, 0.f};
		TexAlpha = 0.0f;
		ColorT = 0.0f;
		break;

	case AREASET:
		Phase_AreaSet();
		break;

	case BOM:
		Phase_Bom();
		break;

	case END:
		Phase_End();
		break;
	}

	//�e�p�����[�^�̃Z�b�g
	if (phase == BOM)
	{
		for (int i = 0; i < ArmObjNum; i++)
		{
			if (ArmObj[i] == nullptr)
			{
				continue;
			}
			if (ArmEffect[i] == nullptr)
			{
				continue;
			}
			ArmObj[i]->SetPosition(ArmPos[i]);
			ArmObj[i]->SetScale({2.f, 2.f, 1.f});
			ArmObj[i]->SetRotation(ArmRot[i]);
			ArmObj[i]->SetColor({1.f, 1.f, 1.f, ArmAlpha[i]});
			ArmObj[i]->Update({1.f, 1.f, 1.f, ArmAlpha[i]}, camera);

			ArmEffect[i]->CreateParticle(true, {ArmPos[i]});
			ArmEffect[i]->Upda();
		}

		DestroyEffect();
	}
	TexAlpha = max(TexAlpha, 0.f);
}


void GuardianShotAttack::ArmShot()
{
	//�K�[�f�B�A���C���X�^���X
	Enemy* Guardian = EnemyControl::GetInstance()->GetGuardianEnemy();
	//�v���C���[���W
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	//�G���v�G���C���[�̕���������
	XMVECTOR positionA = {
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().y,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().z
	};
	//
	XMVECTOR positionB[ArmObjNum];
	XMVECTOR SubVector[ArmObjNum];
	float RotY[ArmObjNum];
	XMVECTOR move[ArmObjNum];
	XMMATRIX matRot[ArmObjNum];

	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmObj[i] == nullptr)
		{
			continue;
		}
		positionB[i] = {ArmPos[i].x, ArmPos[i].y, ArmPos[i].z};
		//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
		SubVector[i] = XMVectorSubtract(positionB[i], positionA); // positionA - positionB;

		//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
		RotY[i] = atan2f(SubVector[i].m128_f32[0], SubVector[i].m128_f32[2]);
		//�ړ��x�N�g����y������̊p�x�ŉ�]
		move[i] = {0.f, 0.f, 0.1f, 0};

		matRot[i] = XMMatrixRotationY(XMConvertToRadians(ArmRot[i].y));

		move[i] = XMVector3TransformNormal(move[i], matRot[i]);

		if (ArmShotF[i] == false)
		{
			ArmPos[i] = {
				EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition().x,
				-30.f,
				EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition().z
			};

			ArmRot[i] = {
				0,
				RotY[i] * 60.f + 180.f,
				0
			};

			if (ShotCount % 120 == 0)
			{
				ArmShotF[i] = true;
				break;
			}
		}
	}

	//�ł�
	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmObj[i] == nullptr)
		{
			continue;
		}

		if (ArmShotF[i] == false)
		{
			continue;
		}
		ArmPos[i] = {
			ArmPos[i].x + move[i].m128_f32[0] * 2.f,
			-30.f,
			ArmPos[i].z + move[i].m128_f32[2] * 2.f
		};
	}
	ColPlayer();
}

void GuardianShotAttack::Phase_AreaSet()
{
	bool nextPhase = TexAlpha > 3.f;
	const float scalingSpeed = 0.05f;
	const float maxScale = 8.f;


	phase = BOM;
}

void GuardianShotAttack::Phase_Bom()
{
	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();
	ShotCount++;
	ArmShot();
	if (ArmObj[ArmObjNum - 1] == nullptr)
	{
		phase = END;
	}
}

void GuardianShotAttack::Phase_End()
{
	EnemyControl::GetInstance()->GetGuardianEnemy()->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
}

void GuardianShotAttack::Draw()
{
	if (phase != BOM)
	{
		return;
	}
	Object3d::PreDraw();
	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmShotF[i] == false)
		{
			continue;
		}
		if (ArmObj[i] == nullptr)
		{
			continue;
		}
		ArmObj[i]->Draw();
	}
	Object3d::PostDraw();

	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmShotF[i] == false)
		{
			continue;
		}
		if (ArmEffect[i] == nullptr)
		{
			continue;
		}
		ArmEffect[i]->Draw();
	}
	PlayerDamageEffect->Draw();
}

void GuardianShotAttack::ColPlayer()
{
	//�v���C���[�̍��W
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	for (int i = 0; i < ArmObjNum; i++)
	{
		if (ArmShotF[i] == false)
		{
			continue;
		}
		if (ArmObj[i] == nullptr)
		{
			continue;
		}
		if (Collision::GetLength(ArmPos[i], ppos) < 5.0f)
		{
			ColMissilePos = ArmPos[i];
			ArmAlpha[i] -= 0.05f;
			//�Փˎ��ɉ��
			if (ArmAlpha[i] <= 0.0f)
			{
				ArmEffect[i].reset();
				ArmObj[i].reset();
			}
			DestroyEffectF = true;
		}
		if (Collision::GetLength(ArmPos[i], {-300.0f, -32, 270}) > 90)
		{
			ArmAlpha[i] -= 0.05f;
			//�͈͊O�łĂ����
			if (ArmAlpha[i] <= 0.0f)
			{
				ArmEffect[i].reset();
				ArmObj[i].reset();
			}
		}
	}
}

void GuardianShotAttack::DestroyEffect()
{
	//�v���C���[�̍��W
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	//�J�������W
	XMFLOAT3 cpos = CameraControl::GetInstance()->GetCamera()->GetEye();
	//�G�t�F�N�g�������W
	XMFLOAT3 epos = {ppos.x - ((ppos.x - cpos.x) / 2.f), ColMissilePos.y + 3.f, ppos.z - ((ppos.z - cpos.z) / 2.f)};
	if (DestroyEffectF)
	{
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(20);
		PlayerDamageEffect->SetParF(1);
		PlayerDamageEffect->CreateParticle(DestroyEffectF, {epos});
		DestroyEffectF = false;
	}
	PlayerDamageEffect->Upda_B();
}