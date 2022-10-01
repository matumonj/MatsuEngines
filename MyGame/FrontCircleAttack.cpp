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
	

		FissureTex = Texture::Create(23, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
		FissureTex->CreateTexture();
		FissureTex->SetAnchorPoint({ 0.5f,0.5f });

	fase = FASENON;
}

void FrontCircleAttack::ActionJudg()
{
	//CameraControl::GetInstance()->ShakeCamera();
	DamageAreaTex->SetRotation({ 90,0,45 });
	DamageAreaTex->SetRotation({ 90,0,-45 });
	DamageAreaTex->SetScale({ 6,13,3 });
	DamageAreaTex->SetScale({ 6,13,3 });

	FissureTex->SetRotation({ 90,0,0 });
	FissureTex->SetScale({ 8,8,3 });
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
		fissureAlpha = 0.8f;
		//fase = FASETWO;
		break;
	case FASETWO:
		SlamAttack();
		break;
	case FASETHREE:
		
		break;
	case FASEFOUR:
		
		BossSpell::GetInstance()->SetEndSpell_UA(false);

		break;
	default:
		break;
	}

		DamageAreaTex->SetUVMove(true);
		DamageAreaTex->SetBillboard(false);
		DamageAreaTex->SetColor({ 1.0f ,1.0f ,1.0f ,0.6f });
		DamageAreaTex->Update(CameraControl::GetInstance()->GetCamera());
		DamageAreaTex->SetPosition({ 0.0f ,-18.0f ,30.0f });
		
		FissureTex->SetBillboard(false);
		FissureTex->SetColor({ 1.0f ,1.0f ,1.0f ,fissureAlpha });
		FissureTex->SetPosition({ 0.0f ,-18.0f ,30.0f });
		FissureTex->SetDisplayRadius(disrad);

		FissureTex->Update(CameraControl::GetInstance()->GetCamera());
	

		fissureAlpha = min(fissureAlpha, 1.0f);
		fissureAlpha = max(fissureAlpha, 0.0f);

		disrad = min(disrad, 60.0f);
		disrad = max(disrad, 0.0f);
}

void FrontCircleAttack::Draw()
{
	Texture::PreDraw();
	DamageAreaTex->Draw();
	FissureTex->Draw();
	Texture::PostDraw();

}

void FrontCircleAttack::PowerAccumulate()
{
	fissureAlpha = 0.8f;
}

void FrontCircleAttack::SlamAttack()
{
	disrad += 0.5f;
	
	if (disrad >= 70.0f) {
		fissureAlpha -= 0.05f;
	}

}

void FrontCircleAttack::Finalize()
{
	Destroy(FissureTex);
	Destroy(DamageAreaTex);

}