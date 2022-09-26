#include "DebugTxt.h"
#include"CameraControl.h"
DebugTxt::DebugTxt()
{
}

DebugTxt::~DebugTxt()
{
	for (int i = 0; i < _countof(spriteDatas); i++) {
		delete spriteDatas[i];
	}
}

DebugTxt* DebugTxt::GetInstance()
{
	static DebugTxt instance;
	return &instance;
}

void DebugTxt::Initialize(UINT texnumber)
{
	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// �X�v���C�g�𐶐�����
		spriteDatas[i] = Texture::Create(texnumber, { 0,0,0 }, { 4,4,4 }, { 0,0,1,1 });
		spriteDatas[i]->CreateTexture();
		spriteDatas[i]->SetAnchorPoint({ 0.5f,0.5f });

	}
}

// 1������ǉ�
void DebugTxt::Print(const std::string& text, float x, float y, float z,float scale = 1.0f)
{
	// �S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		// �ő啶��������
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		spriteDatas[spriteIndex]->SetPosition({ x + fontWidth * 1* i, y,z });
		spriteDatas[spriteIndex]->SetColor(color);
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetScale({ fontWidth * scale/10, fontHeight * scale/10,1 });
		spriteDatas[spriteIndex]->SetRotation({ 180,0,0 });
		spriteDatas[spriteIndex]->Update(CameraControl::GetInstance()->GetCamera());
		// �������P�i�߂�
		spriteIndex++;
	}
}

// �܂Ƃ߂ĕ`��
void DebugTxt::DrawAll()
{
	// �S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		// �X�v���C�g�`��
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}