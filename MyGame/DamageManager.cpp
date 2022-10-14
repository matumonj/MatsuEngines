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

		DamageTex.remove_if([](std::unique_ptr<DebugTxt>& dtex) {
			return dtex->GetisDestroy();
		});
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
		std::unique_ptr<DebugTxt>newDTex;
		std::ostringstream str;

		newDTex = std::make_unique<DebugTxt>();
		newDTex->Initialize(47);
		newDTex->SetPosition(Position);
		
		str << std::fixed << std::setprecision(2)
			<< damage;
		newDTex->SetText(str.str());

		DamageTex.push_back(std::move(newDTex));
	
		
}
