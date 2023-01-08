#pragma once

#include "Sprite.h"
#include <Windows.h>
#include <string>
#include"Texture.h"

class DebugTxt
{
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	// �f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int maxCharCount = 20; // �ő啶����
	static const int fontWidth = 9 * 6; // �t�H���g�摜��1�������̉���
	static const int fontHeight = 18 * 6; // �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14; // �t�H���g�摜��1�s���̕�����

	DebugTxt();
	~DebugTxt();
public: // �ÓI�����o�֐�
	static DebugTxt* GetInstance();

	void Initialize(UINT texnumber);

	void Print(float size);

	void DrawAll();


private:
	bool isDestoy = false;
	void TextBeha();

	enum Phase
	{
		PHASEONE,
		PHASETWO
	};

	Phase phase;
public:
	void SetPosition(XMFLOAT3 position) { Position = position; }
	XMFLOAT3 GetPosition() { return Position; }

	void SetColor(XMFLOAT4 color) { this->color = color; }
	bool GetisDestroy() { return isDestoy; }

	void SetText(const std::string& text) { this->text = text; }
private:
	// �X�v���C�g�f�[�^�̔z��
	Texture* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;
	//
	XMFLOAT4 color;
	//
	XMFLOAT3 Position;

	std::string text;
};

#pragma once

#include "Sprite.h"
#include <Windows.h>
#include <string>

class DebugTextSprite
{
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	// �f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int maxCharCount = 20; // �ő啶����
	static const int fontWidth = 54; // �t�H���g�摜��1�������̉���
	static const int fontHeight = 108; // �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14; // �t�H���g�摜��1�s���̕�����

	DebugTextSprite();
	~DebugTextSprite();
public: // �ÓI�����o�֐�
	static DebugTextSprite* GetInstance();

	void Initialize(UINT texnumber);

	void Print(const std::string& text, float x, float y, float size);

	void DrawAll();

	void FeedTex();

	void SetAlpha(float alpha) { this->alpha = alpha; }
	void SetColor(XMFLOAT3 color) { this->color = color; }
private:
	XMFLOAT3 color;
	bool SizeVariableF = false;
	int VariableStopT = 0;
	XMFLOAT2 TexScale = {0.f, 0.f};
	float alpha = 1.f;
	// �X�v���C�g�f�[�^�̔z��
	Sprite* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;
};

class DebugTextSprite2
{
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	// �f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int maxCharCount = 200; // �ő啶����
	static const int fontWidth = 55; // �t�H���g�摜��1�������̉���
	static const int fontHeight = 110; // �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14; // �t�H���g�摜��1�s���̕�����

	DebugTextSprite2();
	~DebugTextSprite2();
public: // �ÓI�����o�֐�
	static DebugTextSprite2* GetInstance();

	void Initialize(UINT texnumber);

	void Print(const std::string& text, float x, float y, float size);

	void DrawAll();

	void FeedTex();

	void SetAlpha(float alpha) { this->alpha = alpha; }

	void SetColor(XMFLOAT3 color) { this->color = color; }
private:
	bool SizeVariableF = false;
	int VariableStopT = 0;
	XMFLOAT2 TexScale = {0.f, 0.f};
	float alpha = 1.f;
	XMFLOAT3 color;
	// �X�v���C�g�f�[�^�̔z��
	Sprite* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;
};
