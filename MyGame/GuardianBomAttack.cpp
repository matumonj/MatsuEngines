#include "GuardianBomAttack.h"
#include"CameraControl.h"
#include"EnemyControl.h"

GuardianBomAttack* GuardianBomAttack::GetIns()
{
	static GuardianBomAttack ins;
	return &ins;
}

void GuardianBomAttack::TexSet()
{
	Texture::LoadTexture(70, L"Resources/2d/icon/enemyicon.png");

	Texture* l_tex = Texture::Create(70);
	DamageTex.reset(l_tex);
	DamageTex->CreateTexture();
	DamageTex->SetAnchorPoint({0.5f, 0.5f});
	TexAlpha = 0.0f;
	TexScl = {0.0f, 0.f};

	BomEffect = std::make_unique<Particle>();
	BomEffect->Init(64);
}

void GuardianBomAttack::Upda()
{
	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();

	switch (phase)
	{
	case NON:
		TexScl = {0.f, 0.f};
		TexAlpha = 0.0f;
		ColorT = 0.0f;
		break;

	case AREASET:
		Phase_AreaSet();
		break;

	case BOM:
		Phase_Bom();
		break;

	case END:
		Phase_End();
		break;
	}
	DamageTex->SetScale({TexScl.x, TexScl.y, 1.f});
	DamageTex->SetPosition({epos.x, -32.f, epos.z});
	DamageTex->SetRotation({90.f, 0.f, 0.f });
	DamageTex->SetColor({1.f, 1.f, 1.f, 1.f });
	DamageTex->SetBillboard(false);
	DamageTex->Update(CameraControl::GetInstance()->GetCamera());

	BomEffect->CreateParticle(true, {epos.x, epos.y, epos.z});
	BomEffect->Upda_B();
	TexAlpha = max(TexAlpha, 0.f);
}

void GuardianBomAttack::Phase_AreaSet()
{
	TexScl.x += 0.05f;
	TexScl.y += 0.05f;
	TexAlpha += 0.02f;

	if (TexAlpha > 20.0f)
	{
		phase = BOM;
	}
	ColorT++;

	BossColor.x = 1.f;
	BossColor.y = -1.f + sinf(3.14f * 2.f / 120.f * ColorT) * 2.f,
		BossColor.z = -1.f + sinf(3.14f * 2.f / 120.f * ColorT) * 2.f,


		TexScl.x = min(TexScl.x, 3.0f);
	TexScl.y = min(TexScl.y, 3.0f);

	EnemyControl::GetInstance()->GetGuardianEnemy()->SetColor({BossColor.x, BossColor.y, BossColor.z, 1.0f});
}

void GuardianBomAttack::Phase_Bom()
{
	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();

	phase = END;
}

void GuardianBomAttack::Phase_End()
{
	EnemyControl::GetInstance()->GetGuardianEnemy()->SetColor({1.0f, 1.0f, 1.0f, 1.0f});

	TexAlpha -= 0.05f;
}

void GuardianBomAttack::Draw()
{
	Texture::PreDraw();
	DamageTex->Draw();
	Texture::PostDraw();

	BomEffect->Draw();
}
