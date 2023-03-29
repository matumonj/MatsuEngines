#include "Texture.h"

#include <d3dcompiler.h>
#include <DirectXTex.h>
#include"DirectXCommon.h"
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
const float Texture::radius = 5.0f; // ��ʂ̔��a
const float Texture::prizmHeight = 8.0f; // ���̍���
ComPtr<ID3D12Device> Texture::device = nullptr;
UINT Texture::descriptorHandleIncrementSize = 0;
ComPtr<ID3D12GraphicsCommandList> Texture::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Texture::rootsignature;
ComPtr<ID3D12PipelineState> Texture::pipelinestate;
ComPtr<ID3D12DescriptorHeap> Texture::descHeap;
//ComPtr<ID3D12Resource> Texture::vertBuff;
//ComPtr<ID3D12Resource> Texture::indexBuff;
ComPtr<ID3D12Resource> Texture::texbuff[srvCount];
CD3DX12_CPU_DESCRIPTOR_HANDLE Texture::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE Texture::gpuDescHandleSRV;
XMMATRIX Texture::matView{};
XMMATRIX Texture::matProjection{};
XMFLOAT3 Texture::eye = {0, 0, -50.0f};
XMFLOAT3 Texture::target = {0, 0, 0};
XMFLOAT3 Texture::up = {0, 1, 0};
Texture::VertexPosNormalUv Texture::vertices[vertexCount];
unsigned short Texture::indices[indexCount];

Texture::Texture(UINT texNumber, XMFLOAT3 position, XMFLOAT3 size, XMFLOAT4 color)
{
	this->position = position;
	this->scale = size;
	//this->anchorpoint = anchorpoint;//
	this->matWorld = XMMatrixIdentity();
	this->color = color;
	this->texNumber = texNumber;
	//this->texSize = size;
}

bool Texture::StaticInitialize(int window_width, int window_height)
{
	device = DirectXCommon::GetIns()->GetDev();
	cmdList = DirectXCommon::GetIns()->GetCmdList();

	// �f�X�N���v�^�q�[�v�̏�����
	InitializeDescriptorHeap();

	// �J����������
	//InitializeCamera(window_width, window_height);
	// �p�C�v���C��������
	InitializeGraphicsPipeline();


	return true;
}

void Texture::PreDraw()
{
	// �R�}���h���X�g���Z�b�g
	cmdList = DirectXCommon::GetIns()->GetCmdList();

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Texture::PostDraw()
{
	// �R�}���h���X�g������
	cmdList = nullptr;
}

bool Texture::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = srvCount; // �V�F�[�_�[���\�[�X�r���[1��
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap)); //����
	if (FAILED(result))
	{
		//assert(0);
		return false;
	}

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

bool Texture::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shader/TextureVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(static_cast<char*>(errorBlob->GetBufferPointer()),
		            errorBlob->GetBufferSize(),
		            errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shader/TexturePS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(static_cast<char*>(errorBlob->GetBufferPointer()),
		            errorBlob->GetBufferSize(),
		            errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			// xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			// �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			// uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//gpipeline.BlendState.AlphaToCoverageEnable = true;
	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	auto samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
	                           D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob,
	                                               &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
	                                     IID_PPV_ARGS(&rootsignature));
	if (FAILED(result))
	{
		return false;
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Texture::LoadTexture(UINT texnumber, const wchar_t* filename)
{
	// nullptr�`�F�b�N


	HRESULT result;
	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result))
	{
		////assert(0);
		return false;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		static_cast<UINT>(metadata.height),
		static_cast<UINT16>(metadata.arraySize),
		static_cast<UINT16>(metadata.mipLevels)
	);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texbuff[texnumber]));
	if (FAILED(result))
	{
		////assert(0);
		return false;
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texbuff[texnumber]->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels, // ���f�[�^�A�h���X
		static_cast<UINT>(img->rowPitch), // 1���C���T�C�Y
		static_cast<UINT>(img->slicePitch // 1���T�C�Y
		) // 1���T�C�Y
	);
	if (FAILED(result))
	{
		//assert(0);
		return false;
	}
	// �V�F�[�_���\�[�X�r���[�쐬
	//	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	//gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	// �V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texbuff[texnumber]->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texbuff[texnumber].Get(), //�r���[�Ɗ֘A�t����o�b�t�@
	                                 &srvDesc, //�e�N�X�`���ݒ���
	                                 CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(),
	                                                               texnumber, descriptorHandleIncrementSize)
	);

	return true;
}

void Texture::SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize)
{
	this->texBase = texBase;
	this->texSize = texSize;

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();
}

void Texture::CreateTexture()
{
	HRESULT result = S_FALSE;

	std::vector<VertexPosNormalUv> realVertices;
	// �e�N�X�`�����擾

	if (texbuff[texNumber])
	{
		D3D12_RESOURCE_DESC resDesc = texbuff[texNumber]->GetDesc();

		VertexPosNormalUv verticesSquare[] = {
			{{-5.0f, -5.0f, 0.0f}, {0, 0, 1}, {0, 1}},
			{{-5.0f, +5.0f, 0.0f}, {0, 0, 1}, {0, 0}},
			{{+5.0f, -5.0f, 0.0f}, {0, 0, 1}, {1, 1}},
			{{+5.0f, +5.0f, 0.0f}, {0, 0, 1}, {1, 0}},
		};
		std::copy(std::begin(verticesSquare), std::end(verticesSquare), vertices);

		unsigned short indicesSquare[] = {
			0, 1, 2,
			2, 1, 3,
		};
		std::copy(std::begin(indicesSquare), std::end(indicesSquare), indices);
	}
	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result))
	{
		//assert(0);
		return;
	}

	// �C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result))
	{
		//assert(0);
		return;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		// �S�C���f�b�N�X�ɑ΂���
		for (int i = 0; i < _countof(indices); i++)
		{
			indexMap[i] = indices[i]; // �C���f�b�N�X���R�s�[
		}

		indexBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);
}


bool Texture::Initialize()
{
	// nullptr�`�F�b�N


	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	return true;
}

void Texture::Update(DebugCamera* camera)
{
	//anchorpoint = { 0,0 };
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//if (isBillboard) {
	const XMMATRIX& matBillboard = camera->GetBillboardMatrix();

	matWorld = XMMatrixIdentity();
	matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
	if (BillBordflag)
	{
		matWorld *= matBillboard;
	}
	matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f
	//}
	// ���[���h�s��̍���

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->matbillbord = camera->GetBillboardMatrix();
	constMap->world = matWorld;
	constMap->mat = matWorld * camera->GetViewMatrix() * camera->GetProjectionMatrix(); // �s��̍���
	constMap->uvmove = uvf;
	constMap->time = uvtime;
	constMap->dispos = position;
	constMap->radius = DisplayRadius;
	constBuff->Unmap(0, nullptr);
	//TransferVertices();
}

void Texture::Update(XMMATRIX matw, DebugCamera* camera)
{
	//anchorpoint = { 0,0 };
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//if (isBillboard) {
	const XMMATRIX& matBillboard = camera->GetBillboardMatrix();

	matWorld = XMMatrixIdentity();
	matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot * matw; // ���[���h�s��ɉ�]�𔽉f
	if (BillBordflag)
	{
		matWorld *= matBillboard;
	}
	matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f
	//}
	// ���[���h�s��̍���

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->matbillbord = camera->GetBillboardMatrix();
	constMap->world = matWorld;
	constMap->mat = matWorld * camera->GetViewMatrix() * camera->GetProjectionMatrix(); // �s��̍���
	constMap->uvmove = uvf;
	constMap->time = uvtime;
	constMap->dispos = position;
	constMap->radius = DisplayRadius;
	constBuff->Unmap(0, nullptr);
	//TransferVertices();
}

void Texture::SetPosition(XMFLOAT3 position)
{
	this->position = position;
	TransferVertices();
}

void Texture::SetScale(XMFLOAT3 scale)
{
	this->scale = scale;
	TransferVertices();
}


void Texture::SetAnchorPoint(XMFLOAT2 anchorpoint)
{
	this->anchorpoint = anchorpoint;

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();
}

void Texture::Draw()
{
	uvtime += 0.01f;
	if (uvtime > 1.0f)
	{
		uvtime = 0.0f;
	}

	// ���[
	// nullptr�`�F�b�N

	//assert(Texture::cmdList);

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = {descHeap.Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		                                        descHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber,
		                                        descriptorHandleIncrementSize));

	//cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// �`��R�}���h
	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void Texture::TransferVertices()
{
	size = {scale.x, scale.y};
	HRESULT result = S_FALSE;

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;

	// ���_�f�[�^
	VertexPosNormalUv vertices[vertNum];

	vertices[LB].pos = {left, bottom, 0.0f}; // ����
	vertices[LT].pos = {left, top, 0.0f}; // ����
	vertices[RB].pos = {right, bottom, 0.0f}; // �E��
	vertices[RT].pos = {right, top, 0.0f}; // �E��

	// �e�N�X�`�����擾
	if (texbuff[texNumber])
	{
		D3D12_RESOURCE_DESC resDesc = texbuff[texNumber]->GetDesc();
		if (texNormalSize)
		{
			texSize = {static_cast<float>(resDesc.Width), static_cast<float>(resDesc.Height)};
		}
		float tex_left = texBase.x / resDesc.Width;
		float tex_right = (texBase.x + texSize.x) / resDesc.Width;
		float tex_top = texBase.y / resDesc.Height;
		float tex_bottom = (texBase.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = {tex_left, tex_bottom}; // ����
		vertices[LT].uv = {tex_left, tex_top}; // ����
		vertices[RB].uv = {tex_right, tex_bottom}; // �E��
		vertices[RT].uv = {tex_right, tex_top}; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

Texture* Texture::Create(UINT texNumber, XMFLOAT3 position, XMFLOAT3 size, XMFLOAT4 color)
{
	XMFLOAT3 texsize = {0.0f, 0.0f, 0.0f};
	if (texbuff[texNumber])
	{
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = texbuff[texNumber]->GetDesc();
		// �X�v���C�g�̃T�C�Y���e�N�X�`���̃T�C�Y�ɐݒ�
		texsize = {static_cast<float>(resDesc.Width), static_cast<float>(resDesc.Height), 0};
	}

	// Sprite�̃C���X�^���X�𐶐�
	auto texture = new Texture(texNumber, position, texsize, color);
	if (texture == nullptr)
	{
		return nullptr;
	}

	// ������
	if (!texture->Initialize())
	{
		delete texture;
		//assert(0);
		return nullptr;
	}

	return texture;
}

void Texture::SetRotation(XMFLOAT3 rotation)
{
	this->rotation = rotation;
	TransferVertices();
}