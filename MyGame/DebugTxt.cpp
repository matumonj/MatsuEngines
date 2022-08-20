#include "DebugTxt.h"

DebugTxt::DebugTxt()
{
}

DebugTxt::~DebugTxt()
{
	for (int i = 0; i < _countof(spriteDatas); i++) {
		delete spriteDatas[i];
	}
}

void DebugTxt::Initialize(UINT texnumber)
{
	Sprite::LoadTexture(60, L"Resources/target.png");

	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// �X�v���C�g�𐶐�����
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0});
		//spriteDatas[i]->CreateTexture();
		//spriteDatas[i]->SetScale({ 1000,1000,1 });
	}
}

// 1������ǉ�
void DebugTxt::Print(const std::string& text, float x, float y, float scale = 2000.0f)
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
		spriteDatas[spriteIndex]->SetPosition({ x + fontWidth * scale * i, y});
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });
		spriteDatas[spriteIndex]->setcolor({ 1,1,1,1 });
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