#include "DebugTxt.h"
#include"CameraControl.h"

DebugTxt::DebugTxt()
{
}

DebugTxt::~DebugTxt()
{
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		delete spriteDatas[i];
	}
}

DebugTxt* DebugTxt::GetIns()
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
		spriteDatas[i] = Texture::Create(texnumber, {0, 0, 0}, {4, 4, 4}, {0, 0, 1, 1});
		spriteDatas[i]->CreateTexture();
		spriteDatas[i]->SetAnchorPoint({0.5f, 0.5f});
	}
	color = {1.0f, 1.0f, 1.0f, 1.0f};
	isDestoy = false;
}

void DebugTxt::TextBeha()
{
	//Position.y -= 0.01f;
	color.w -= 0.005f;
}

// 1������ǉ�
void DebugTxt::Print(float scale = 1.0f)
{
	TextBeha();

	// �S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		// �ő啶��������
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		spriteDatas[spriteIndex]->SetPosition({Position.x + fontWidth * 1 * i / 15, Position.y, Position.z});
		spriteDatas[spriteIndex]->SetColor(color);
		spriteDatas[spriteIndex]->SetTextureRect(
			{static_cast<float>(fontIndexX) * fontWidth, static_cast<float>(fontIndexY) * fontHeight}, {
				static_cast<float>(fontWidth), static_cast<float>(fontHeight)
			});
		spriteDatas[spriteIndex]->SetScale({fontWidth * scale / 100, fontHeight * scale / 100, 1});
		spriteDatas[spriteIndex]->SetRotation({180, 0, 0});
		spriteDatas[spriteIndex]->SetBillboard(true);
		spriteDatas[spriteIndex]->SetUVMove(false);
		spriteDatas[spriteIndex]->SetDisplayRadius(100);
		spriteDatas[spriteIndex]->Update(CameraControl::GetIns()->GetCamera());
		// �������P�i�߂�
		spriteIndex++;
	}

	if (color.w <= 0.0f)
	{
		isDestoy = true;
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


DebugTextSprite::DebugTextSprite()
{
}

DebugTextSprite::~DebugTextSprite()
{
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		delete spriteDatas[i];
	}
}

DebugTextSprite* DebugTextSprite::GetIns()
{
	static DebugTextSprite instance;
	return &instance;
}

void DebugTextSprite::Initialize(UINT texnumber)
{
	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// �X�v���C�g�𐶐�����
		spriteDatas[i] = Sprite::Create(texnumber, {0, 0});
		spriteDatas[i]->SetAnchorPoint({0.5f, 0.5f});
	}
}

// 1������ǉ�
void DebugTextSprite::Print(const std::string& text, float x, float y, float scale = 1.0f)
{
	// �S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		// �ő啶��������
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		spriteDatas[spriteIndex]->SetPosition({x + fontWidth * scale * i, y});
		spriteDatas[spriteIndex]->SetTextureRect(
			{static_cast<float>(fontIndexX) * fontWidth, static_cast<float>(fontIndexY) * fontHeight}, {
				static_cast<float>(fontWidth), static_cast<float>(fontHeight)
			});
		spriteDatas[spriteIndex]->SetSize({fontWidth * scale, fontHeight * scale});

		// �������P�i�߂�
		spriteIndex++;
	}
}

void DebugTextSprite::FeedTex()
{
	alpha -= 0.0005f;
	if (SizeVariableF)
	{
		alpha = 1.0f;
		TexScale.x += 0.05f;
		TexScale.y += 0.05f;

		if (TexScale.x >= 2.0f)
		{
			VariableStopT++;
			if (VariableStopT > 60)
			{
				SizeVariableF = false;
			}
		}
	}
	else
	{
		alpha -= 0.02f;
		VariableStopT = 0;
		if (alpha <= 0.0f)
		{
			SizeVariableF = false;
		}
	}

	//alpha = max(alpha, 0);
	TexScale.x = min(TexScale.x, 2.0f);
	TexScale.y = min(TexScale.y, 2.0f);
	TexScale.x = max(TexScale.x, 0.0f);
	TexScale.y = max(TexScale.y, 0.0f);
}

// �܂Ƃ߂ĕ`��
void DebugTextSprite::DrawAll()
{
	// �S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		spriteDatas[i]->setcolor({color.x, color.y, color.z, alpha});
		// �X�v���C�g�`��
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}


DebugTextSprite2::DebugTextSprite2()
{
}

DebugTextSprite2::~DebugTextSprite2()
{
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		delete spriteDatas[i];
	}
}

DebugTextSprite2* DebugTextSprite2::GetIns()
{
	static DebugTextSprite2 instance;
	return &instance;
}

void DebugTextSprite2::Initialize(UINT texnumber)
{
	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// �X�v���C�g�𐶐�����
		spriteDatas[i] = Sprite::Create(texnumber, {0, 0});
		spriteDatas[i]->SetAnchorPoint({0.5f, 0.5f});
	}
}

// 1������ǉ�
void DebugTextSprite2::Print(const std::string& text, float x, float y, float scale = 1.0f)
{
	// �S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		// �ő啶��������
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		spriteDatas[spriteIndex]->SetPosition({x + fontWidth * scale * i, y});
		spriteDatas[spriteIndex]->SetTextureRect(
			{static_cast<float>(fontIndexX) * fontWidth, static_cast<float>(fontIndexY) * fontHeight}, {
				static_cast<float>(fontWidth), static_cast<float>(fontHeight)
			});
		spriteDatas[spriteIndex]->SetSize({fontWidth * scale, fontHeight * scale});

		// �������P�i�߂�
		spriteIndex++;
	}
}

// �܂Ƃ߂ĕ`��
void DebugTextSprite2::DrawAll()
{
	// �S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		// �X�v���C�g�`��
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}
