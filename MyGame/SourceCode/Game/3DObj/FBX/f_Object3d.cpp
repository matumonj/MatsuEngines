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
/// 静的メンバ変数の実体
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
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト


	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shader/FBXVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(static_cast<char*>(errorBlob->GetBufferPointer()),
		            errorBlob->GetBufferSize(),
		            errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shader/FBXPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(static_cast<char*>(errorBlob->GetBufferPointer()),
		            errorBlob->GetBufferSize(),
		            errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			// xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			// 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			// uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			//影響を受けるボーン番号（4つ）
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			//ボーンのスキンウェイト（4つ）BONEWEIGHTS
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0〜255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[4];
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//CBV（スキニング用）
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	// スタティックサンプラー
	auto samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
	                           D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob,
	                                               &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
	                                     IID_PPV_ARGS(rootsignature.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		//assert(0);
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
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
	//定数バッファ
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

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin));

	//1フレーム分の時間を60FPSで設定
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	color = {1, 1, 1, 1};
}

void f_Object3d::Updata(bool animeloop)
{
	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//ワールド行列の合成
	matWorld = XMMatrixIdentity(); //変形をリセット
	matWorld *= matScale; //ワールド行列にスケーリングを反映
	matWorld *= matRot; //ワールド行列に回転を反映
	matWorld *= matTrans; //ワールド行列に平行移動を反映

	//handa = modelTransform * matWorld;
	//ビュープロジェクション行列
	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	//モデルのメッシュトランスフォーム
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//カメラ座標
	const XMFLOAT3& cameraPos = camera->GetEye();

	//	handa = modelTransform * matWorld;
	HRESULT result;
	//定数バッファへのデータ転送
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

	//ボーン配列
	std::vector<f_Model::Bone>& bones = model->GetBones();

	start_time = static_cast<float>(startTime.GetSecondDouble());
	end_time = static_cast<float>(endTime.GetSecondDouble());
	//アニメーション

	currentTime.SetSecondDouble(f_time);
	//定数バッファへデータ転送
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//今の姿勢
		XMMATRIX matCurrentPose;
		//今の姿勢行列を取得
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIXに変換
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//合成してスキニング行列に
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

	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//ワールド行列の合成
	matWorld = XMMatrixIdentity(); //変形をリセット
	matWorld *= matScale; //ワールド行列にスケーリングを反映
	matWorld *= matRot; //ワールド行列に回転を反映
	matWorld *= matTrans; //ワールド行列に平行移動を反映

	//ビュープロジェクション行列
	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	//モデルのメッシュトランスフォーム
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//カメラ座標
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;
	//定数バッファへのデータ転送
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


	//ボーン配列
	std::vector<f_Model::Bone>& bones = model->GetBones();

	//アニメーション

	if (isPlay)
	{
		//1フレーム進める
		AnimationSpeed += frameTime.GetSecondDouble() * Speed;

		//最後まで再生したら先頭に戻す
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
	//定数バッファへデータ転送
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//今の姿勢
		XMMATRIX matCurrentPose;
		//今の姿勢行列を取得
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIXに変換
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//合成してスキニング行列に
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
	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//ワールド行列の合成
	matWorld = XMMatrixIdentity(); //変形をリセット
	matWorld *= matScale; //ワールド行列にスケーリングを反映
	matWorld *= matRot; //ワールド行列に回転を反映
	matWorld *= matTrans; //ワールド行列に平行移動を反映

	//handa = modelTransform * matWorld;
	//ビュープロジェクション行列
	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	//モデルのメッシュトランスフォーム
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//カメラ座標
	const XMFLOAT3& cameraPos = camera->GetEye();

	//	handa = modelTransform * matWorld;
	HRESULT result;
	//定数バッファへのデータ転送
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


	//ボーン配列
	std::vector<f_Model::Bone>& bones = model->GetBones();

	start_time = static_cast<float>(startTime.GetSecondDouble());
	end_time = endTime.GetSecondDouble();
	//アニメーション

	currentTime.SetSecondDouble(f_time);
	//定数バッファへデータ転送
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//今の姿勢
		XMMATRIX matCurrentPose;
		//今の姿勢行列を取得
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIXに変換
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//合成してスキニング行列に
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
	//モデルの割り当てがなければ描画しない
	if (model == nullptr)
	{
		return;
	}
	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());

	// ライトの描画

	lightGroups->Draw(cmdList.Get(), 3);

	//モデル描画
	model->Draw();
}

void f_Object3d::PlayAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	//0番のアニメーション取得
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	//アニメーションの名前取得
	const char* animstackname = animstack->GetName();
	//アニメーションの時間取得
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	//開始時間取得
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//終了時間取得
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//開始時間に合わせる
	currentTime = startTime;
	//再生中状態にする
	isPlay = true;
}

void f_Object3d::PlayAnimation(int number)
{
	FbxScene* fbxScene = model->GetFbxScene();
	//0番のアニメーション取得
	//FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(2);
	//アニメーションの変更
	fbxScene->SetCurrentAnimationStack(animationData[number].stack);
	////アニメーションの名前取得
	//const char* animstackname = animstack->GetName();
	////アニメーションの時間取得
	//FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);
	//開始時間取得
	startTime = animationData[number].info->mLocalTimeSpan.GetStart();
	//終了時間取得
	endTime = animationData[number].info->mLocalTimeSpan.GetStop();
	//開始時間に合わせる
	AnimationSpeed = startTime.GetSecondDouble();
	//再生中状態にする
	isPlay = true;
}


void f_Object3d::LoadAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	//アニメーションカウント
	int AnimationCount = fbxScene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < AnimationCount; i++)
	{
		//仮データ
		Animation tempData;
		//i番のアニメーション取得
		tempData.stack = fbxScene->GetSrcObject<FbxAnimStack>(i);
		//アニメーションの名前を取得
		const char* animstackname = tempData.stack->GetName();
		//アニメーションの時間情報
		tempData.info = fbxScene->GetTakeInfo(animstackname);
		//仮データを実データに入れる
		animationData.push_back(tempData);
	}
}

XMMATRIX f_Object3d::ExtractRotationMat(XMMATRIX matworld)
{
	XMMATRIX mOffset = ExtractPositionMat(matworld);
	XMMATRIX mScaling = ExtractScaleMat(matworld);

	XMVECTOR det;
	// 左からScaling、右からOffsetの逆行列をそれぞれかける。
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
