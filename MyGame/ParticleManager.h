#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<forward_list>

/// <summary>
/// 3D�I�u�W�F�N�g
/// </summary>
class ParticleManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz���W
		float scale;
		XMFLOAT3 rot;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color; // �F (RGBA)
		XMMATRIX mat; // �R�c�ϊ��s��]
		XMMATRIX matBillboard; //�r���{�[�h�s��
	};

	XMFLOAT4 color = {1, 1, 1, 1};

	struct Particle
	{
		using XMFLOAT3 = DirectX::XMFLOAT3;
		//���W
		XMFLOAT3 position = {};
		//���x
		XMFLOAT3 velocity = {};
		XMFLOAT3 velocity_old = {};
		//�����x
		XMFLOAT3 accel = {};
		//���݃t���[��
		int frame = 0;
		//�I���t���[��
		int num_frame = 10;
		float scale = 1.0f;
		float s_scale = 1.0f;
		float e_scale = 0.0f;
		XMFLOAT4 color;
		XMFLOAT4 e_color = {1, 0, 0, 1};
		XMFLOAT4 s_color = {1, 0, 0, 1};
		XMFLOAT3 rot = {0, 0, 0};
	};

private: // �萔

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	/// <returns>����</returns>
	static bool StaticInitialize(int window_width, int window_height);

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

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	//static ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texbuff[100];
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �r���[�s��
	UINT texNumber = 0;
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//�r���{�[�h�s��
	static XMMATRIX matBillboard;
	static XMMATRIX matBillboardY;
private: // �ÓI�����o�֐�
	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	/// <returns></returns>
	static bool InitializeDescriptorHeap();

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

	/// <summary>
	/// ���f���쐬
	/// </summary>

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	static void UpdateViewMatrix();

public:
	static bool LoadTexture(UINT texnum, const wchar_t* filename);
	void CreateModel();
	void SetColor(XMFLOAT4 color) { this->color = color; }
private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	// �F
	//XMFLOAT4 colors;// = colors;
	// ���[�J���X�P�[��
	XMFLOAT3 scale = {1, 1, 1};

	//�p�[�e�B�N���z��
	std::forward_list<Particle> particles;

public:
	enum ParticleType
	{
		NORMAL,
		ABSORPTION,
		FOLLOW,
		CHARGE
	};

	//���ʂ̊g�U�G�t�F�N�g
	void Normal();
	//��苗���s������p�[�e�B�N�������̈ʒu�ɂ��ǂ��Ă���
	void Absorption();
	//��苗���s������position�����ɖ߂��Ă���
	void Follow(XMFLOAT3 position, int lifejudg);
	//�`���[�W���ǂ�
	void Charge(XMFLOAT3 position);
public: // �����o�֐�
	bool Initialize(UINT texnum);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(ParticleType type, XMFLOAT3 position = {0, 0, 0}, int lifejudg = false);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	//�p�[�e�B�N���̒ǉ�
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale);
	static ParticleManager* Create(UINT texnum, const wchar_t* filename);
};
