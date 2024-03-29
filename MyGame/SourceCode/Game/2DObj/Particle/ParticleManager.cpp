#include "ParticleManager.h"
#include <d3dcompiler.h>
#include<DirectXTex.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"DirectXCommon.h"
using namespace DirectX;
using namespace Microsoft::WRL;
#include"CameraControl.h"
#include"PlayerControl.h"
/// <summary>
/// 静的メンバ変数の実体
/// </summary>			// 柱の高さ
ID3D12Device* ParticleManager::device = nullptr;
UINT ParticleManager::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* ParticleManager::cmdList = nullptr;
ComPtr<ID3D12RootSignature> ParticleManager::rootsignature;
ComPtr<ID3D12PipelineState> ParticleManager::pipelinestate;
ComPtr<ID3D12DescriptorHeap> ParticleManager::descHeap;
CD3DX12_CPU_DESCRIPTOR_HANDLE ParticleManager::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE ParticleManager::gpuDescHandleSRV;
XMMATRIX ParticleManager::matView{};
XMMATRIX ParticleManager::matProjection{};
XMFLOAT3 ParticleManager::eye = {0, 0, 10.0f};
XMFLOAT3 ParticleManager::target = {0, 0, 0};
XMFLOAT3 ParticleManager::up = {0, 1, 0};
ComPtr<ID3D12Resource> ParticleManager::texbuff[100];
XMMATRIX ParticleManager::matBillboard = XMMatrixIdentity();
XMMATRIX ParticleManager::matBillboardY = XMMatrixIdentity();

bool ParticleManager::StaticInitialize(int window_width, int window_height)
{
	device = DirectXCommon::GetIns()->GetDev();

	// デスクリプタヒープの初期化
	InitializeDescriptorHeap();

	// カメラ初期化
	InitializeCamera(window_width, window_height);

	// パイプライン初期化
	InitializeGraphicsPipeline();

	return true;
}

void ParticleManager::PreDraw()
{
	// コマンドリストをセット
	cmdList = DirectXCommon::GetIns()->GetCmdList();

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ParticleManager::PostDraw()
{
	// コマンドリストを解除
	cmdList = nullptr;
}

ParticleManager* ParticleManager::Create(UINT texnum, const wchar_t* filename)
{
	// 3Dオブジェクトのインスタンスを生成
	auto object3d = new ParticleManager();
	if (object3d == nullptr)
	{
		return nullptr;
	}
	object3d->CreateModel();
	// 初期化
	if (!object3d->Initialize(texnum))
	{
		delete object3d;
		//assert(0);
		return nullptr;
	}

	return object3d;
}


bool ParticleManager::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap)); //生成
	if (FAILED(result))
	{
		//assert(0);
		return false;
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

void ParticleManager::InitializeCamera(int window_width, int window_height)
{
}

bool ParticleManager::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob; // ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shader/ParticleVS.hlsl", // シェーダファイル名
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
		L"Resources/Shader/ParticlePS.hlsl", // シェーダファイル名
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

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shader/ParticleGS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
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
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			// xy座標(1行で書いたほうが見やすい)
			"ROTATION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			// 法線ベクトル(1行で書いたほうが見やすい)
			"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
		//{ // uv座標(1行で書いたほうが見やすい)
		//	"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		//},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	//加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;
	//デプスの書き込みを禁止
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1; // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0〜255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

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
	                                     IID_PPV_ARGS(&rootsignature));
	if (FAILED(result))
	{
		return result;
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result))
	{
		return result;
	}

	return true;
}

bool ParticleManager::LoadTexture(UINT texnumber, const wchar_t* filename)
{
	// nullptrチェック


	HRESULT result;
	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result))
	{
		//assert(0);
		//return false;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		static_cast<UINT>(metadata.height),
		static_cast<UINT16>(metadata.arraySize),
		static_cast<UINT16>(metadata.mipLevels)
	);

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff[texnumber]));
	if (FAILED(result))
	{
		//assert(0);
		//return false;
	}

	// テクスチャバッファにデータ転送
	result = texbuff[texnumber]->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels, // 元データアドレス
		static_cast<UINT>(img->rowPitch), // 1ラインサイズ
		static_cast<UINT>(img->slicePitch // 1枚サイズ
		) // 1枚サイズ
	);
	if (FAILED(result))
	{
		//assert(0);
		//return false;
	}
	// シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0,
	                                                 descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0,
	                                                 descriptorHandleIncrementSize);

	// シェーダリソースビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff[texnumber]->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texbuff[texnumber].Get(), //ビューと関連付けるバッファ
	                                 &srvDesc, //テクスチャ設定情報
	                                 CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(),
	                                                               texnumber, descriptorHandleIncrementSize)
	);

	return true;
}

void ParticleManager::CreateModel()
{
	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPos) * 1024),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result))
	{
		//assert(0);
		return;
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPos) * 1024;
	vbView.StrideInBytes = sizeof(VertexPos);
}

void ParticleManager::UpdateViewMatrix()
{
}

bool ParticleManager::Initialize(UINT texnum)
{
	texNumber = texnum;
	// nullptrチェック


	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	return true;
}

//XMFLOAT3同士の加算処理
const XMFLOAT3 operator+(const XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}


void ParticleManager::Update(ParticleType type, XMFLOAT3 position, int lifejudg)
{
	HRESULT result;
	//寿命が尽きた物を削除
	particles.remove_if(
		[](Particle& x)
		{
			return x.frame >= x.num_frame;
		}
	);
	if (type == NORMAL)
	{
		Normal();
	}
	else if (type == ABSORPTION)
	{
		Absorption();
	}
	else if (type == FOLLOW)
	{
		Follow(position, lifejudg);
	}
	else if (type == CHARGE)
	{
		Charge(position);
	}

	//頂点バッファへデータ転送

	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		//パーティクルの情報を一つずつ反映
		for (auto it = particles.begin();
		     it != particles.end(); ++it)
		{
			//座標
			vertMap->pos = it->position;
			vertMap->scale = it->scale;
			vertMap->rot = it->rot;
			//次の頂点へ
			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}
	const XMMATRIX& matBillboard = CameraControl::GetIns()->GetCamera()->GetBillboardMatrix();

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->mat = CameraControl::GetIns()->GetCamera()->GetViewMatrix() * CameraControl::GetIns()->
	                                                                        GetCamera()->GetProjectionMatrix(); // 行列の合成

	constMap->matBillboard = matBillboard; // 行列の合成

	constBuff->Unmap(0, nullptr);
}

void ParticleManager::Draw()
{
	// nullptrチェック

	//assert(ParticleManager::cmdList);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファの設定
	//cmdList->IASetIndexBuffer(&ibView);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = {descHeap.Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		                                        descHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber,
		                                        descriptorHandleIncrementSize));
	// 描画コマンド
	cmdList->DrawInstanced(static_cast<UINT>(std::distance(particles.begin(), particles.end())), 1, 0, 0);
}

void ParticleManager::Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale,
                          float end_scale)
{
	//リストに要素を追加
	particles.emplace_front();
	//追加した要素の参照
	Particle& p = particles.front();
	//値のセット
	p.e_scale = end_scale;
	p.s_scale = start_scale;
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
}

void ParticleManager::Absorption()
{
	//全パーティクル更新
	for (auto it = particles.begin();
	     it != particles.end(); ++it)
	{
		//経過フレーム数をカウント
		it->frame++;

		//速度に加速度を加算
		if (it->frame < 60)
		{
			it->velocity_old = it->velocity;
			it->velocity = it->velocity + it->accel;
		}
		else
		{
			it->velocity = {
				it->velocity_old.x * -1 - it->accel.x, it->velocity_old.y * -1 - it->accel.y, it->velocity_old.z
			};
		}
		//速度による移動
		it->position = it->position + it->velocity;
		float f = static_cast<float>(it->num_frame) / it->frame;
		it->scale = (it->e_scale - it->s_scale) / f;
		it->scale += it->s_scale;
		//it->colors =XMFLOAT4(0,0,0,1) ;
	}
}

#include"PlayerAttackState.h"

void ParticleManager::Normal()
{
	//全パーティクル更新
	for (auto it = particles.begin();
	     it != particles.end(); ++it)
	{
		//経過フレーム数をカウント
		it->frame++;
		it->velocity = it->velocity + it->accel;
		//速度による移動
		if (PlayerAttackState::GetIns()->GetHitStopJudg())
		{
			it->position = it->position + it->velocity;
		}
		else
		{
			it->position = {
				it->position.x + it->velocity.x / 2, it->position.y + it->velocity.y / 2,
				it->position.z + it->velocity.z / 2
			};
		}
		float f = static_cast<float>(it->num_frame) / it->frame;
		it->scale = (it->e_scale - it->s_scale) / f;
		it->scale += it->s_scale;
	}
}

void ParticleManager::Follow(XMFLOAT3 position, int lifejudg)
{
	//全パーティクル更新
	for (auto it = particles.begin();
	     it != particles.end(); ++it)
	{
		//経過フレーム数をカウント
		float dis;
		//追跡スピード
		float centerSpeed = 0.2f;

		if (it->frame > lifejudg)
		{
			//敵とプレイヤーの距離求め
			float disX = PlayerControl::GetIns()->GetPlayer()->GetPosition().x - it->position.x;
			float disY = PlayerControl::GetIns()->GetPlayer()->GetPosition().y - it->position.y;
			float disZ = PlayerControl::GetIns()->GetPlayer()->GetPosition().z - it->position.z;
			dis = Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(), it->position);
			//trad=(PlayerControl::GetIns()->GetPlayer()->GetPosition().y - it->position.y, PlayerControl::GetIns()->GetPlayer()->GetPosition().x - it->position.x);

			//座標のセット
			if (dis > 1.0f)
			{
				//座標のセット
				it->position = {
					it->position.x + (disX / dis) * centerSpeed, it->position.y + (disY / dis) * centerSpeed,
					it->position.z + (disZ / dis) * centerSpeed
				};
			}
		}
		else
		{
			//速度による移動
			it->position = it->position + it->velocity;
		}
		it->frame++;
		it->velocity = it->velocity + it->accel;

		float f = static_cast<float>(it->num_frame) / it->frame;
		it->scale = (it->e_scale - it->s_scale) / f;
		it->scale += it->s_scale;
		//it->colors =XMFLOAT4(0,0,0,1) ;
	}
}

void ParticleManager::Charge(XMFLOAT3 position)
{
	//全パーティクル更新
	for (auto it = particles.begin();
	     it != particles.end(); ++it)
	{
		//経過フレーム数をカウント
		float dis;
		//追跡スピード
		float centerSpeed = 0.2f;

		//敵とプレイヤーの距離求め
		float disX = PlayerControl::GetIns()->GetPlayer()->GetPosition().x - it->position.x;
		float disY = PlayerControl::GetIns()->GetPlayer()->GetPosition().y - it->position.y;
		float disZ = PlayerControl::GetIns()->GetPlayer()->GetPosition().z - it->position.z;
		dis = Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(), it->position);
		//trad=(PlayerControl::GetIns()->GetPlayer()->GetPosition().y - it->position.y, PlayerControl::GetIns()->GetPlayer()->GetPosition().x - it->position.x);

		//座標のセット
		if (dis > 1.0f)
		{
			//座標のセット
			it->position = {
				it->position.x + (disX / dis) * centerSpeed, it->position.y + (disY / dis) * centerSpeed,
				it->position.z + (disZ / dis) * centerSpeed
			};
		}
		it->frame++;
		it->velocity = it->velocity + it->accel;

		float f = static_cast<float>(it->num_frame) / it->frame;
		it->scale = (it->e_scale - it->s_scale) / f;
		it->scale += it->s_scale;
		//it->colors =XMFLOAT4(0,0,0,1) ;
	}
}
