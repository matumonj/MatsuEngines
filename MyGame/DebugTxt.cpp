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
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
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

// 1文字列追加
void DebugTxt::Print(float scale = 1.0f)
{
	TextBeha();

	// 全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
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
		// 文字を１つ進める
		spriteIndex++;
	}

	if (color.w <= 0.0f)
	{
		isDestoy = true;
	}
}

// まとめて描画
void DebugTxt::DrawAll()
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		// スプライト描画
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
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Sprite::Create(texnumber, {0, 0});
		spriteDatas[i]->SetAnchorPoint({0.5f, 0.5f});
	}
}

// 1文字列追加
void DebugTextSprite::Print(const std::string& text, float x, float y, float scale = 1.0f)
{
	// 全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		spriteDatas[spriteIndex]->SetPosition({x + fontWidth * scale * i, y});
		spriteDatas[spriteIndex]->SetTextureRect(
			{static_cast<float>(fontIndexX) * fontWidth, static_cast<float>(fontIndexY) * fontHeight}, {
				static_cast<float>(fontWidth), static_cast<float>(fontHeight)
			});
		spriteDatas[spriteIndex]->SetSize({fontWidth * scale, fontHeight * scale});

		// 文字を１つ進める
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

// まとめて描画
void DebugTextSprite::DrawAll()
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		spriteDatas[i]->setcolor({color.x, color.y, color.z, alpha});
		// スプライト描画
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
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Sprite::Create(texnumber, {0, 0});
		spriteDatas[i]->SetAnchorPoint({0.5f, 0.5f});
	}
}

// 1文字列追加
void DebugTextSprite2::Print(const std::string& text, float x, float y, float scale = 1.0f)
{
	// 全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		spriteDatas[spriteIndex]->SetPosition({x + fontWidth * scale * i, y});
		spriteDatas[spriteIndex]->SetTextureRect(
			{static_cast<float>(fontIndexX) * fontWidth, static_cast<float>(fontIndexY) * fontHeight}, {
				static_cast<float>(fontWidth), static_cast<float>(fontHeight)
			});
		spriteDatas[spriteIndex]->SetSize({fontWidth * scale, fontHeight * scale});

		// 文字を１つ進める
		spriteIndex++;
	}
}

// まとめて描画
void DebugTextSprite2::DrawAll()
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		// スプライト描画
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}
