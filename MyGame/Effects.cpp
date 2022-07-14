#include "Effects.h"

void Effects::Initialize( DebugCamera* camera)
{
	//エフェクトのインスタンス生成
	efk = new mEffekseer();
	efk1 = new mEffekseer();

	//エフェクトのセット(3引き数に)
	efk->EffekseerSetting(camera, (const EFK_CHAR*)L"Effect/10/thunder.efk", (const EFK_CHAR*)L"effect/10");
	efk1->EffekseerSetting(camera, (const EFK_CHAR*)L"Effect/10/SimpleLaser.efk", (const EFK_CHAR*)L"effect/10");
}

void Effects::Update(DebugCamera* camera)
{
	//エフェクトのパラメータセット
	efk->SetPosition(Effect_Pos.x, Effect_Pos.y, Effect_Pos.z);
	efk->SetRotation(0, 0, 0);
	efk->SetScale(1, 1, 1);
	efk1->SetPosition(-10, 0, 190);

	//エフェクトの再生
	//if (Input::GetInstance()->Pushkey(DIK_SPACE)) {
	efk->Load_Effect();
	//}
	efk1->Load_Effect();

	//エフェクトのアニメーション止める
	if (Input::GetInstance()->Pushkey(DIK_C)) {
		efk->Stop_Effect();
	}

	//view,projection行列をエフェクトのテクスチャにかける
	efk->EffekseerUpdate( camera);
	efk1->EffekseerUpdate( camera);

}

void Effects::Draw()
{
	//エフェクトの画像
	efk->EffekseerDraw();
	efk1->EffekseerDraw();
}