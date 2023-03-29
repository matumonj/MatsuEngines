#include "f_Object3d.h"
#include "f_Model.h"
#include "FbxLoader.h"
#include"DirectXCommon.h"

#include<vector>
#include"BaseCollider.h"
#include"CollisionManager.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

ComPtr<ID3D12RootSignature> f_Object3d::rootsignature;
ComPtr<ID3D12PipelineState> f_Object3d::pipelinestate;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ComPtr<ID3D12Device> f_Object3d::device = nullptr;
Camera* f_Object3d::camera = nullptr;
ComPtr<ID3D12GraphicsCommandList> f_Object3d::cmdList = nullptr;

LightGroup* f_Object3d::lightGroups = nullptr;

void f_Object3d::CreateGraphicsPipeline()
{
	device = DirectXCommon::GetIns()->GetDev();
	cmdList = DirectXCommon::GetIns()->GetCmdList();

	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g


	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shader/FBXVS.hlsl", // �V�F�[�_�t�@�C����
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
		L"Resources/Shader/FBXPS.hlsl", // �V�F�[�_�t�@�C����
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
		{
			//�e�����󂯂�{�[���ԍ��i4�j
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			//�{�[���̃X�L���E�F�C�g�i4�jBONEWEIGHTS
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
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
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;

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
	CD3DX12_ROOT_PARAMETER rootparams[4];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//CBV�i�X�L�j���O�p�j
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	// �X�^�e�B�b�N�T���v���[
	auto samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

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
	                                     IID_PPV_ARGS(rootsignature.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		//assert(0);
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipelinestate.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		//assert(0);
	}
}

void f_Object3d::Initialize()
{
	device = DirectXCommon::GetIns()->GetDev();
	cmdList = DirectXCommon::GetIns()->GetCmdList();
	ambient = {0.3f, 0.3f, 0.3f};
	diffuse = {0.0f, 0.0f, 0.0f};
	specular = {0.0f, 0.0f, 0.0f};
	alpha = 1.0f;
	HRESULT result;
	//�萔�o�b�t�@
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin));

	//1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	color = {1, 1, 1, 1};
}

void f_Object3d::Updata(bool animeloop)
{
	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity(); //�ό`�����Z�b�g
	matWorld *= matScale; //���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; //���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; //���[���h�s��ɕ��s�ړ��𔽉f

	//handa = modelTransform * matWorld;
	//�r���[�v���W�F�N�V�����s��
	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	//���f���̃��b�V���g�����X�t�H�[��
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//�J�������W
	const XMFLOAT3& cameraPos = camera->GetEye();

	//	handa = modelTransform * matWorld;
	HRESULT result;
	//�萔�o�b�t�@�ւ̃f�[�^�]��
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	matWorld = modelTransform * matWorld;

	if (SUCCEEDED(result))
	{
		constMap->color = this->color;
		constMap->viewproj = matViewProjection;
		constMap->world = matWorld;
		constMap->cameraPos = camera->GetEye();
		constBuffTransform->Unmap(0, nullptr);
	}

	//�{�[���z��
	std::vector<f_Model::Bone>& bones = model->GetBones();

	start_time = static_cast<float>(startTime.GetSecondDouble());
	end_time = static_cast<float>(endTime.GetSecondDouble());
	//�A�j���[�V����

	currentTime.SetSecondDouble(f_time);
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//���̎p��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		//auto& bindMatrix = model->GetModelTransform();
		//auto inverseBindMatrix = XMMatrixInverse(nullptr, bindMatrix);
		//constMapSkin->bones[i] = bindMatrix * bones[i].invInitialPose * matCurrentPose * inverseBindMatrix;
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}

	int num = bindexs;
	FbxLoader::ConvertMatrixFromFbx(&HandMatWorld,
	                                bones[num].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime));

	HandMatWorld = HandMatWorld * matWorld;

	constBuffSkin->Unmap(0, nullptr);


	ConstBufferDataB1* constMapB1 = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMapB1);
	if (SUCCEEDED(result))
	{
		constMapB1->ambient = ambient;
		constMapB1->diffuse = diffuse;
		constMapB1->specular = specular;
		constMapB1->alpha = alpha;
		constBuff->Unmap(0, nullptr);
	}
}

void f_Object3d::Update(bool Loop, double Speed, bool& Stop)
{
	XMMATRIX matScale, matRot, matTrans;

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity(); //�ό`�����Z�b�g
	matWorld *= matScale; //���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; //���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; //���[���h�s��ɕ��s�ړ��𔽉f

	//�r���[�v���W�F�N�V�����s��
	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	//���f���̃��b�V���g�����X�t�H�[��
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//�J�������W
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;
	//�萔�o�b�t�@�ւ̃f�[�^�]��
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);

	if (SUCCEEDED(result))
	{
		constMap->color = this->color;
		constMap->viewproj = matViewProjection;
		constMap->world = matWorld;
		constMap->cameraPos = fogpos;
		constBuffTransform->Unmap(0, nullptr);
	}


	//�{�[���z��
	std::vector<f_Model::Bone>& bones = model->GetBones();

	//�A�j���[�V����

	if (isPlay)
	{
		//1�t���[���i�߂�
		AnimationSpeed += frameTime.GetSecondDouble() * Speed;

		//�Ō�܂ōĐ�������擪�ɖ߂�
		if (Loop)
		{
			if (AnimationSpeed > endTime.GetSecondDouble())
			{
				AnimationSpeed = startTime.GetSecondDouble();
			}
		}
		else
		{
			if (AnimationSpeed > endTime.GetSecondDouble())
			{
				isPlay = false;
				if (Stop)
				{
					Stop = false;
				}
			}
		}

		currentTime.SetSecondDouble(AnimationSpeed);
	}
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//���̎p��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}


	//IKEFbxLoader::ConvertMatrixFromFbx(&hRot, bones[BoneNumber].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime));
	//XMMATRIX matRot2 = XMMatrixIdentity();
	//matRot2 *= XMMatrixRotationZ(XMConvertToRadians((float)PosNode2[0]));
	//matRot2 *= XMMatrixRotationX(XMConvertToRadians((float)PosNode2[1]));
	//matRot2 *= XMMatrixRotationY(XMConvertToRadians((float)PosNode2[2]));

	//RotMat = matRot2;
	//rot = hRot * matWorld;

	int num = bindexs;
	FbxLoader::ConvertMatrixFromFbx(&HandMatWorld,
	                                bones[num].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime));


	HandMatWorld = HandMatWorld * matWorld;

	constBuffSkin->Unmap(0, nullptr);


	ConstBufferDataB1* constMapB1 = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMapB1);
	if (SUCCEEDED(result))
	{
		constMapB1->ambient = ambient;
		constMapB1->diffuse = diffuse;
		constMapB1->specular = specular;
		constMapB1->alpha = alpha;
		constBuff->Unmap(0, nullptr);
	}
}

void f_Object3d::Updata()
{
	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity(); //�ό`�����Z�b�g
	matWorld *= matScale; //���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; //���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; //���[���h�s��ɕ��s�ړ��𔽉f

	//handa = modelTransform * matWorld;
	//�r���[�v���W�F�N�V�����s��
	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	//���f���̃��b�V���g�����X�t�H�[��
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//�J�������W
	const XMFLOAT3& cameraPos = camera->GetEye();

	//	handa = modelTransform * matWorld;
	HRESULT result;
	//�萔�o�b�t�@�ւ̃f�[�^�]��
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	matWorld = modelTransform * matWorld;

	if (SUCCEEDED(result))
	{
		constMap->color = this->color;
		constMap->viewproj = matViewProjection;
		constMap->world = matWorld;
		constMap->cameraPos = fogpos;

		constBuffTransform->Unmap(0, nullptr);
	}


	//�{�[���z��
	std::vector<f_Model::Bone>& bones = model->GetBones();

	start_time = static_cast<float>(startTime.GetSecondDouble());
	end_time = endTime.GetSecondDouble();
	//�A�j���[�V����

	currentTime.SetSecondDouble(f_time);
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//���̎p��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		auto& bindMatrix = model->GetModelTransform();
		auto inverseBindMatrix = XMMatrixInverse(nullptr, bindMatrix);
		constMapSkin->bones[i] = bindMatrix * bones[i].invInitialPose * matCurrentPose * inverseBindMatrix;
		//constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}

	int num = bindexs;
	FbxLoader::ConvertMatrixFromFbx(&HandMatWorld,
	                                bones[num].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime));


	constBuffSkin->Unmap(0, nullptr);


	ConstBufferDataB1* constMapB1 = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMapB1);
	if (SUCCEEDED(result))
	{
		constMapB1->ambient = ambient;
		constMapB1->diffuse = diffuse;
		constMapB1->specular = specular;
		constMapB1->alpha = alpha;
		constBuff->Unmap(0, nullptr);
	}
}

void f_Object3d::GetBoneIndexMat(int index, XMMATRIX& matworld)
{
	FbxLoader::ConvertMatrixFromFbx(&matworld,
	                                model->GetBones()[index].fbxCluster->GetLink()->
	                                                         EvaluateGlobalTransform(currentTime));

	matworld = matworld * matWorld;
}

void f_Object3d::Draw()
{
	//���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ�
	if (model == nullptr)
	{
		return;
	}
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());

	// ���C�g�̕`��

	lightGroups->Draw(cmdList.Get(), 3);

	//���f���`��
	model->Draw();
}

void f_Object3d::PlayAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	//0�Ԃ̃A�j���[�V�����擾
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	//�A�j���[�V�����̖��O�擾
	const char* animstackname = animstack->GetName();
	//�A�j���[�V�����̎��Ԏ擾
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	//�J�n���Ԏ擾
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime = startTime;
	//�Đ�����Ԃɂ���
	isPlay = true;
}

void f_Object3d::PlayAnimation(int number)
{
	FbxScene* fbxScene = model->GetFbxScene();
	//0�Ԃ̃A�j���[�V�����擾
	//FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(2);
	//�A�j���[�V�����̕ύX
	fbxScene->SetCurrentAnimationStack(animationData[number].stack);
	////�A�j���[�V�����̖��O�擾
	//const char* animstackname = animstack->GetName();
	////�A�j���[�V�����̎��Ԏ擾
	//FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);
	//�J�n���Ԏ擾
	startTime = animationData[number].info->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = animationData[number].info->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	AnimationSpeed = startTime.GetSecondDouble();
	//�Đ�����Ԃɂ���
	isPlay = true;
}


void f_Object3d::LoadAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	//�A�j���[�V�����J�E���g
	int AnimationCount = fbxScene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < AnimationCount; i++)
	{
		//���f�[�^
		Animation tempData;
		//i�Ԃ̃A�j���[�V�����擾
		tempData.stack = fbxScene->GetSrcObject<FbxAnimStack>(i);
		//�A�j���[�V�����̖��O���擾
		const char* animstackname = tempData.stack->GetName();
		//�A�j���[�V�����̎��ԏ��
		tempData.info = fbxScene->GetTakeInfo(animstackname);
		//���f�[�^�����f�[�^�ɓ����
		animationData.push_back(tempData);
	}
}

XMMATRIX f_Object3d::ExtractRotationMat(XMMATRIX matworld)
{
	XMMATRIX mOffset = ExtractPositionMat(matworld);
	XMMATRIX mScaling = ExtractScaleMat(matworld);

	XMVECTOR det;
	// ������Scaling�A�E����Offset�̋t�s������ꂼ�ꂩ����B
	return XMMatrixInverse(&det, mScaling) * matworld * XMMatrixInverse(&det, mOffset);
}

XMMATRIX f_Object3d::ExtractScaleMat(XMMATRIX matworld)
{
	return XMMatrixScaling(
		XMVector3Length(XMVECTOR{
			matworld.r[0].m128_f32[0], matworld.r[0].m128_f32[1], matworld.r[0].m128_f32[2]
		}).m128_f32[0],
		XMVector3Length(XMVECTOR{
			matworld.r[1].m128_f32[0], matworld.r[1].m128_f32[1], matworld.r[1].m128_f32[2]
		}).m128_f32[0],
		XMVector3Length(XMVECTOR{
			matworld.r[2].m128_f32[0], matworld.r[2].m128_f32[1], matworld.r[2].m128_f32[2]
		}).m128_f32[0]
	);
}

XMMATRIX f_Object3d::ExtractPositionMat(XMMATRIX matworld)
{
	return XMMatrixTranslation(matworld.r[3].m128_f32[0], matworld.r[3].m128_f32[1], matworld.r[3].m128_f32[2]);
}