#include "KnockAttack.h"
#include"EnemyControl.h"
#include"CameraControl.h"

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
	KnockTex->SetAnchorPoint({ 0.5,0.5 });

}

void KnockAttack::ActionJudg()
{
	if (CameraControl::GetInstance()->GetCamera() != nullptr) {
		KnockTex->SetUVMove(true);
		KnockTex->Update(CameraControl::GetInstance()->GetCamera());
		KnockTex->SetRotation({ 70, 0,0
			});
		KnockTex->SetScale({ 5,5,3 });
	
	}
	if (EnemyControl::GetInstance()->GetTutorialEnemyindex()[0] != nullptr) {
		KnockTex->SetPosition({ EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().x
			,EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().y+5,
			EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().z});
	}
}
void KnockAttack::Draw()
{
	Texture::PreDraw();
	KnockTex->Draw();
	Texture::PostDraw();
}