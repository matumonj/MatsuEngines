#include "PlayerControl.h"
#include"SceneManager.h"
#include"TutorialSprite.h"
#include"PlayerAttackState.h"
#include"AttackCollision.h"
#include"Feed.h"
PlayerControl* PlayerControl::GetInstance()
{
	static PlayerControl instance;
	return &instance;
}
void PlayerControl::Init_Tutorial(DebugCamera* camera)
{
	Sprite::LoadTexture(190, L"Resources/2d/damage/playerdamage.png");

	DamageTex = Sprite::Create(190, { 0,0 });
	DamageTex->SetSize({ 1900,1000 });
	player = std::make_unique<Player>();
	player->Initialize(camera);

	StartPos = { 92.0f,-23.0f,-760.0f };

	player->SetPosition(StartPos);
	AttackCollision::GetInstance()->Init();
}

void PlayerControl::Init_Play(DebugCamera* camera)
{
	StartPos = { 110.0f,-12.0f,-379.0f };

	player->SetPosition(StartPos);
	AttackCollision::GetInstance()->Init();
}
void PlayerControl::Init_Boss(DebugCamera* camera)
{
	StartPos = { 0.0f,-20.0f,0.0f };

	player->SetPosition(StartPos);
	AttackCollision::GetInstance()->Init();
}
/*------------------------*/
/*--------�������---------*/
/*------------------------*/
void PlayerControl::Finalize()
{
	AttackCollision::GetInstance()->Finalize();
	//Destroy_unique(player);
}

/*------------------------*/
/*--------�Ǎ�����---------*/
/*----------scv----------*/
void PlayerControl::Load(DebugCamera* camera)
{
	switch (SceneManager::GetInstance()->GetScene())
	{
	case SceneManager::TUTORIAL:
		StartPos = { 92.0f,-20.0f,-760.0f };
		break;
	case SceneManager::PLAY:
		StartPos = { 110.0f,-12.0f,-379.0f };
		break;
	case SceneManager::BOSS:
		StartPos = { 0.0f,2.0f,-50.0f };
		break;
	default:
		break;
	}

	player->SetPosition(StartPos);
	AttackCollision::GetInstance()->Init();
}

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
#include"HUD.h"
#include"CameraControl.h"
//player�̒��ɂ���ړ������Ƃ��͌�ł����������Ă���
void PlayerControl::Update_Tutorial(DebugCamera* camera)//�`���[�g���A����
{
	if (player == nullptr)return;
	player->Update(camera);
	
	
	dalpha -= 0.02f;
	DamageTex->setcolor({ 1,1,1,dalpha });
	PlayerAttackState::GetInstance()->Update();
	dalpha = max(dalpha, 0.0f);
}

void PlayerControl::Update_Play(DebugCamera* camera)//�v���C�V�[����
{
	player->Update(camera);
	if (HUD::GetInstance()->GetRecvDamageFlag()) {
		dalpha = 1.0f;

	}

	dalpha -= 0.02f;
	DamageTex->setcolor({ 1,1,1,dalpha });
	PlayerAttackState::GetInstance()->Update();
	dalpha = max(dalpha, 0.0f);
}
void PlayerControl::Update_Boss(DebugCamera* camera)
{
	player->Update(camera);
	if (HUD::GetInstance()->GetRecvDamageFlag()) {
		dalpha = 1.0f;

	}

	dalpha -= 0.02f;
	DamageTex->setcolor({ 1,1,1,dalpha });
	PlayerAttackState::GetInstance()->Update();
	dalpha = max(dalpha, 0.0f);
}
/*------------------------*/
/*--------�`�揈��---------*/
/*------------------------*/
void PlayerControl::Draw_Play()
{
	if (player == nullptr)return;
		player->Draw();	
}

void PlayerControl::Draw_Tutorial()
{
	if (player == nullptr)return;
	player->Draw();
}

void PlayerControl::Draw_Boss()
{
	if (player == nullptr)return;
	player->Draw();
}

void PlayerControl::DamageTexDraw()
{
	Sprite::PreDraw();
	DamageTex->Draw();
	Sprite::PostDraw();
}