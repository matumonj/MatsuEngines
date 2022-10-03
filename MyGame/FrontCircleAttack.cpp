#include "FrontCircleAttack.h"
#include"EnemyControl.h"
#include"CameraControl.h"
#include"BossSpell.h"
FrontCircleAttack* FrontCircleAttack::GetInstance()
{
	static FrontCircleAttack instance;

	return &instance;
}

void FrontCircleAttack::Initialize()
{
	Texture::LoadTexture(26, L"Resources/AOE.png");
		DamageAreaTex = Texture::Create(26, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
		DamageAreaTex->CreateTexture();
		DamageAreaTex->SetAnchorPoint({ 0.5f,0.5f });
	
	fase = FASENON;
}

void FrontCircleAttack::ActionJudg()
{
	//CameraControl::GetInstance()->ShakeCamera();
	DamageAreaTex->SetRotation({ 90,0,45 });
	DamageAreaTex->SetRotation({ 90,0,-45 });
	DamageAreaTex->SetScale({ 8,8,3 });
	
	if (Input::GetInstance()->TriggerButton(Input::Button_X)) {
		fase = FASEONE;
	}
	
	//フェーズごとの処理
	switch (fase)
	{
	case FASENON:
		break;
	case FASEONE:
		BossSpell::GetInstance()->SetStartSpell_FC(true);
		if (BossSpell::GetInstance()->GetEndSpell_FC()) {
			fase = FASETWO;
		}
		//fase = FASETWO;
		break;
	case FASETWO:
		break;
	case FASETHREE:
		
		break;
	case FASEFOUR:
		
		BossSpell::GetInstance()->SetEndSpell_UA(false);

		break;
	default:
		break;
	}

		DamageAreaTex->SetBillboard(false);
		DamageAreaTex->SetColor({ 1.0f ,1.0f ,1.0f ,0.9f });
		DamageAreaTex->Update(CameraControl::GetInstance()->GetCamera());
		DamageAreaTex->SetPosition({ 0.0f ,-18.0f ,30.0f });
		
}

void FrontCircleAttack::Draw()
{
	Texture::PreDraw();
	DamageAreaTex->Draw();
	Texture::PostDraw();

}


void FrontCircleAttack::Finalize()
{
	Destroy(DamageAreaTex);
}