#include "Effects.h"
#include"Player.h"
#include"PlayerAttackState.h"
#include"TargetMarker.h"
#include"Collision.h"
#include"EnemyControl.h"
Effects* Effects::GetInstance()
{
	static Effects instance;

	return &instance;
}
void Effects::Initialize(DebugCamera* camera)
{
	//エフェクトのインスタンス生成
	efk = new mEffekseer();
	//efk1 = new mEffekseer();

	//エフェクトのセット(3引き数に)
	efk->EffekseerSetting(camera, (const EFK_CHAR*)L"Effect/10/deadef.efk", (const EFK_CHAR*)L"effect/10");
	//efk1->EffekseerSetting(camera, (const EFK_CHAR*)L"Effect/10/SimpleLaser.efk", (const EFK_CHAR*)L"effect/10");
}

void Effects::Update(DebugCamera* camera)
{
	//エフェクトのパラメータセット
	int index = TargetMarker::GetInstance()->GetNearIndex();
	float posx = EnemyControl::GetInstance()->GetEnemyindex(1)[index]->GetPosition().x;
		float posy= EnemyControl::GetInstance()->GetEnemyindex(1)[index]->GetPosition().y;
		float posz= EnemyControl::GetInstance()->GetEnemyindex(1)[index]->GetPosition().z;
	efk->SetPosition(posx,posy,posz);
	efk->SetRotation(0, 0, 0);
	efk->SetScale(0.001, 0.001, 0.001);
	//efk1->SetPosition(-10, 0, 190);
	//if (Input::GetInstance()->PushButton(Input::GetInstance()->Button_RB)) {
		
	//エフェクトの再生
	//if (Input::GetInstance()->Pushkey(DIK_SPACE)) {
	
	if (Collision::GetLength(Player::GetInstance()->GetPosition(), EnemyControl::GetInstance()->GetEnemyindex(1)[index]->GetPosition() )<25&& Input::GetInstance()->PushButton(Input::GetInstance()->Button_B)) {
		ff = true;
	}
	if (PlayerAttackState::GetInstance()->GetNowSkill() == PlayerAttackState::GetInstance()->Third) {
		tf = true;
	}
	
	if(ff){
	efk->Load_Effect();
	ff = false;
	}
	//efk1->Load_Effect();

	//エフェクトのアニメーション止める
	if (Input::GetInstance()->Pushkey(DIK_C)) {
		efk->Stop_Effect();
	}

	//view,projection行列をエフェクトのテクスチャにかける
	efk->EffekseerUpdate(camera);
	//efk1->EffekseerUpdate(camera);

}

void Effects::Draw()
{
	//エフェクトの画像
	efk->EffekseerDraw();
	//efk1->EffekseerDraw();
}