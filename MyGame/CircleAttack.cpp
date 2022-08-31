#include "CircleAttack.h"
#include"CameraControl.h"
#include"mHelper.h"
CircleAttack*CircleAttack::GetInstance()
{
	static CircleAttack instance;
	return &instance;
}
void CircleAttack::Initialize()
{
	Texture::LoadTexture(21, L"Resources/AOE.png");

	for (int i = 0; i < 2; i++) {
		ImpactAreaTex[i] = Texture::Create(21, { 0,0,0 }, { 100,100,1 }, { 1,1,1,1 });
		ImpactAreaTex[i]->CreateTexture();
		ImpactAreaTex[i]->SetAnchorPoint({ 0.5,0.5 });
	}
	
//	}
	//ps0 = new OBBCollision();
	NailModel = Model::CreateFromOBJ("Nail");
	Direction[NORTH] = { 0,0,30 };
	Direction[SOUTH] = { 0,0,-30 };
	Direction[EAST] = { 30,0,0 };
	Direction[WEST] = { -30,0,0 };
}
void CircleAttack::ActionJudg()
{
	if (Input::GetInstance()->TriggerButton(Input::GetInstance()->Button_X)) {
		fase1 = true;
	}
	if (fase2) {
		AttackCount++;
		if (AttackCount >= 180) {
			fase3 = true;
			fase2 = false;
		}
	}

	if (fase3) {
		CircleAreaTime += 0.01f;
		CircleSize.x = Easing::EaseOut(CircleAreaTime, 0, 8);
		CircleSize.y = Easing::EaseOut(CircleAreaTime, 0, 8);
		if (CircleAreaTime >= 1.0f) {
			fase3 = false;
		}
	}
	for (int i = 0; i < 2; i++) {
		ImpactAreaTex[i]->Update(CameraControl::GetInstance()->GetCamera());
		ImpactAreaTex[i]->SetScale({ CircleSize.x,CircleSize.y,3 });
		ImpactAreaTex[i]->SetRotation({ 90,0,0 });
	}
	ImpactAttack(WEST, EAST);
	for (int i = 0; i < NailObj.size(); i++) {
		NailObj[i]->SetScale({ 3,3,3 });
		NailObj[i]->Update({ 1,1,1,1 }, CameraControl::GetInstance()->GetCamera());
	}
	
}

void CircleAttack::Draw()
{
	Texture::PreDraw();
	for (int i = 0; i < 2; i++) {
		ImpactAreaTex[i]->Draw();
		ImpactAreaTex[i]->Draw();
	}
	Texture::PostDraw();
	for (int i = 0; i < NailObj.size(); i++) {
		NailObj[i]->PreDraw();
		NailObj[i]->Draw();
		NailObj[i]->PostDraw();
	}
}

void CircleAttack::ImpactAttack(int area1, int area2)
{
	if (fase1) {
		NailObj.resize(2);
		for (int i = 0; i < NailObj.size(); i++) {
			NailObj[i] = std::make_unique<Object3d>();
			//フィールドにモデル割り当て
			NailObj[i]->Initialize(CameraControl::GetInstance()->GetCamera());
			NailObj[i]->SetModel(NailModel);
			//ps0 = new OBBCollision();
		}
		NailObj[0]->SetPosition(Direction[area1]);
		NailObj[1]->SetPosition(Direction[area2]);
		ImpactAreaTex[0]->SetBillboard(false);
		ImpactAreaTex[1]->SetBillboard(false);
		
		fase2 = true;
		fase1 = false;
	}
	if (fase2) {
		Direction[area1].y--;
		Direction[area2].y--;

	}
	Direction[area1].y = min(Direction[area1].y, -17);
	Direction[area2].y = min(Direction[area1].y, -17);
	ImpactAreaTex[0]->SetPosition({ Direction[area1].x ,-18,Direction[area1].z });
	ImpactAreaTex[1]->SetPosition({ Direction[area2].x ,-18,Direction[area2].z });
}