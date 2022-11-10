#include "DamageManager.h"
#include"CameraControl.h"
DamageManager* DamageManager::GetIns() {
	static DamageManager instans;
	return &instans;
}

void DamageManager::Init() {
	
}
void DamageManager::Upda()
{
		for (std::unique_ptr<DebugTxt>& dtex : DamageTex) {
			if (dtex != nullptr) {
				
				dtex->Print(2);
			}
		}

	//	DamageTex.remove_if([](std::unique_ptr<DebugTxt>& dtex) {
		//	return dtex->GetisDestroy();
		//});
}

void DamageManager::TextBeha()
{
}
void DamageManager::Draw() 
{
	for (std::unique_ptr<DebugTxt>& dtex : DamageTex) {
		if (dtex!= nullptr) {
			Texture::PreDraw();
			dtex->DrawAll();
			Texture::PostDraw();
		}
	}
}
void DamageManager::DamageDisPlay(int damage, XMFLOAT4 color, XMFLOAT3 Position)
{
	TexSize.x += 0.02f;
	TexSize.y += 0.02f;
	TexAlpha -= 0.01f;
		std::ostringstream str;

		str << std::fixed << std::setprecision(2)
			<< damage;
		XMVECTOR tex2DPos = {Position.x,Position.y + 10,Position.z};
		tex2DPos = PosDivi(tex2DPos, CameraControl::GetInstance()->GetCamera()->GetViewMatrix(), false);
		tex2DPos = PosDivi(tex2DPos, CameraControl::GetInstance()->GetCamera()->GetProjectionMatrix(), true);
		tex2DPos = WDivi(tex2DPos, false);
		tex2DPos = PosDivi(tex2DPos, CameraControl::GetInstance()->GetCamera()->GetViewPort(), false);

		DebugTextSprite::GetInstance()->SetAlpha(TexAlpha);
		DebugTextSprite::GetInstance()->Print(str.str(), tex2DPos.m128_f32[0], tex2DPos.m128_f32[1], TexSize.x);
		
		TexSize.x = min(TexSize.x, 1.4f);
		TexSize.x = max(TexSize.x, 0.0f);
}

XMVECTOR DamageManager::WDivi(const DirectX::XMVECTOR& pos, const DirectX::XMMATRIX& mat, const bool s) {
	float x, y, z, w;

	x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat.r[2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
	y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat.r[2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
	z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat.r[2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

	w = pos.m128_f32[3];
	if (s) {
		w = z;
	}

	x = x / w;
	y = y / w;
	z = z / w;

	return XMVECTOR{ x, y, z, w };
}

XMVECTOR DamageManager::WDivi(const DirectX::XMVECTOR& pos, const bool s) {
	float x, y, z, w;
	x = pos.m128_f32[0];
	y = pos.m128_f32[1];
	z = pos.m128_f32[2];
	w = pos.m128_f32[3];

	if (s) {
		w = z;
	}

	x = x / w;
	y = y / w;
	z = z / w;

	return XMVECTOR{ x, y, z, w };
}
XMVECTOR DamageManager::PosDivi(const DirectX::XMVECTOR& pos, const DirectX::XMMATRIX& mat, const bool s) {
	float x, y, z, w;

	x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat.r[2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
	y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat.r[2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
	z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat.r[2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

	w = 1.0f;

	if (s) {
		w = z;
	}

	return XMVECTOR{ x, y, z, w };
}
