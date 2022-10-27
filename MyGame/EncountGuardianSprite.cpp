#include "EncountGuardianSprite.h"
#include"EnemyControl.h"
#include"CameraControl.h"

EncountGuardianSprite* EncountGuardianSprite::GetInstance()
{
	static EncountGuardianSprite ins;
	return &ins;
}

void EncountGuardianSprite::Init()
{
	Sprite::LoadTexture(350, L"Resources/GuardianAppearrance/GuardianAppearrance_Up.png");
	Sprite::LoadTexture(360, L"Resources/GuardianAppearrance/GuardianAppearrance_Buttom.png");
	Sprite::LoadTexture(370, L"Resources/GuardianAppearrance/EnemyForm.png");

	//��̃t���[���摜
	Sprite*up = Sprite::Create(350, { 0.0f,-200.0f });
	AppearranceSprite[Site::UP].reset(up);
	//���̃t���[���摜
	Sprite* bottom = Sprite::Create(360, { 0.0f,-200.0f });
	AppearranceSprite[Site::BOTTOM].reset(bottom);
	//�K�[�f�B�A���̎p�G
	Sprite*form = Sprite::Create(370, { 0.0f,-200.0f });
	AppearranceSprite[Site::FORM].reset(form);
	//���W�ʒu�ݒ�
	AppearranceSprite[Site::UP]->SetPosition({0.0f,100.0f });
	AppearranceSprite[Site::BOTTOM]->SetPosition({ 0.0f,700.0f });
	AppearranceSprite[Site::FORM]->SetPosition({ 0.0f,250.0f });
	//�T�C�Y�ݒ�
	AppearranceSprite[Site::UP]->SetSize({ WinApp::window_width,150 });
	AppearranceSprite[Site::BOTTOM]->SetSize({ WinApp::window_width,150 });
	AppearranceSprite[Site::FORM]->SetSize({ WinApp::window_width,350});
	for (int i = 0; i < 3; i++) {
		AppearranceSprite[i]->SetAnchorPoint({0.0,0.0});
	}

	SpriteAlpha = 0.0f;
}

void EncountGuardianSprite::Update()
{
	//�K�[�f�B�A���Ƃ̃G���J�E���g��������������A���t�@�l�P��
	if (CameraControl::GetInstance()->GetEncountFlag()) {
		SpriteAlpha = 1.0f;
	}
	else {
		SpriteAlpha -= 0.02f;
	}
	for (int i = 0; i < 3; i++) {
		AppearranceSprite[i]->setcolor({ 1.0f,1.0f,1.0f,SpriteAlpha });
	}
	//UV�X�N���[��
	AppearranceSprite[Site::UP]->SetUvscroll(TRUE);
	AppearranceSprite[Site::UP]->SetUv_time(+0.01f);
	AppearranceSprite[Site::BOTTOM]->SetUvscroll(TRUE);
	AppearranceSprite[Site::BOTTOM]->SetUv_time(-0.01f);

	SpriteAlpha = min(SpriteAlpha, 1.0f);
	SpriteAlpha = max(SpriteAlpha, 0.0f);
}


void EncountGuardianSprite::Draw()
{
	Sprite::PreDraw();
	for (int i = 0; i < 3; i++) {
		AppearranceSprite[i]->Draw();
	}
	Sprite::PostDraw();
}

#include"mHelper.h"
void EncountGuardianSprite::Finalize()
{
	for (int i = 0; i < 3; i++) {
		Destroy_unique(AppearranceSprite[i]);
	}
}