#include "GuardianBomAttack.h"
#include"CameraControl.h"
#include"EnemyControl.h"
void GuardianBomAttack::TexSet()
{
	Texture::LoadTexture(70, L"Resources/2d/enemy/guarbom.png");

	Texture* l_tex = Texture::Create(70);
	DamageTex.reset(l_tex);
	DamageTex->CreateTexture();
	DamageTex->SetAnchorPoint({ 0.5f,0.5f });
	TexAlpha = 0.0f;
	TexScl = { 0.0f,0.f };
}

void GuardianBomAttack::Upda()
{
	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();
	DamageTex->SetScale({ TexScl.x,TexScl.y,1.f });
	DamageTex->SetPosition({ epos.x, -30, epos.z });
	DamageTex->Update(CameraControl::GetInstance()->GetCamera());

	TexAlpha = max(TexAlpha, 0.f);
}

void GuardianBomAttack::Phase_AreaSet()
{
	TexScl.x += 0.05f;
	TexScl.y += 0.05f;
	TexAlpha += 0.02f;
	if(TexAlpha>2.0f)
	{
		phase = BOM;
	}
	TexScl.x = min(TexScl.x, 3.0f);
	TexScl.y = min(TexScl.y, 3.0f);

}

void GuardianBomAttack::Phase_Bom()
{
	
}

void GuardianBomAttack::Phase_End()
{
	TexAlpha -= 0.05f;
}

void GuardianBomAttack::Draw()
{
	Texture::PreDraw();
	DamageTex->Draw();
	Texture::PostDraw();
}


