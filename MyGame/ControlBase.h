#pragma once
#include"Enemy.h"
#include<fstream>
#include<string>
#include<sstream>
#include"Collision.h"

class ControlBase
{
public:
	virtual ~ControlBase();
public:
	virtual void Load() =0;

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();


	virtual void Update_Tutorial() = 0;
	virtual void Update_Play() = 0;
	virtual void Update_Boss() = 0;

	virtual void Init_Tutorial() = 0;
	virtual void Init_Play() = 0;
	virtual void Init_Boss() = 0;

	virtual void Draw_Tutorial() = 0;
	virtual void Draw_Play() = 0;
	virtual void Draw_Boss() = 0;


	virtual void Finalize() = 0;
private:
	static void (ControlBase::* initTable[])();
	static void (ControlBase::* updateTable[])();
	static void (ControlBase::* drawTable[])();
protected:
	template <class T>
	using ComPtr = ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = XMFLOAT2;
	using XMFLOAT3 = XMFLOAT3;
	using XMFLOAT4 = XMFLOAT4;
	using XMMATRIX = XMMATRIX;
	using XMVECTOR = XMVECTOR;

protected:
	//�G�f�B�^�̓ǂݍ��݂ɕK�v�ȕϐ�
	int Quantity;
	std::vector<XMFLOAT3> pos;
	std::vector<XMFLOAT3> rot;
	std::vector<XMFLOAT3> scl;
	std::string line;
	std::stringstream popcom;
	std::ifstream file;
	std::vector<int> Num;
	//�I�u�W�F�N�g�̍X�V�ƕ`�悪�����͈�
	float UpdateRange;
public:
	int GetQuentity() { return Quantity; }
};
