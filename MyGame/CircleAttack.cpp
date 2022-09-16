#include "CircleAttack.h"
#include"CameraControl.h"
#include"mHelper.h"
#include"Nail.h"
#include"BossSpell.h"
#include"Collision.h"
#include"PlayerControl.h"
CircleAttack::~CircleAttack()
{
	//delete ImpactAreaTex;
}
CircleAttack*CircleAttack::GetInstance()
{
	static CircleAttack instance;
	return &instance;
}
void CircleAttack::Initialize()
{
	Texture::LoadTexture(23, L"Resources/AOE.png");

	for (int i = 0; i < 2; i++) {
		ImpactAreaTex[i] = Texture::Create(23, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
		ImpactAreaTex[i]->CreateTexture();
		ImpactAreaTex[i]->SetAnchorPoint({ 0.5f,0.5f });
	}
	
//	}
	//ps0 = new OBBCollision();
	NailModel = Model::CreateFromOBJ("Nail");
	Direction[NORTH] = { 0.0f ,0.0f ,30.0f };
	Direction[SOUTH] = { 0.0f ,0.0f ,-30.0f };
	Direction[EAST] = { 30.0f ,0.0f ,0.0f };
	Direction[WEST] = { -30.0f ,0.0f ,0.0f };
}
void CircleAttack::ActionJudg()
{
	
	if (fase==FASEONE) {
		NailAttackFlag = false;
		BossSpell::GetInstance()->SetStartSpell_CA(true);
		if (BossSpell::GetInstance()->GetEndSpell_CA()) {
			fase = FASETWO;
		}
	}

	if (fase == FASETWO) {
		CircleAreaTime += 0.01f;
		CircleSize.x = Easing::EaseOut(CircleAreaTime, 0, 8);
		CircleSize.y = Easing::EaseOut(CircleAreaTime, 0, 8);
		AttackCount = 0;
		if (CircleAreaTime >= 1.0f) {
			fase = FASETHREE;
		}
	}

	if (fase == FASETHREE) {
		Nail::GetInstance()->CircleAttack(Area1, Area2);
		CollisonNailPlayer();

		Direction[Area1].y++;
		Direction[Area2].y++;
	
		NailObj[0]->SetPosition(Direction[Area1]);
		NailObj[1]->SetPosition(Direction[Area2]);
		TexAlpha -= 0.01f;
		if (TexAlpha <= 0.0f) {
			fase = FASEFOUR;
		}
	}


	for (int i = 0; i < 2; i++) {
		ImpactAreaTex[i]->Update(CameraControl::GetInstance()->GetCamera());
		ImpactAreaTex[i]->SetScale({ CircleSize.x,CircleSize.y,3.0f });
		ImpactAreaTex[i]->SetRotation({ 90.0f,0.0f,0.0f });
		ImpactAreaTex[i]->SetColor({ 1.0f,1.0f,1.0f,TexAlpha});
	}
	ImpactAttack();
	for (int i = 0; i < NailObj.size(); i++) {
		NailObj[i]->SetScale({ 3.0f,3.0f,3.0f });
		NailObj[i]->Update({ 1.0f,1.0f,1.0f,1.0f }, CameraControl::GetInstance()->GetCamera());
	}
	TexAlpha = min(TexAlpha, 1.0f);
	TexAlpha = max(TexAlpha, 0.0f);
}

void CircleAttack::Draw()
{
	Texture::PreDraw();
	if (Nail::GetInstance()->GetEndAction_Circle() == false) {
		for (int i = 0; i < 2; i++) {
			ImpactAreaTex[i]->Draw();
			ImpactAreaTex[i]->Draw();
		}
	}
	Texture::PostDraw();
	for (int i = 0; i < NailObj.size(); i++) {
			Object3d::PreDraw();
			NailObj[i]->Draw();
			Object3d::PostDraw();
		}
	if (fase == FASETHREE) {
	Nail::GetInstance()->Draw();
	}
}

void CircleAttack::ImpactAttack()
{
	
	if (fase==FASEONE) {
		NailObj.resize(2);
		for (int i = 0; i < NailObj.size(); i++) {
			NailObj[i] = std::make_unique<Object3d>();
			//フィールドにモデル割り当て
			NailObj[i]->Initialize(CameraControl::GetInstance()->GetCamera());
			NailObj[i]->SetModel(NailModel);
			
			//ps0 = new OBBCollision();
		}
	
		ImpactAreaTex[0]->SetBillboard(false);
		ImpactAreaTex[1]->SetBillboard(false);
		
	}
	if (fase == FASEONE) {
		if (Direction[Area1].y > -17.0f) {
			Direction[Area1].y--;
			Direction[Area2].y--;
		}
		NailObj[0]->SetPosition(Direction[Area1]);
		NailObj[1]->SetPosition(Direction[Area2]);
	}
	ImpactAreaTex[0]->SetPosition({ Direction[Area1].x ,-18,Direction[Area1].z });
	ImpactAreaTex[1]->SetPosition({ Direction[Area2].x ,-18,Direction[Area2].z });

}

void CircleAttack::CollisonNailPlayer()
{
	if(Collision::GetLength(NailObj[0]->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 30) {
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(20);
	}
	if (Collision::GetLength(NailObj[1]->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 30) {
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(20);
	}
}