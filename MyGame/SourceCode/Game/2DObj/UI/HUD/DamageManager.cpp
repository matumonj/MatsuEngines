#include "DamageManager.h"
#include"CameraControl.h"
#include <algorithm>

#include "PlayerAttackState.h"

DamageManager::DamageManager(XMFLOAT3 Position, int Damage)
{
	this->Position = Position;
	this->Damage = Damage;
	DamageTex = std::make_unique<DebugTextSprite>();
	DamageTex->Initialize(0);
	TexAlpha = 1.0f;
	TexSize = {static_cast<float>(Damage) / 10.0f, static_cast<float>(Damage) / 10.f};
}

DamageManager::~DamageManager()
{
	DamageTex.reset(nullptr);
}

void DamageManager::DamageDisPlay(int damage, XMFLOAT4 color)
{
	//テクスチャのアルファ値だのサイズだの
	TexSize.x -= 0.04f;
	TexSize.y -= 0.04f;
	if (TexSize.x < 0.9f)
	{
		TexAlpha -= 0.006f;
	}
	//int->stringに
	std::ostringstream str;
	str << std::fixed << std::setprecision(2)
		<< Damage;
	//3D->2D変換 3引数消す
	XMVECTOR tex2DPos = {Position.x, Position.y + 5.f, Position.z};
	tex2DPos = PosDivi(tex2DPos, CameraControl::GetIns()->GetCamera()->GetViewMatrix(), false);
	tex2DPos = PosDivi(tex2DPos, CameraControl::GetIns()->GetCamera()->GetProjectionMatrix(), true);
	tex2DPos = WDivi(tex2DPos, false);
	tex2DPos = PosDivi(tex2DPos, CameraControl::GetIns()->GetCamera()->GetViewPort(), false);

	if (Damage >= 40.f)
	{
		DamageTex->SetColor({1.f, 0.2f, 0.2f});
	}
	else if (Damage < 40 && Damage >= 35.f)
	{
		DamageTex->SetColor({1.f, 0.5f, 0.5f});
	}
	else if (Damage < 35 && Damage >= 30.f)
	{
		DamageTex->SetColor({1.f, 0.6f, 0.6f});
	}
	else if (Damage < 30 && Damage >= 0.f)
	{
		DamageTex->SetColor({1.f, 0.7f, 0.7f});
	}
	DamageTex->SetAlpha(TexAlpha);

	DamageTex->Print(str.str(), tex2DPos.m128_f32[0], tex2DPos.m128_f32[1], TexSize.x);


	//TexSize.x = min(TexSize.x, 1.4f);
	TexSize.x = max(TexSize.x, 0.8f);
}

void DamageManager::DamageDisPlay_Green(int damage, XMFLOAT4 color)
{
	//テクスチャのアルファ値だのサイズだの
	TexSize.x -= 0.04f;
	TexSize.y -= 0.04f;
	if (TexSize.x < 0.9f)
	{
		TexAlpha -= 0.01f;
	}
	//int->stringに
	std::ostringstream str;
	str << std::fixed << std::setprecision(2)
		<< Damage;
	//3D->2D変換 3引数消す
	XMVECTOR tex2DPos = {Position.x, Position.y + 5.f, Position.z};
	tex2DPos = PosDivi(tex2DPos, CameraControl::GetIns()->GetCamera()->GetViewMatrix(), false);
	tex2DPos = PosDivi(tex2DPos, CameraControl::GetIns()->GetCamera()->GetProjectionMatrix(), true);
	tex2DPos = WDivi(tex2DPos, false);
	tex2DPos = PosDivi(tex2DPos, CameraControl::GetIns()->GetCamera()->GetViewPort(), false);

	DamageTex->SetColor({10.f, 10.f, 10.f});
	DamageTex->SetAlpha(TexAlpha);
	//表記
	DamageTex->Print(str.str(), tex2DPos.m128_f32[0], tex2DPos.m128_f32[1], TexSize.x);


	//TexSize.x = min(TexSize.x, 1.4f);
	TexSize.x = max(TexSize.x, 0.8f);
}

void DamageManager::Draw()
{
	if (DamageTex == nullptr)
	{
		return;
	}
	Sprite::PreDraw();
	DamageTex->DrawAll();
	Sprite::PostDraw();
}

XMVECTOR DamageManager::WDivi(const DirectX::XMVECTOR& pos, const DirectX::XMMATRIX& mat, const bool s)
{
	float x, y, z, w;

	x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat.r[
		2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
	y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat.r[
		2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
	z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat.r[
		2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

	w = pos.m128_f32[3];
	if (s)
	{
		w = z;
	}

	x = x / w;
	y = y / w;
	z = z / w;

	return XMVECTOR{x, y, z, w};
}

XMVECTOR DamageManager::WDivi(const DirectX::XMVECTOR& pos, const bool s)
{
	float x, y, z, w;
	x = pos.m128_f32[0];
	y = pos.m128_f32[1];
	z = pos.m128_f32[2];
	w = pos.m128_f32[3];

	if (s)
	{
		w = z;
	}

	x = x / w;
	y = y / w;
	z = z / w;

	return XMVECTOR{x, y, z, w};
}

XMVECTOR DamageManager::PosDivi(const DirectX::XMVECTOR& pos, const DirectX::XMMATRIX& mat, const bool s)
{
	float x, y, z, w;

	x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat.r[
		2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
	y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat.r[
		2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
	z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat.r[
		2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

	w = 1.0f;

	if (s)
	{
		w = z;
	}

	return XMVECTOR{x, y, z, w};
}
