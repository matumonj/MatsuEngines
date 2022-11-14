#include "FrontCircleAttack.h"
#include"EnemyControl.h"
#include"CameraControl.h"
#include"BossSpell.h"
#include"GigaBossEnemy.h"

FrontCircleAttack* FrontCircleAttack::GetInstance()
{
	static FrontCircleAttack instance;

	return &instance;
}

void FrontCircleAttack::Initialize()
{
	Texture::LoadTexture(26, L"Resources/AOE.png");
	DamageAreaTex = Texture::Create(26, {0.0f, 0.0f, 0.0f}, {100.0f, 100.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
	DamageAreaTex->CreateTexture();
	DamageAreaTex->SetAnchorPoint({0.5f, 0.5f});

	fase = FASENON;
	TexAlpha = 0.0f;
}

void FrontCircleAttack::ActionJudg()
{
	//CameraControl::GetInstance()->ShakeCamera();
	DamageAreaTex->SetRotation({90, 0, 45});
	DamageAreaTex->SetRotation({90, 0, -45});
	DamageAreaTex->SetScale({8, 8, 3});

	if (Input::GetInstance()->TriggerButton(Input::X))
	{
		fase = FASEONE;
	}

	//フェーズごとの処理
	switch (fase)
	{
	case FASENON:
		TexAlpha = 0;
		break;
	case FASEONE:
		TexAlpha = 1.0f;
		BossSpell::GetInstance()->SetStartSpell(BossSpell::SLAM, true);
		if (BossSpell::GetInstance()->GetEndSpell(BossSpell::SLAM))
		{
			fase = FASETWO;
		}
	//fase = FASETWO;
		break;
	case FASETWO:
		EnemyControl::GetInstance()->GetGigaBossEnemy()->SetMotion(
			EnemyControl::GetInstance()->GetGigaBossEnemy()->SLAM);
		fase = FASETHREE;
		break;
	case FASETHREE:
		if (EnemyControl::GetInstance()->GetGigaBossEnemy()->EndSlamMotion())
		{
			TexAlpha -= 0.02f;
			if (TexAlpha <= 0.0f)
			{
				fase = FASEFOUR;
			}
		}
		break;
	case FASEFOUR:
		BossSpell::GetInstance()->SetEndSpell(BossSpell::SLAM, false);
		break;
	default:
		break;
	}

	DamageAreaTex->SetBillboard(false);
	DamageAreaTex->SetColor({1.0f, 1.0f, 1.0f, TexAlpha});
	DamageAreaTex->Update(CameraControl::GetInstance()->GetCamera());
	DamageAreaTex->SetPosition({0.0f, -18.0f, 30.0f});

	TexAlpha = min(TexAlpha, 1.0f);
	TexAlpha = max(TexAlpha, 0.0f);
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
