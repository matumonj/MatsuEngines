#pragma once

#include "f_Model.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include"LightGroup.h"
//LightGroup* Object3d::lightGroup = nullptr;

class BaseCollider;

class f_Object3d
{
protected: //�G�C���A�X
	//Microosoft::WRL::���ȗ�
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public: //�ÓI�����o�֐�
	//setter
	static void SetDevice(ID3D12Device* device) { f_Object3d::device = device; }
	static void SetCamera(Camera* camera) { f_Object3d::camera = camera; }


	/// <summary>
	///�O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ComPtr<ID3D12Device> device;
	static ComPtr<ID3D12GraphicsCommandList> cmdList;
	//�J����
	static Camera* camera;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

public: //�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\���́i���W�ϊ��s��j
	struct ConstBufferDataTransform
	{
		XMFLOAT4 color;
		XMMATRIX viewproj; //�r���[�v���W�F�N�V�����s��
		XMMATRIX world; //���[���h�s��
		XMFLOAT3 cameraPos; //�J�����s��i���[���h���W�j
	};

public: //�����o�֐�
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[��
	/// </summary>
	void Updata(bool animeloop);
	void Updata();
	/// </summary>
	void Update(bool Loop, double Speed, bool& Stop);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw();

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(f_Model* model) { this->model = model; }

	/// <summary>
	/// �A�j���[�V����
	/// </summary>
	void PlayAnimation(int number);
	void PlayAnimation();
protected: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
	//���[�J���X�P�[��
	XMFLOAT3 scale = {0.1f, 0.1f, 0.1f};
	//X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = {0.0f, 0.0f, 0.0f};
	//���[�J�����W
	XMFLOAT3 position = {0.0f, 0.0f, 0.0f};
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//���f��
	f_Model* model = nullptr;
	//�萔�o�b�t�@�i�X�L���j
	ComPtr<ID3D12Resource> constBuffSkin;
	//1�t���[��
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ��ԁi�A�j���[�V�����j
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
	XMMATRIX matScale, matRot, matTrans;
	XMFLOAT4 color;
	float f_time;
	int fbxTime = 0;
	float start_time;
	double end_time;

	bool AttackFlag;
	float NormalAttackTime;
	bool DeathFlag;
	float DeathTime;

public: //�萔
	void SetColor(XMFLOAT4 color) { this->color = color; }
	void SetPosition(XMFLOAT3 pos) { position = pos; }
	void SetRotation(XMFLOAT3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 scl) { scale = scl; }

	XMMATRIX GetMatRot() { return matRot; }

	XMMATRIX GetMatTrans() { return matTrans; }

	XMMATRIX GetMatScale() { return matScale; }
	//�{�[���̍ő吔
	static const int MAX_BONES = 88;
	XMFLOAT3 GetPosition() { return position; }

	//�萔�o�b�t�@�p�f�[�^�\���́i�X�L�j���O�j
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	bool stopFlag;
	void SeteCurrent(bool f) { stopFlag = f; }
	XMMATRIX HandMatWorld;
	XMMATRIX Posmat;
	XMFLOAT3 pos;
	XMMATRIX GetPos() { return Posmat; }
	XMMATRIX GetMatrot() { return matRot; }
	XMMATRIX GetHandBoneMatWorld() { return HandMatWorld; }
	FbxTime SetCurrent() { return currentTime = startTime; }
	// ���C�g
	//static LightGroup* lightGroup;
	int bindexs = 13;
	void SetFogPos(XMFLOAT3 pos) { fogpos = pos; }
	void SetHandBoneIndex(int bindex) { bindexs = bindex; }
	double GetFTime() { return currentTime.GetSecondDouble(); }
private:
	XMFLOAT3 fogpos;
	bool nowAttack;
	bool nowDeath;

	// �A�j���[�V�����p�f�[�^�\����
	struct Animation
	{
		FbxAnimStack* stack;
		FbxTakeInfo* info;
	};
	//�A�j���[�V�����̃f�[�^
	std::vector<Animation> animationData;
	bool isendtime;

	double AnimationSpeed;
public:
	
	void LoadAnimation();
	void SetFbxTime(float time) { f_time = time; }
	void SetFTime(int time) { fbxTime = time; }
	void SetAttackFlag(bool flag) { AttackFlag = flag; }
	void SetAttackTime(float time) { NormalAttackTime = time; }
	void SetDeathFlag(bool flag) { DeathFlag = flag; }
	void SetDeathTime(float time) { DeathTime = time; }
	double GetEndTime() { return  endTime.GetSecondDouble(); }
	double GetAnimeTime() { return currentTime.GetSecondDouble(); }
	XMMATRIX GetWorld() { return matWorld; }

	XMMATRIX ExtractPositionMat(XMMATRIX matworld);
	XMMATRIX ExtractRotationMat(XMMATRIX matworld);
	XMMATRIX ExtractScaleMat(XMMATRIX matworld);
};
