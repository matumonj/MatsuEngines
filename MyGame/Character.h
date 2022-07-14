#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include"DirectXCommon.h"
#include"Object3d.h"
#include"Model.h"
#include"DebugCamera.h"
//���ׂẴI�u�W�F�N�g�̊��N���X
class Character
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	//�������̃t���O
	bool AliveFlag = TRUE;
	//�̗�
	float HP;
public:
	//�U������
	virtual void Attack();
	//���b����(�j�������ȂǓ����)
	virtual void Death();
};

