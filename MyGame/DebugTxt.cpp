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
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Texture::Create(texnumber, { 0,0,0 }, { 4,4,4 }, { 0,0,1,1 });
		spriteDatas[i]->CreateTexture();
		spriteDatas[i]->SetAnchorPoint({ 0.5f,0.5f });

	}
}

// 1文字列追加
void DebugTxt::Print(const std::string& text, float x, float y, float z,float scale = 1.0f)
{
	// 全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		spriteDatas[spriteIndex]->SetPosition({ x + fontWidth * 1* i, y,z });
		spriteDatas[spriteIndex]->SetColor(color);
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetScale({ fontWidth * scale/10, fontHeight * scale/10,1 });
		spriteDatas[spriteIndex]->SetRotation({ 180,0,0 });
		spriteDatas[spriteIndex]->Update(CameraControl::GetInstance()->GetCamera());
		// 文字を１つ進める
		spriteIndex++;
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