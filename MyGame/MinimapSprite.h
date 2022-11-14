#pragma once
#include "Sprite.h"

class MinimapSprite :
	public Sprite
{
private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	//�e�N�X�`���o�b�t�@
	//ComPtr<ID3D12Resource>texBuff;
	//�ύX��
	ComPtr<ID3D12Resource> texBuff[2];
	//SRV�p�ŃX�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descheapSRV;
	//�[�x�o�b�t�@    
	ComPtr<ID3D12Resource> depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//�O���t�B�b�N�X�p�C�v���C��
	ComPtr<ID3D12PipelineState> pipelineState;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	//�f�o�C�X
	static ComPtr<ID3D12Device> device;
	static ComPtr<ID3D12GraphicsCommandList> cmdList;
private:
	//��ʃN���A�J���[
	static const float clearColor[4];
	XMFLOAT2 centerpos;
public:
	struct MinimapConstBufferData
	{
		XMFLOAT2 centerpos; // �R�c�ϊ��s��
	};

	void SetCenterpos(XMFLOAT2 pos) { centerpos = pos; }
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	MinimapSprite();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	/// <param name="cmdList"></param>
	void CreateGraphicsPipeline();
	///������
	void Initialize();

	/// <summary>
	/// �`��R�}���h
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw();

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	/// <param name="cmdList"></param> 
	void PreDrawScene();

	/// <summary>
   /// �V�[���`��㏈��
   /// </summary>
   /// <param name="cmdList"></param>
	void PostDrawScene();
};
