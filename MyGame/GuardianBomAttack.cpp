#include "GuardianBomAttack.h"
#include"CameraControl.h"
#include"EnemyControl.h"
#include "PlayerControl.h"
#define PI 3.14f
GuardianBomAttack* GuardianBomAttack::GetIns()
{
	static GuardianBomAttack ins;
	return &ins;
}

void GuardianBomAttack::TexSet()
{
	Texture::LoadTexture(70, L"Resources/2d/icon/enemyicon.png");
	Texture::LoadTexture(66, L"Resources/2d/attackEffect/missileeffect.png");

	//テクスチャセット
	Texture* l_tex = Texture::Create(70);
	DamageTex.reset(l_tex);
	DamageTex->CreateTexture();
	DamageTex->SetAnchorPoint({0.5f, 0.5f});

	//初期化
	TexAlpha = 0.0f;
	TexScl = {0.0f, 0.f};

	BomEffect = std::make_unique<Particle>();
	BomEffect->Init(66);
	BomEffect->SetParF(1);
	BomEffect->SetParScl({ 5.f,5.f });
	BomEffect->SetParColor({ 1.f,1.f,1.f,0.7f });
}

void GuardianBomAttack::Upda()
{
	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();
	XMFLOAT3 ppos =PlayerControl::GetInstance()->GetPlayer()->GetPosition();

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

	//各パラメータのセット
	DamageTex->SetScale({TexScl.x, TexScl.y, 1.f});
	DamageTex->SetPosition({epos.x, -32.f, epos.z});
	DamageTex->SetRotation({90.f, 0.f, 0.f });
	DamageTex->SetColor({1.f, 1.f, 1.f, 1.f });
	DamageTex->SetBillboard(false);
	DamageTex->Update(CameraControl::GetInstance()->GetCamera());


	BomEffect->CreateParticle(phase==Phase::BOM, {epos.x-(epos.x-ppos.x)/2.f, -32.f, epos.z - (epos.x - ppos.x) / 2.f });
	BomEffect->Upda_B();
	TexAlpha = max(TexAlpha, 0.f);
}

void GuardianBomAttack::Phase_AreaSet()
{
	bool nextPhase = TexAlpha > 3.f;
	const float scalingSpeed = 0.05f;
	const float maxScale = 8.f;

	//テクスチャ拡大
	TexScl.x +=scalingSpeed;
	TexScl.y += scalingSpeed;
	TexAlpha += 0.02f;

	//色の明度下げたり上げたり
	ColorT++;

	BossColor.x = 1.f;
	BossColor.y = -1.f + sinf(PI * 2.f / 120.f * ColorT) * 2.f,
	BossColor.z = -1.f + sinf(PI * 2.f / 120.f * ColorT) * 2.f,


	TexScl.x = min(TexScl.x,maxScale);
	TexScl.y = min(TexScl.y, maxScale);

	if (nextPhase)
	{
		phase = BOM;
	}

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
