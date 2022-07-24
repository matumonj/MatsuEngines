#pragma once
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>
#include"Model.h"
#include"Object3d.h"
#include"DebugCamera.h"
class CreateObjManager
{
protected:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	virtual void Initialize(DebugCamera*camera)=0;

	virtual void ArgMent(DebugCamera* camera)=0;

	virtual void Update(DebugCamera* camera)=0;

	virtual void FileWriting()=0;

	virtual void ImGui_Draw()=0;

	virtual void Draw()=0;

protected:
	std::ifstream file;
	std::stringstream popcom;

	bool ArgmentFlag;
	bool DeleteFlag;
	//�z�u�I�u�W�F�W�F�N�g���f��
	Model* Model;
	Object3d* Obj;

	//
	std::vector<XMFLOAT3>Position;
	//
	XMFLOAT3 pos;

};

