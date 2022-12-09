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
	DamageAreaTex = Texture::Create(26, {0.0f, 0.0f, 0.0f}, {100.0f, 100.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
	DamageAreaTex->CreateTexture();
	DamageAreaTex->SetAnchorPoint({0.5f, 0.5f});

	phase = PHASENON;
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
		phase = PHASEONE;
	}

	//フェーズごとの処理
	switch (phase)
	{
	case PHASENON:
		TexAlpha = 0;
		break;
	case PHASEONE:
		TexAlpha = 1.0f;
		BossSpell::GetInstance()->SetStartSpell(BossSpell::SLAM, true);
		if (BossSpell::GetInstance()->GetEndSpell(BossSpell::SLAM))
		{
			phase = PHASETWO;
		}
	//phase = PHASETWO;
		break;
	case PHASETWO:
		phase = PHASETHREE;
		break;
	case PHASETHREE:


		TexAlpha -= 0.02f;
		if (TexAlpha <= 0.0f)
		{
			phase = PHASEFOUR;
		}

		break;
	case PHASEFOUR:
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
