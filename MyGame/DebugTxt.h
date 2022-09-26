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
	static const int maxCharCount = 256;// �ő啶����
	static const int fontWidth = 9;// �t�H���g�摜��1�������̉���
	static const int fontHeight = 18;// �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14;// �t�H���g�摜��1�s���̕�����

	DebugTxt();
	~DebugTxt();
public:// �ÓI�����o�֐�
	static DebugTxt* GetInstance();

	void Initialize(UINT texnumber);

	void Print(const std::string& text, float x, float y,float z, float size);

	void DrawAll();
public:
	void SetColor(XMFLOAT4 color) { this->color = color; }
private:
	// �X�v���C�g�f�[�^�̔z��
	Texture* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;
	//
	XMFLOAT4 color;
};
