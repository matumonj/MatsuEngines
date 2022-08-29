#include "KnockAttack.h"
#include"EnemyControl.h"
#include"CameraControl.h"
#include"PlayerControl.h"
#include"mHelper.h"
KnockAttack* KnockAttack::GetInstance()
{
	static KnockAttack instance;
	return &instance;
}
void KnockAttack::Initialize()
{
	Texture::LoadTexture(20, L"Resources/Knock.png");

	KnockTex = Texture::Create(20, {0,0,0}, { 100,100,1 }, { 1,1,1,1 });
	KnockTex->CreateTexture();
	KnockTex->SetAnchorPoint({0.5,0.5 });

}

void KnockAttack::ActionJudg()
{
	if (Input::GetInstance()->TriggerButton(Input::GetInstance()->Button_B)) {
		fase1 = true;
	}
	if (fase1) {
		AttackCount++;//攻撃待機カウンタ
		if (AttackCount >= 120) {//2秒立ったら
			fase2 = true;//攻撃フェーズsrart
			fase1 = false;//待機フェーズend
		}
		//吹き飛ばし直前に吹き飛ばし後のプレイヤーz座標を設定
		AfterPositionZ = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z - 50;
		BeforePositionZ = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z;

		KnockTime = 0;//イージング用カウンタリセット
	}
	
	if (fase2) {
		if (KnockTime >= 1.0f) {
			fase2 = false;
		}
		KnockTime += 0.04f;//イージング用カウンタ
		AttackCount = 0;//攻撃待機時間リセット
		PlayerControl::GetInstance()->GetPlayer()->SetPosition({
				PlayerControl::GetInstance()->GetPlayer()->GetPosition().x ,
				PlayerControl::GetInstance()->GetPlayer()->GetPosition().y,
					Easing::EaseOut(KnockTime, BeforePositionZ, AfterPositionZ)
			});
	}
	if (CameraControl::GetInstance()->GetCamera() != nullptr) {
		KnockTex->SetUVMove(true);
		KnockTex->SetBillboard(false);

		KnockTex->Update(CameraControl::GetInstance()->GetCamera());
	}
	if (EnemyControl::GetInstance()->GetTutorialEnemyindex()[0] != nullptr) {
		KnockTex->SetPosition({ EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().x
			,EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().y+5,
			EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().z});

		KnockTex->SetRotation({ 90, 0,0 });
		KnockTex->SetScale({ 5,5,3 });
	}
	AttackCount = min(AttackCount, 180);
	AttackCount = max(AttackCount, 0);

	TexAlpha = min(TexAlpha, 1.0f);
	TexAlpha = max(TexAlpha, 0.0f);
}
void KnockAttack::Draw()
{
	Texture::PreDraw();
	KnockTex->Draw();
	Texture::PostDraw();
}