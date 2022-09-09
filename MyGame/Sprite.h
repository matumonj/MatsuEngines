#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Sprite
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public:
	//Sprite();
	//�X�v���C�g�p
	struct VertexPosUv
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};
	//�萔�o�b�t�@�p
	struct ConstBufferData
	{
		XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��

	};
	
	static bool StaticInitialize(int window_width, int window_height);
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);
	static void PreDraw();
	static void PostDraw();
	static Sprite* Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f });
protected:
	// �e�N�X�`���̍ő喇��
	static const int srvCount = 512;
	// ���_��
	static const int vertNum = 4;
	// �f�o�C�X
	static ComPtr<ID3D12Device> device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ComPtr<ID3D12GraphicsCommandList> cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelineState;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texBuff[srvCount];
public:
	//�R���X�g���N�^
	Sprite();
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint);
	//������
	bool Initialize();

	//�p�x�̐ݒ�
	void SetRotation(float rotation);

	//���W�̎擾
	const XMFLOAT2& GetPosition() { return position; }

	//���W�̐ݒ�
	void SetPosition(XMFLOAT2 position);

	//�T�C�Y�̐ݒ�
	void SetSize(XMFLOAT2 size);

	//�A���J�[�|�C���g�̐ݒ�
	void SetAnchorPoint(XMFLOAT2 anchorpoint);

	//�e�N�X�`���͈͐ݒ�
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize);
	void SpriteUpdate(XMMATRIX matview, XMMATRIX matprojection, XMFLOAT4 color);
	//�`��
	void Draw();
	void feed(float& feed);
protected:
	bool CollisionCursul;
	int setf = 0;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �e�N�X�`���ԍ�
	UINT texNumber = 0;
	// Z�����̉�]�p
	float rotation = 0.0f;
	// ���W
	XMFLOAT2 position{};
	// �X�v���C�g���A����
	XMFLOAT2 size = { 100.0f, 100.0f };
	// �A���J�[�|�C���g
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ���[���h�s��
	XMMATRIX matWorld{};
	// �F
	XMFLOAT4 color;
	// �e�N�X�`���n�_
	XMFLOAT2 texBase = { 0, 0 };
	// �e�N�X�`�����A����
	XMFLOAT2 texSize = { 100.0f, 100.0f };
bool f;
public:
	void SetHUDLayOutFlag(bool flag) { f = flag; }
	bool GetHUDLayOutFlag() { return f; }
	void Setfeed(int set) { this->setf = set; }
	void setcolor(XMFLOAT4 color){ this->color = color; }

	void TransferVertices();
	XMFLOAT2 GetSize() { return size; }
	void ChangePosition_PlayerHP(XMFLOAT2 MousePosition);
	void ChangePosition(XMFLOAT2 MousePosition,XMFLOAT2 &c);
};
