#include "AttackEffect.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"CustomButton.h"
#include"SelectSword.h"
AttackEffect* AttackEffect::GetIns()
{
	static AttackEffect ins;
	return&ins;
}
void AttackEffect::Init()
{
	Texture::LoadTexture(12, L"Resources/2d/attackEffect/slash_first.png");
	Texture::LoadTexture(13, L"Resources/2d/attackEffect/slash_second.png");
	Texture::LoadTexture(14, L"Resources/2d/attackEffect/slash_third.png");
	Texture::LoadTexture(15, L"Resources/2d/attackEffect/particle.png");
	
	
}

void AttackEffect::LoadTex()
{
	texAlpha = 1.0f;
	TexScl.x = 2.0f;

	//攻撃テクスチャ用
	Texture* l_tex = nullptr;

	if (etype == SLASH_FIRST) {
		//攻撃テクスチャ設定
		l_tex = Texture::Create(12, { 0.0f,0.0f,1 }, { 0,0,1 }, { 1,1,1,1 });
	} 
	else if (etype == SLASH_SECOND) {
		//攻撃テクスチャ設定
		l_tex = Texture::Create(13, { 0.0f,0.0f,1 }, { 1,1,1 }, { 1,1,1,1 });
	}
	else if (etype == SLASH_THIRD) {
		//攻撃テクスチャ設定
		l_tex = Texture::Create(14, { 0.0f,-200.0f,1 }, { 1,1,1 }, { 1,1,1,1 });
	}

	
	AttackTex.reset(l_tex);
	AttackTex->CreateTexture();
	AttackTex->SetAnchorPoint({ 0.5f, 0.0f });
}

void AttackEffect::SetParticle(XMFLOAT3 pos)
{
	//パーティクル用
	//パーティクルサイズ設定
	ParPos.resize(ParticleSize::FIRST);
	ParRot.resize(ParticleSize::FIRST);
	ParScl.resize(ParticleSize::FIRST);

	AttackParticle.resize(ParticleSize::FIRST);

	for (int i = 0; i < ParticleSize::FIRST; i++) {
		AttackParticle[i].reset(Texture::Create(15, { 0.0f,-200.0f,1 }, { 1,1,1 }, { 1,1,1,1 }));
		AttackParticle[i]->CreateTexture();
		AttackParticle[i]->SetAnchorPoint({ 0.5f,0.5f });
	}
	ParCenterPos = pos;
	
		for (int i = 0; i < ParticleSize::FIRST; i++) {
			ParPos[i] = ParCenterPos;
			ParScl[i] = { 2,2,2 };
			ParRot[i] = { 0,0,(float)i * 18 };
		}
		eupda = UPDA;
}

void AttackEffect::ParticleUpda()
{
	if (AttackParticle.size()< ParticleSize::FIRST)return;
	std::vector<XMVECTOR> move;
	std::vector<XMMATRIX> matRot;
	move.resize(ParticleSize::FIRST);
	matRot.resize(ParticleSize::FIRST);

	for (int i = 0; i < AttackParticle.size(); i++) {
		move[i] = { 0.0f,0.1f,0.0f,0.0f };
		matRot[i] = XMMatrixRotationZ(XMConvertToRadians(ParRot[i].z));
		move[i] = XMVector3TransformNormal(move[i], matRot[i]);

		ParPos[i].y += move[i].m128_f32[1] * 1;
		ParPos[i].x += move[i].m128_f32[0] * 1;

		AttackParticle[i]->SetBillboard(TRUE);
		AttackParticle[i]->SetPosition(ParPos[i]);
		AttackParticle[i]->SetRotation(ParRot[i]);
		AttackParticle[i]->SetScale(ParScl[i]);
		AttackParticle[i]->Update(CameraControl::GetInstance()->GetCamera());
	}
}

void AttackEffect::Upda()
{
	XMFLOAT3 ppos = { SelectSword::GetInstance()->GetSword()->GetMatWorld().r[3].m128_f32[0],SelectSword::GetInstance()->GetSword()->GetMatWorld().r[3].m128_f32[1],SelectSword::GetInstance()->GetSword()->GetMatWorld().r[3].m128_f32[2] };
	XMVECTOR pmove = PlayerControl::GetInstance()->GetPlayer()->GetPlayerMove();
	float protY = PlayerControl::GetInstance()->GetPlayer()->GetRotation().y + 62.0f;
	if (CustomButton::GetInstance()->GetAttackAction()) {
		//攻撃の種類
		etype = SLASH_FIRST;
		//テクスチャ読み込み
		LoadTex();
		//画像の各初期値
		TexScl = { 2,0,0 };
		TexPos={ ppos.x + pmove.m128_f32[0] * 10.f,ppos.y+3.0f ,ppos.z + pmove.m128_f32[2] * 10.f };
		TexRot={ 0.0f,protY, -135.0f };
	}
	

	if (etype == SLASH_FIRST) {
		TexScl.y += 0.1f;
		if (TexScl.y >= 4) {
			TexScl.x -= 0.02f;
			texAlpha -= 0.02f;
		}
	}

	
	if (AttackTex != nullptr) {
		AttackTex->SetBillboard(FALSE);
		AttackTex->SetPosition(TexPos);
		AttackTex->SetRotation(TexRot);
		AttackTex->SetScale({ TexScl});
		AttackTex->SetColor({ 1.0f,1.0f,1.0f,texAlpha });
		AttackTex->Update(CameraControl::GetInstance()->GetCamera());
	}
	ParticleUpda();
	Effect_First();
	TexScl.y = min(TexScl.y, 4.0f);
	TexScl.x = max(TexScl.x, 0.0f);
	texAlpha = max(texAlpha, 0.0f);
}

#include"imgui.h"
void AttackEffect::Draw()
{
	
	Texture::PreDraw();
	for (int i = 0; i < AttackParticle.size(); i++) {
		if (AttackParticle[i] == nullptr)continue;
		AttackParticle[i]->Draw();
	}
	if (AttackTex != nullptr) {
		AttackTex->Draw();
	}
	Texture::PostDraw();
		

	ImGui::Begin("slashpar");

	if (ParRot.size() > 9) {
	for (int i = 0; i < 10; i++) {
			ImGui::Text("%f", ParRot[i].z);
		}
	}
	ImGui::Text("ParSize %d", AttackParticle.size());
	ImGui::Text("parPos.size %d", ParPos.size());
	ImGui::End();
}

void AttackEffect::Effect_First()
{
	if (eupda == EffectUpda::LOAD) {
	//	LoadTex();
	}
	else if (eupda == EffectUpda::UPDA) {
	
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

