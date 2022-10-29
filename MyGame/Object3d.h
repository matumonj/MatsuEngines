#pragma once

#include<string>
#include"Model.h"
#include"LightGroup.h"
#include"CollisionInfo.h"
#include"DebugCamera.h"
//#include"CollisionManager.h"
class BaseCollider;

class Object3d
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	Model* model = nullptr;

	 Camera* camera;
public: // �T�u�N���X
	void SetModel(Model* model) { this->model = model; }

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataB0
	{
		XMFLOAT4 color;
		//XMMATRIX mat;
		// �萔�o�b�t�@�p�f�[�^�\����B0
		XMMATRIX viewproj;    // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
		//bool shadowf;
		bool gsflag;
		XMFLOAT3 ks2;
		bool f;
		XMFLOAT3 ks3;
		float time;
		XMFLOAT3 ks4;
		bool destF;
		XMFLOAT3 ks5;
		float destTime;
		bool shadowf;
	};

private: // �萔
	XMFLOAT3 FogCenter;
	static const int division = 50;
	bool shadowf = true;
public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	/// <returns>����</returns>
	static bool StaticInitialize(int window_width, int window_height, Camera* camera = nullptr);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static Object3d* Create(DebugCamera* camera);

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// ���_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// �����_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	static void CameraMoveVector(XMFLOAT3 move);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ComPtr<ID3D12Device> device;

	// �R�}���h���X�g
	static ComPtr<ID3D12GraphicsCommandList> cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;



	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;

	// ���_�f�[�^�z��

	//static unsigned short indices[planeCount * 3];

private:// �ÓI�����o�֐�

	/// <summary>
	/// �J����������
	/// </summary>
	/// <param name="window_width">��ʉ���</param>
	/// <param name="window_height">��ʏc��</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static bool InitializeGraphicsPipeline();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <returns>����</returns>
	//	void CreateGraphicsPipeline(const wchar_t* vsShaderName, const wchar_t* psShaderName, const wchar_t* gsame, );
	/// <summary>
	/// ���f���쐬
	/// </summary>


	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	static void UpdateViewMatrix();

	static void UpdateViewMatrix(XMMATRIX mat);

	static LightGroup* lightGroup;
public: // �����o�֐�
	Object3d() = default;

	virtual ~Object3d();

	virtual bool Initialize( DebugCamera* camera);
	//void CreateModel();
	/// <summary>
	/// ���t���[������
	/// </summary>
	virtual void Update(XMFLOAT4 color, DebugCamera* camera);

	virtual void Update(XMMATRIX matWorld, XMFLOAT4 color, DebugCamera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	void SetFogCenter(XMFLOAT3 pos) { FogCenter = pos; }
	const XMFLOAT3& GetPosition() { return position; }
	const XMFLOAT3& GetRotation() { return rotation; }
	const XMFLOAT3& GetScale() { return scale; }
	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	void SetCamera(Camera* cameras) {
		camera = cameras;
	}

	static void SetLightGroup(LightGroup* lightGroup) {
		Object3d::lightGroup = lightGroup;
	}
protected: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@

	// �F
	XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };

	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	// �e�I�u�W�F�N�g
	Object3d* parent = nullptr;
	bool rf;
	bool gsf = false;;
	XMMATRIX rm,rt,rr,rs;
	bool setef=false;
	bool uvf=false;
	float uvtime;

	bool DestF = false;
	float DestTime = 0;
public:
	void SetDestFlag(bool f) { DestF = f; }
	void SetDestTime(float t) { DestTime = t; }

	void setFog(bool f) { setef = f; }
	void SetUVf(bool f) { uvf = f; }
	void setr(bool f) { rf = f; }
	XMMATRIX GetMatrot() {return matRot; }
	XMMATRIX GetMatScl() { return matScale; }
	void SetRotS(XMMATRIX matrot) { matRot *= matrot; }
	void Setf(bool flag);
	XMMATRIX GetMatTrans() { return matTrans; }
	void SetMatrot(XMMATRIX rot) { rm = rot; }
	void SetMatRot(XMMATRIX rot) {matRot = rot; }
	void SetMatTrans(XMMATRIX rot) { rt = rot; }
	void SetMatScale(XMMATRIX rot) { rs = rot; }
	void UpdateWorldMatrix();
	void UpdateWorldMatrix(XMMATRIX mat);
	void SetCollider(BaseCollider* collider);
	virtual void OnCollision(const CollisionInfo& info) {}
	void SetParent(XMMATRIX matworld) { this->matWorld *= matworld; }
	void SetColor(XMFLOAT4 color) { this->color = color; }
	void SetShadowF(bool f) { shadowf = f; }
	XMMATRIX ExtractPositionMat();
	XMMATRIX ExtractRotationMat();
	XMMATRIX ExtractScaleMat();

	XMMATRIX ExtractPositionMat(XMMATRIX matworld);
protected:
	const char* name = nullptr;
	public:

	BaseCollider* collider = nullptr;
	private:
		XMMATRIX matScale, matRot, matTrans;
		XMFLOAT2 time;
public:
	/// <summary>
	/// ���f�����擾
	/// </summary>
	/// <param name="material">�}�e���A��</param>
	inline Model* GetModel() { return model; }
};



