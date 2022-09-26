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
	static const int maxCharCount = 256;// 最大文字数
	static const int fontWidth = 9;// フォント画像内1文字分の横幅
	static const int fontHeight = 18;// フォント画像内1文字分の縦幅
	static const int fontLineCount = 14;// フォント画像内1行分の文字数

	DebugTxt();
	~DebugTxt();
public:// 静的メンバ関数
	static DebugTxt* GetInstance();

	void Initialize(UINT texnumber);

	void Print(const std::string& text, float x, float y,float z, float size);

	void DrawAll();
public:
	void SetColor(XMFLOAT4 color) { this->color = color; }
private:
	// スプライトデータの配列
	Texture* spriteDatas[maxCharCount] = {};
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;
	//
	XMFLOAT4 color;
};
