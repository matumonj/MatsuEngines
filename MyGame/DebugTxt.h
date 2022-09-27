#pragma once

#include "Sprite.h"
#include <Windows.h>
#include <string>
#include"Texture.h"
class DebugTxt
{
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	// デバッグテキスト用のテクスチャ番号を指定
	static const int maxCharCount = 40;// 最大文字数
	static const int fontWidth = 9*6;// フォント画像内1文字分の横幅
	static const int fontHeight = 18*6;// フォント画像内1文字分の縦幅
	static const int fontLineCount = 14;// フォント画像内1行分の文字数

	DebugTxt();
	~DebugTxt();
public:// 静的メンバ関数
	static DebugTxt* GetInstance();

	void Initialize(UINT texnumber);

	void Print(float size);

	void DrawAll();

private:
	bool isDestoy=false;
	void TextBeha();
	enum Fase {
		FASEONE,
		FASETWO
	};
	Fase fase;
public:
	void SetPosition(XMFLOAT3 position) { Position = position; }
	XMFLOAT3 GetPosition() { return Position; }

	void SetColor(XMFLOAT4 color) { this->color = color; }
	bool GetisDestroy() { return isDestoy; }

	void SetText(const std::string& text) { this->text = text; }
private:
	// スプライトデータの配列
	Texture* spriteDatas[maxCharCount] = {};
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;
	//
	XMFLOAT4 color;
	//
	XMFLOAT3 Position;

	std::string text;
};
