#include "DamageManager.h"
DamageManager* DamageManager::GetIns() {
	static DamageManager instans;
	return &instans;
}

void DamageManager::Init() {
	const int w = 32;
	const int h = 64;
	const int l = 10;
	Sprite::LoadTexture(310, L"Resources/2d/Num.png");
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 10; j++) {
			num[i][j] = Sprite::Create(310, { 0.0f,0.0f });
			int number_index_y = j / l;
			int number_index_x = j % l;
			num[i][j]->SetTextureRect(
				{ static_cast<float>(number_index_x) * w, static_cast<float>(number_index_y) * h },
				{ static_cast<float>(w), static_cast<float>(h) });
			num[i][j]->SetSize({ w,h });
			num[i][j]->SetAnchorPoint({ 0,0 });
			num[i][j]->SetSize({ w * 2,h * 2 });
			num[i][j]->setcolor({ 1.0f,0.0f,0.0f,1.0f });
			num[i][j]->SetPosition({ (float)980 - (70.0f * i) ,580.0f });
		}
	}
	Texture* Doll_ = Texture::Create(310, { 1050.0f,630.0f,-50 }, { 3,3,3 }, {1,1,1,1});

	Doll.reset(Doll_);
	Doll->CreateTexture();
}
void DamageManager::Upda() {

	First.clear();
	for (int tmp = score; tmp > 0;) {
		First.push_back(tmp % 10);
		tmp /= 10;
	}

}
void DamageManager::Draw() {
	Sprite::PreDraw();
	if (score == 0) {
		num[0][0]->Draw();

	} else {
		for (int i = 0; i < First.size() && i < 5; i++) {
			num[i][(int)First[i]]->Draw();
		}
	}
	Doll->Draw();
	Sprite::PostDraw();
}