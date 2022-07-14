#include "mEffekseer.h"

//カメラ設定
void mEffekseer::SyncronizeEffekseerCamera( DebugCamera* camera)
{
	Effekseer::Matrix44 fkViewMat;
	Effekseer::Matrix44 fkProjMat;
	auto view = camera->GetViewMatrix();//カメラのセット
	auto proj = camera->GetProjectionMatrix();//射影、透視投影を代入
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			fkViewMat.Values[i][j] = view.r[i].m128_f32[j];
			fkProjMat.Values[i][j] = proj.r[i].m128_f32[j];
		}
	}
	_efkRenderer->SetCameraMatrix(fkViewMat);
	_efkRenderer->SetProjectionMatrix(fkProjMat);
}

void mEffekseer::EffekseerSetting(DebugCamera* camera, const EFK_CHAR* name1, const EFK_CHAR* name2)
{
	//とりあえずはこのまま
	DXGI_FORMAT bbFormats[] = { DXGI_FORMAT_R8G8B8A8_UNORM };
	_efkRenderer = EffekseerRendererDX12::Create(
		DirectXCommon::GetInstance()->GetDev(),//デバイス
		DirectXCommon::GetInstance()->GetcmdQue(), //コマンドキュー
		2, //バックバッファの数
		bbFormats, //レンダーターゲットフォーマット
		1, //レンダーターゲット数
		DXGI_FORMAT_UNKNOWN, //デプスフォーマット
		false, //反対デプスありか
		1000);//最大パーティクルの数 できれば５００ぐらいがいいかも　１００００だと複数の時重くなりそう

	_efkManager = Effekseer::Manager::Create(1000);//最大インスタンス数

	//左手系に
	_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// 描画用インスタンスから描画機能を設定
	_efkManager->SetSpriteRenderer(_efkRenderer->CreateSpriteRenderer());
	_efkManager->SetRibbonRenderer(_efkRenderer->CreateRibbonRenderer());
	_efkManager->SetRingRenderer(_efkRenderer->CreateRingRenderer());
	_efkManager->SetTrackRenderer(_efkRenderer->CreateTrackRenderer());
	_efkManager->SetModelRenderer(_efkRenderer->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読込
	_efkManager->SetTextureLoader(_efkRenderer->CreateTextureLoader());
	_efkManager->SetModelLoader(_efkRenderer->CreateModelLoader());

	//DX12特有の処理
	_efkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(_efkRenderer->GetGraphicsDevice());
	_efkCmdList = EffekseerRenderer::CreateCommandList(_efkRenderer->GetGraphicsDevice(), _efkMemoryPool);
	_efkRenderer->SetCommandList(_efkCmdList);


	SyncronizeEffekseerCamera( camera);
	// エフェクトの読込
	//_effect = Effekseer::Effect::Create(_efkManager, (const EFK_CHAR*)L"Effect/10/SimpleLaser.efk", 1.0f, (const EFK_CHAR*)L"effect/10");
	_effect = Effekseer::Effect::Create(_efkManager, name1, 1.0f, name2);

	// エフェクトの再生
	//_efkHandle = _efkManager->Play(_effect, 10, 5, 0);

}

void mEffekseer::Load_Effect()//playsceneとかで使えるように
{
	_efkManager->SetSpeed(_efkHandle, 0.5f);
	_efkManager->SetScale(_efkHandle, 5,5,5 );
	_efkHandle = _efkManager->Play(_effect,{ positionx, positiony, positionz
}, 1);
}

void mEffekseer::Stop_Effect()
{
_efkManager->Release();
	//_efkManager->Exists(_efkHandle);
	//_efkManager->StopEffect(_efkHandle);
	//}

	// _efkManager = nullptr;
}


void mEffekseer::EffekseerUpdate( DebugCamera* camera)
{
	//if (Input::GetInstance()->Pushkey(DIK_SPACE)) {
		// エフェクトの再生

	//_efkHandle = _efkManager->Play(_effect, positionx, positiony, positionz);
	_efkManager->SetScale(_efkHandle, scalex, scaley, scalez);
	_efkManager->SetRotation(_efkHandle, rotationx, rotationy, rotationz);
	SyncronizeEffekseerCamera(camera);
	
	auto efkpos = _efkManager->GetLocation(_efkHandle);
	efkpos.X +=0.1f;
	
		_efkManager->SetLocation(_efkHandle, efkpos);

}

void mEffekseer::EffekseerDraw()
{
	//エフェクト描画
	_efkManager->Update();//マネージャの更新
	_efkMemoryPool->NewFrame();//適切なバックバッファを選択
	EffekseerRendererDX12::BeginCommandList(_efkCmdList,DirectXCommon::GetInstance()->GetCmdList());//
	_efkRenderer->BeginRendering();//描画前処理
	_efkManager->Draw();//エフェクト描画
	_efkRenderer->EndRendering();//描画後処理
	EffekseerRendererDX12::EndCommandList(_efkCmdList);
}
