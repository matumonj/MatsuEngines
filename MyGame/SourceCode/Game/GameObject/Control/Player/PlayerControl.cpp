#include "PlayerControl.h"

#include <algorithm>

#include"SceneManager.h"
#include"TutorialSprite.h"
#include"PlayerAttackState.h"
#include"AttackCollision.h"
#include "AttackEffect.h"
#include "EnemyControl.h"

#include"HUD.h"
#include"CameraControl.h"

PlayerControl* PlayerControl::GetIns()
{
	static PlayerControl instance;
	return &instance;
}

void PlayerControl::Init_Tutorial()
{
	Sprite::LoadTexture(190, L"Resources/2d/damage/playerdamage.png");

	DamageTex = Sprite::Create(190, {0, 0});
	DamageTex->SetSize({1900, 1000});
	player = std::make_unique<Player>();
	player->Initialize();

	StartPos = {62.0f, -20.0f, -386.0f};

	player->SetPosition(StartPos);
	AttackCollision::GetIns()->Init();
}

void PlayerControl::Init_Play()
{
	StartPos = {53.0f, -20.0f, -189.0f};

	player->SetHP(player->GetMaxHP());

	player->SetPosition(StartPos);
	AttackCollision::GetIns()->Init();
}

void PlayerControl::Init_Boss()
{
	StartPos = {-1.0f, 10.0f, -80.0f};
	player->SetHP(player->GetMaxHP());
	HUD::GetIns()->SetMax();
	player->SetPosition(StartPos);
	AttackCollision::GetIns()->Init();
}

/*------------------------*/
/*--------�������---------*/
/*------------------------*/
void PlayerControl::Finalize()
{
	AttackCollision::GetIns()->Finalize();
	//Destroy_unique(player);
}

/*------------------------*/
/*--------�Ǎ�����---------*/
/*----------scv----------*/
void PlayerControl::Load()
{
	//�����ʒu
	switch (SceneManager::GetIns()->GetScene())
	{
	case SceneManager::TUTORIAL:
		StartPos = {92.0f, -50.0f, -760.0f};
		break;
	case SceneManager::PLAY:
		StartPos = {110.0f, -12.0f, -379.0f};
		break;
	case SceneManager::BOSS:
		StartPos = {0.0f, 12.0f, -50.0f};
		break;
	default:
		break;
	}

	player->SetPosition(StartPos);
	AttackCollision::GetIns()->Init();
}

void PlayerControl::GameOverResetParam()
{
	//���S���̈ʒu�Ƒ̗͂̃��Z�b�g
	player->SetPosition(StartPos);
	player->ReStartSetParam();
	player->SetHP(player->GetMaxHP());
}

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
//player�̒��ɂ���ړ������Ƃ��͌�ł����������Ă���
void PlayerControl::Update_Tutorial() //�`���[�g���A����
{
	if (player == nullptr)
	{
		return;
	}

	PlayerAttackState::GetIns()->Update();
	player->Update();
	DamageTexUpdate();
}

void PlayerControl::Update_Play() //�v���C�V�[����
{
	PlayerAttackState::GetIns()->Update();
	player->Update();

	TargetMarker::GetIns()->Update_PlayScene(CameraControl::GetIns()->GetCamera());

	DamageTexUpdate();
}

void PlayerControl::Update_Boss()
{
	PlayerAttackState::GetIns()->Update();
	//�{�X���̍U���G�t�F�N�g(�K�[�h��)
	if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
	{
		XMFLOAT3 rot = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetRotation();
		XMFLOAT3 spos = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->HandRightPos2();

		AttackEffect::GetIns()->SetGuardRot(rot);
		AttackEffect::GetIns()->GuarEffect(spos);
	}
	player->Update();
	DamageTexUpdate();
}

void PlayerControl::DamageTexUpdate()
{
	constexpr float l_AlphaFeedVal = 0.02f;
	if (player->GetHP() < 0)
	{
		vignette = 0.5f;
	}
	if (HUD::GetIns()->GetRecvDamageFlag())
	{
		vignette = 0.9f;
		dalpha = 1.0f;
	}
	dalpha -= l_AlphaFeedVal;

	vignette -= 0.02f;
	vignette = max(vignette, 0.0f);

	DamageTex->setcolor({1.f, 1.f, 1.f, dalpha});

	dalpha = std::clamp(dalpha, 0.0f, 1.f);
}

void PlayerControl::DamageTexDraw()
{
	if (HUD::GetIns()->GetPlayerHP()->GetSize().x > 0.f && GetIns()->GetPlayer()->GetHP() > 0)
	{
		player->DamageTexDraw();
		Sprite::PreDraw();
		DamageTex->Draw();
		Sprite::PostDraw();
	}
}

/*------------------------*/
/*--------�`�揈��---------*/
/*------------------------*/
/*��������������ł��܂��Ă���...*/
void PlayerControl::Draw_Play()
{
	if (player == nullptr || TurnoffDrawF)
	{
		return;
	}
	player->Draw();
}

void PlayerControl::Draw_Tutorial()
{
	if (player == nullptr)
	{
		return;
	}
	player->Draw();
	ImGui::Begin("pos,,");

	ImGui::SetWindowPos(ImVec2(0, 500));
	ImGui::SetWindowSize(ImVec2(300, 300));
	ImGui::Text("%f", player->GetPosition().x);
	ImGui::Text("%f", player->GetPosition().z);
	ImGui::End();
}

void PlayerControl::Draw_Boss()
{
	if (player == nullptr)
	{
		return;
	}
	player->Draw();
}

void PlayerControl::BossFieldCol()
{
	if (Collision::GetLength(player->GetPosition(), {0, -10, 0}) > 120)
	{
		player->isOldPos();
	}
}