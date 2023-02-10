#include "AttackEffect.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"CustomButton.h"
#include"SelectSword.h"

AttackEffect* AttackEffect::GetIns()
{
	static AttackEffect ins;
	return &ins;
}

void AttackEffect::Init()
{
	Texture::LoadTexture(12, L"Resources/2d/attackEffect/slash_first.png");
	Texture::LoadTexture(13, L"Resources/2d/attackEffect/normal.png");
	Texture::LoadTexture(14, L"Resources/2d/attackEffect/slash_third.png");
	Texture::LoadTexture(15, L"Resources/2d/attackEffect/inpact.png");

	Texture* l_tex[2];// = nullptr;

	if (etype == SLASH_FIRST)
	{

	}
	for (int i = 0; i < Gsize; i++) {
		//攻撃テクスチャ設定
		l_tex[i] = Texture::Create(12, { 0.0f, 0.0f, 1 }, { 0, 0, 1 }, { 1, 1, 1, 1 });
		GuardTex[i].reset(l_tex[i]);
		GuardTex[i]->CreateTexture();
		GuardTex[i]->SetAnchorPoint({ 0.5f,0.5f });
	}

}

void AttackEffect::LoadTex()
{
	texAlpha = 1.0f;
	TexScl.x = 2.0f;

	//攻撃テクスチャ用
	
	Texture* l_tex= Texture::Create(12, { 0.0f, 0.0f, 1 }, { 0, 0, 1 }, { 1, 1, 1, 1 });
	// = nullptr;
	

	AttackTex.reset(l_tex);
	AttackTex->CreateTexture();
	AttackTex->SetAnchorPoint({0.5f, 0.0f});
}

void AttackEffect::SetParticle(XMFLOAT3 pos)
{
	//パーティクル用
	//パーティクルサイズ設定
	ParPos.resize(FIRST);
	ParRot.resize(FIRST);
	ParScl.resize(FIRST);
	ParAlpha.resize(FIRST);
	AttackParticle.resize(FIRST);

	InpactTex.reset(Texture::Create(15, {0.0f, -200.0f, 1}, {1, 1, 1}, {1, 1, 1, 1}));
	InpactTex->CreateTexture();
	InpactTex->SetAnchorPoint({0.5f, 0.5f});

	InpactScl = {3.0f, 3.0f};
	InpactAlpha = 1.0f;
	for (int i = 0; i < FIRST; i++)
	{
		AttackParticle[i].reset(Texture::Create(13, {0.0f, -200.0f, 1}, {1, 1, 1}, {1, 1, 1, 1}));
		AttackParticle[i]->CreateTexture();
		AttackParticle[i]->SetAnchorPoint({0.5f, 0.5f});
	}
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	ParCenterPos = {pos.x - (pos.x - ppos.x), pos.y, pos.z - (pos.z - ppos.z)};

	for (int i = 0; i < FIRST; i++)
	{
		ParPos[i] = {ParCenterPos.x, ParCenterPos.y + 10.0f, ParCenterPos.z};
		ParScl[i] = {3, 2, 2};
		ParRot[i] = {
			0, PlayerControl::GetIns()->GetPlayer()->GetRotation().y + 62.0f, static_cast<float>(rand() % 360)
		};
		ParAlpha[i] = 0.5f;
	}
	eupda = UPDA;
}

void AttackEffect::ParticleUpda()
{
	if (AttackParticle.size() < FIRST)
	{
		return;
	}
	XMVECTOR pmove;
	XMMATRIX pmatRot;

	for (int i = 0; i < AttackParticle.size(); i++)
	{
		if (AttackParticle[i] == nullptr)
		{
			continue;
		}
		pmove = {0.0f, 0.1f, 0.0f, 0.0f};
		pmatRot = XMMatrixRotationZ(XMConvertToRadians(ParRot[i].z));
		pmove = XMVector3TransformNormal(pmove, pmatRot);

		ParPos[i].y += pmove.m128_f32[1] * 1;
		ParPos[i].x += pmove.m128_f32[0] * 1;
		ParAlpha[i] -= 0.008f;
		AttackParticle[i]->SetBillboard(TRUE);
		AttackParticle[i]->SetPosition(ParPos[i]);
		AttackParticle[i]->SetRotation(ParRot[i]);
		AttackParticle[i]->SetScale(ParScl[i]);
		AttackParticle[i]->SetColor({1.0f, 1.0f, 1.0f, ParAlpha[i]});
		AttackParticle[i]->Update(CameraControl::GetIns()->GetCamera());

		if (ParAlpha[i] <= 0.0f)
		{
			AttackParticle[i].reset();
		}
	}
	InpactScl.x += 0.1f;
	InpactScl.y += 0.1f;
	InpactAlpha -= 0.02f;

	InpactTex->SetBillboard(TRUE);
	InpactTex->SetPosition({ParCenterPos.x, ParCenterPos.y + 10.0f, ParCenterPos.z});
	InpactTex->SetColor({1.0f, 1.0f, 1.0f, InpactAlpha});
	InpactTex->SetScale({InpactScl.x, InpactScl.y, 2.0f});
	InpactTex->Update(CameraControl::GetIns()->GetCamera());
}

void AttackEffect::Upda()
{
	XMFLOAT3 ppos = {
		SelectSword::GetIns()->GetSword()->GetMatWorld().r[3].m128_f32[0],
		SelectSword::GetIns()->GetSword()->GetMatWorld().r[3].m128_f32[1],
		SelectSword::GetIns()->GetSword()->GetMatWorld().r[3].m128_f32[2]
	};
	XMVECTOR pmove = PlayerControl::GetIns()->GetPlayer()->GetPlayerMove();
	float protY = PlayerControl::GetIns()->GetPlayer()->GetRotation().y + 62.0f;
	if (Input::GetIns()->TriggerButton(Input::B) && PlayerControl::GetIns()->GetPlayer()->GetAttackType() ==
		PlayerControl::GetIns()->GetPlayer()->FIRST)
	{
		//攻撃の種類
		etype = SLASH_FIRST;
		//テクスチャ読み込み
		LoadTex();
		//画像の各初期値
		TexScl = {2, 0, 0};
		TexPos = {ppos.x + pmove.m128_f32[0] * 10.f, ppos.y + 3.0f, ppos.z + pmove.m128_f32[2] * 10.f};
		TexRot = {0.0f, protY, -135.0f};
	}
	else if (Input::GetIns()->TriggerButton(Input::B) && PlayerControl::GetIns()->GetPlayer()->
		GetAttackType() == PlayerControl::GetIns()->GetPlayer()->SECOND)
	{
		//攻撃の種類
		etype = SLASH_FIRST;
		TexScl = {2, 0, 0};
		TexPos = {ppos.x + pmove.m128_f32[0] * 10.f, ppos.y + 3.0f, ppos.z + pmove.m128_f32[2] * 10.f};
		TexRot = {0.0f, protY, 45.0f};
	}

	if (etype == SLASH_FIRST)
	{
		TexScl.y += 0.1f;
		if (TexScl.y >= 4)
		{
			TexScl.x -= 0.02f;
			texAlpha -= 0.02f;
		}
	}


	if (AttackTex != nullptr)
	{
		AttackTex->SetBillboard(FALSE);
		AttackTex->SetPosition(TexPos);
		AttackTex->SetRotation(TexRot);
		AttackTex->SetScale({TexScl});
		AttackTex->SetColor({1.0f, 1.0f, 1.0f, texAlpha});
		AttackTex->Update(CameraControl::GetIns()->GetCamera());
	}
	ParticleUpda();
	Effect_First();
	TexScl.y = min(TexScl.y, 4.0f);
	TexScl.x = max(TexScl.x, 0.0f);
	texAlpha = max(texAlpha, 0.0f);



}

void AttackEffect::GuarEffect(XMFLOAT3 pos)
{
		switch (gphase)
		{

		case NOGUARD:
			for (int i = 0; i < Gsize; i++)
			{
				GuardAlpha[i] = 1.f;
				GuardSize[i] = { 0.f,0.f,0.f };
			}

		gphase = LARGE;
			break;

		case LARGE:
			GuardSize[0].x += 0.07f;
			GuardSize[0].y += 0.07f;
			GuardAlpha[0] -= 0.01f;

			if (GuardSize[0].x > 1.0f)
			{
				GuardSize[1].x += 0.07f;
				GuardSize[1].y += 0.07f;
				GuardAlpha[1] -= 0.01f;
			}
			if (GuardAlpha[1] < 0.0f)
			{
				gphase = END;
			}
			break;

		case END:
			break;
		}
	
	for (int i = 0; i < Gsize; i++)
	{
		//GuardSize[i] = { 5,5,5 };
		GuardTex[i]->SetBillboard(FALSE);
		GuardTex[i]->SetPosition(pos);
		GuardTex[i]->SetRotation({ defrot.x,defrot.y + 304.f,defrot.z });

		GuardTex[i]->SetScale({ GuardSize[i]});
		GuardTex[i]->SetColor({ 1.0f, 1.0f, 1.0f, GuardAlpha[i]});
		GuardTex[i]->Update(CameraControl::GetIns()->GetCamera());
	}
}


#include"imgui.h"

void AttackEffect::Draw()
{
	Texture::PreDraw();
	for (int i = 0; i < AttackParticle.size(); i++)
	{
		if (AttackParticle[i] == nullptr)
		{
			continue;
		}
		AttackParticle[i]->Draw();
	}

	for(int i=0;i<Gsize;i++)
	{
		if (GuardTex[i] == nullptr)continue;
		GuardTex[i]->Draw();
	}
	if (InpactTex != nullptr)
	{
		InpactTex->Draw();
	}
	Texture::PostDraw();


	/*ImGui::Begin("slashpar");


	ImGui::Text("ParSize %f", InpactScl.x);
	ImGui::Text("parPos.size %f", InpactAlpha);
	ImGui::End();*/
}

void AttackEffect::Effect_First()
{
	if (eupda == LOAD)
	{
		//	LoadTex();
	}
	else if (eupda == UPDA)
	{
	}
}

void AttackEffect::Effect_Second()
{
}

void AttackEffect::Effect_Third()
{
}

void AttackEffect::Effect_Particle()
{
}

void AttackEffect::Effect_Clear()
{
	AttackParticle.clear();
}
