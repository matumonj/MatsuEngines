#include "SelectSword.h"
#include"BigSword.h"
#include"SmallSword.h"
#include"NormalSword.h"
#include"CameraControl.h"
#include"mHelper.h"
#include"ModelManager.h"
#include"HUD.h"
#include"imgui.h"
#include"DropWeapon.h"
#include"EnemyControl.h"
#include"SceneManager.h"
#include"PlayerControl.h"

SelectSword::~SelectSword()
{
	delete SwordSample[0], SwordSample[1], SwordSample[2], Frame;
}

SelectSword* SelectSword::GetIns()
{
	static SelectSword instance;
	return &instance;
}

void SelectSword::Finalize()
{
	delete SwordSample[0], SwordSample[1], SwordSample[2], Frame;
}

void SelectSword::SpriteSet()
{
	Sprite::LoadTexture(228, L"Resources/2d/icon/SelectWeaponWood.png");
	Sprite::LoadTexture(229, L"Resources/2d/icon/SelectWeaponSword.png");

	Sprite::LoadTexture(230, L"Resources/2d/icon/SelectWeaponAxe.png");
	Sprite::LoadTexture(231, L"Resources/2d/icon/SelectWeaponFrame.png");
	Sprite::LoadTexture(232, L"Resources/2d/icon/SelectWeaponSword.png");
	Sprite::LoadTexture(233, L"Resources/2d/icon/pin.png");
	Sprite::LoadTexture(234, L"Resources/2d/Button/RB.png");
	Sprite::LoadTexture(235, L"Resources/2d/Button/LB.png");

	/// <summary>
	/// ����Sprite����
	/// </summary>
	SwordSample[WAND] = Sprite::Create(232, {1.0f, 1.0f});
	SwordSample[SWORD] = Sprite::Create(228, {1.0f, 1.0f});
	SwordSample[AXE] = Sprite::Create(230, {1.0f, 1.0f});

	/// <summary>
	/// ����̃t���[������
	/// </summary>
	WeaponFrame[0] = Sprite::Create(231, {1.0f, 1.0f});
	WeaponFrame[1] = Sprite::Create(231, {1.0f, 1.0f});
	WeaponFrame[2] = Sprite::Create(231, {1.0f, 1.0f});

	//�I�𒆂̕�����w���e�N�X�`��
	Frame = Sprite::Create(233, {1.0f, 1.0f});
	Frame->SetAnchorPoint({0.5f, 0.5f});

	//LB,RB
	Button_RB = Sprite::Create(234, {1.0f, 1.0f});
	Button_RB->SetAnchorPoint({0.5f, 0.5f});
	Button_LB = Sprite::Create(235, {1.0f, 1.0f});
	Button_LB->SetAnchorPoint({0.5f, 0.5f});

	RBPos = {1830.0f, 250.f};
	RBScl = {230.0f, 200.0f};
	LBPos = {1400.0f, 250.f};
	LBScl = {230.0f, 200.0f};
	/// <summary>
	/// �A���t�@�l������(�������ŏ�����)
	/// </summary>
	sampleSwordAlpha[WAND] = 0.0f;
	sampleSwordAlpha[SWORD] = 1.0f;
	sampleSwordAlpha[AXE] = 0.0f;

	WeapomScl = {1.f, 1.f, 1.f};
	WeaponChangeEffect = std::make_unique<Particle>();
	WeaponChangeEffect->Init(65);
}

void SelectSword::Initialize()
{
	/// <summary>
	/// ����̃I�u�W�F�N�g����
	/// </summary>
	Sword = std::make_unique<NormalSword>();
	Sword->Initialize();

	SpriteSet();

	input = Input::GetIns();

	index = 1;

	TurnOffDrawF = false;
}

void SelectSword::SetSword(SelectWeapon nowsword)
{
	switch (nowsword)
	{
	case SWORD: //�ʏ핐��
		Sword.reset(new NormalSword());
		Sword->Initialize();
		break;

	case AXE: //�S�[������
		if (sampleSwordAlpha[AXE] >= 1.0f)
		{
			Sword.reset(new BigSword());
			Sword->Initialize();
			//sampleSwordAlpha[AXE] = 0.0f;
		}
		break;

	case WAND: //���̏�

		if (sampleSwordAlpha[WAND] >= 1.0f)
		{
			Sword.reset(new SmallSword());
			Sword->Initialize();
			//sampleSwordAlpha[WAND] = 0.0f;
		}
		break;
	default:
		break;
	}
}

void SelectSword::Update()
{

	//����ύX�{�^�������ꂽ��
	if (!SelectJudg_Right && !SelectJudg_Left)
	{
		EaseTime = 0.0f;
		//�g�X�v���C�g�̈ʒu���(�C�[�W���O�p)
		for (int i = 0; i < 3; i++)
		{
			oldpos[i] = Position[i];
		}
		//�E�Ɉړ�
		if (input->TriggerButton(input->RB))
		{

			if (SceneManager::GetIns()->GetScene() != SceneManager::TUTORIAL)
			{
				if (index == 2)
				{
					//�ő�l�s������ŏ��ɖ߂�
					index = 0;
				} else
				{

					index++;
			}
			SelectJudg_Right = true;
		}
		}
		//���Ɉړ�
		if (input->TriggerButton(input->LB))
		{
			if (SceneManager::GetIns()->GetScene() != SceneManager::TUTORIAL)
			{
				if (index == 0)
				{
					//�ŏ��l�s�������ԍŌ��
					index = 2;
				} else
				{
					index--;
				}
				SelectJudg_Left = true;
			}
		}
	}

	//�E�Ɉړ�
	if (SelectJudg_Right)
	{
		//�C�[�W���O�I�������t���O�؂�
		if (EaseTime >= 1.0f)
		{
			SelectJudg_Right = false;
		}
		else
		{
			//�t���O��������
			for (int i = 0; i < 3; i++)
			{
				Position[i].x = Easing::EaseOut(EaseTime, oldpos[i].x, oldpos[i].x + 200);
				//�g���E�[�������獶�[��
				if (Position[i].x > 1800.f)
				{
					Position[i].x = 1300.f;
				}
			}
			//�C�[�W���O�i�߂�
			EaseTime += 0.02f;
		}
	}

	//���Ɉړ�
	if (SelectJudg_Left)
	{
		if (EaseTime >= 1.0f)
		{
			//�C�[�W���O�I�������t���O�؂�
			SelectJudg_Left = false;
		}
		else
		{
			//�t���O��������
			for (int i = 0; i < 3; i++)
			{
				Position[i].x = Easing::EaseOut(EaseTime, oldpos[i].x, oldpos[i].x - 200);
				//�g�����[�s������E�[�Ɉړ�
				if (Position[i].x < 1200.f)
				{
					Position[i].x = 1700.f;
				}
			}
			//�C�[�W���O�i�߂�
			EaseTime += 0.02f;
		}
	}

	//RB,LB�����ꂽ�畐��X�V
	if (input->TriggerButton(input->RB) || input->TriggerButton(input->LB))
	{
		if (SceneManager::GetIns()->GetScene() != SceneManager::TUTORIAL)
		{
			//����ԍ��ƌ��ݑ������Ă镐��𑵂���
			//��
			if (index == SWORD)
			{
				NowSelectSword = SWORD;
			}
			//�S�[������
			else if (index == WAND)
			{
				NowSelectSword = AXE;
			}
			//��
			else if (index == AXE)
			{
				NowSelectSword = WAND;
			}
			weaponscalingf = true;
		}
	}
	if (weaponscalingf)
	{
		WeapomScl.x -= 0.02f;
		WeapomScl.y -= 0.02f;
		WeapomScl.z -= 0.02f;
		if (WeapomScl.x <= 0.0f)
		{
			WeaponChangeEffect->SetParF(0);
			SetSword(NowSelectSword);
			weaponscalingf = false;
		}
	}
	else
	{
		WeapomScl.x += 0.02f;
		WeapomScl.y += 0.02f;
		WeapomScl.z += 0.02f;
	}
	//�G�����Ƃ�������������畀�e�N�X�`���\��
	if (DropWeapon::GtIns()->PickUpWeapon(DropWeapon::AXE) == true)
	{
		sampleSwordAlpha[AXE] += 0.02f;
	}
	if (DropWeapon::GtIns()->PickUpWeapon(DropWeapon::SWORD) == true)
	{
		sampleSwordAlpha[WAND] += 0.02f;
	}
	WeaponChangeEffect->CreateParticle(WeapomScl.x <= 0.0f, PlayerControl::GetIns()->GetPlayer()->GetHandPos());

	Sword->SetScale(WeapomScl);
	//����I�u�W�F�N�g�̍X�V����
	Sword->Update();

	WeaponChangeEffect->Upda_B();
	//����e�N�X�`���̍X�V
	for (int i = 0; i < 3; i++)
	{
		SwordSample[i]->SetPosition(Position[i]);
		SwordSample[i]->SetSize(Scale[i]);
		SwordSample[i]->setcolor({1.0f, 1.0f, 1.0f, sampleSwordAlpha[i]});
		WeaponFrame[i]->SetPosition(Position[i]);
		WeaponFrame[i]->SetSize(Scale[i]);
	}
	//�I�𒆂̕�����w���e�N�X�`���͊g�k������
	FrameScalingTime++; //�g�k�̎����p
	//230��240�̊ԂŊg�k
	Frame->SetSize({
		230.0f + sinf(3.14f * 2.f / 90.f * FrameScalingTime) * 10.f,
		230.0f + sinf(3.14f * 2.f / 90.f * FrameScalingTime) * 10.f
	});
	Frame->SetPosition({1600, 130});

	Button_LB->SetPosition(LBPos);
	Button_LB->SetSize(LBScl);

	Button_RB->SetPosition(RBPos);
	Button_RB->SetSize(RBScl);

	WeaponScalingETime = min(WeaponScalingETime, 1.0f);
	WeaponScalingETime = max(WeaponScalingETime, 0.0f);

	WeapomScl.x = min(WeapomScl.x, 1.0f);
	WeapomScl.y = min(WeapomScl.y, 1.0f);
	WeapomScl.z = min(WeapomScl.z, 1.0f);

	WeapomScl.x = max(WeapomScl.x, 0.0f);
	WeapomScl.y = max(WeapomScl.y, 0.0f);
	WeapomScl.z = max(WeapomScl.z, 0.0f);
}

void SelectSword::Draw()
{
	SpriteDraw();
}

void SelectSword::SpriteDraw()
{
	if (TurnOffDrawF)
	{
		return;
	}
	Sprite::PreDraw();

	for (int i = 0; i < 3; i++)
	{
		WeaponFrame[i]->Draw();
		SwordSample[i]->Draw();
	}
	Frame->Draw();
	Button_RB->Draw();
	Button_LB->Draw();
	Sprite::PostDraw();
}


void SelectSword::SwordDraw()
{
	Sword->Draw();
	WeaponChangeEffect->Draw();
	AttackEffect::GetIns()->Draw();
}
