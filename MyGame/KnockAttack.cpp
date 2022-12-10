#include "KnockAttack.h"
#include"EnemyControl.h"
#include"CameraControl.h"
#include"PlayerControl.h"
#include"mHelper.h"
#include"BossSpell.h"
#include"BossMap.h"
KnockAttack::~KnockAttack()
{
	//delete KnockTex;
}

void KnockAttack::Finalize()
{
	for (int i = 0; i < axeSize; i++) {
		AxeObj[i].reset(nullptr);
		ImpactTex[i].reset(nullptr);
		ImpactPar[i].reset(nullptr);
	}
}
KnockAttack* KnockAttack::GetInstance()
{
	static KnockAttack instance;
	return &instance;
}

void KnockAttack::Initialize()
{
	AxePos[0] = { 70,200,70 };
	AxePos[1] = { -70,200,70 };
	AxePos[2] = { 70,200,-70 };
	AxePos[3] = { -70,200,-70 };
	
	Texture* l_tex[4];
	Texture::LoadTexture(30, L"Resources/2d/attackeffect/inpact.png");

	for (int i = 0; i < axeSize; i++) {
		AxeObj[i] = std::make_unique<Object3d>();
		AxeObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SMALLSWORD));
		AxeObj[i]->Initialize(CameraControl::GetInstance()->GetCamera());

		l_tex[i] = Texture::Create(30);
		ImpactTex[i].reset(l_tex[i]);
		ImpactTex[i]->CreateTexture();
		ImpactTex[i]->SetAnchorPoint({ 0.5f,0.5f });
		ImpactPar[i] = std::make_unique<Particle>();
		ImpactPar[i]->Init();
		SetPos[i] = AxePos[i];
	}
	AxeRot[0] = { 90.f,180.f,90.f };
	AxeRot[1] = { 90.f,90.f,90.f };
	AxeRot[2] = { 90.f,270.f,90.f };
	AxeRot[3] = { 90.f,0.f,90.f };

}

void KnockAttack::ActionJudg()
{
	if (phase == PHASEONE)
	{
		bool nextPhase = AttackCount > 120;
		for (int i = 0; i < axeSize; i++) {
			AxePos[i].y = 200.f;
		}
		AttackCount++;
		if (nextPhase) {
			phase = PHASETWO;
		}
	}

	if (phase == PHASETWO)
	{
		AttackCount = 0;
		AxePosDownEtime += 0.02f;

		for (int i = 0; i < axeSize; i++) {
			AxePos[i].y = Easing::EaseOut(AxePosDownEtime, 200.f, 70);
		}
		for (int i = 0; i < axeSize; i++) {
			//•€‚ªŒü‚¢‚Ä‚é•ûŒü‚É“®‚­
			move[i] = { 0.0f, 0.0f, 0.1f, 0.0f };
			matRot[i] = XMMatrixRotationY(XMConvertToRadians(AxeRot[i].y));

			move[i] = XMVector3TransformNormal(move[i], matRot[i]);
		}
		if (AxePosDownEtime >= 1.0f) {
			phase =PHASETHREE;
		}
	}

	if (phase == PHASETHREE)
	{
		AxePosDownEtime += 1.f;

		
		AttackCount++;
		if (AttackCount >380) {
			for (int i = 0; i < axeSize; i++) {
				if (AxeRot[i].x <= 0) {

					CameraControl::GetInstance()->ShakeCamera();
					ImpactTexPos[i].x = AxePos[i].x + move[i].m128_f32[0] *400.f;
					ImpactTexPos[i].y = 15.f;
					ImpactTexPos[i].z= AxePos[i].z + move[i].m128_f32[2] * 400.f ;
					ImpactTexScl[i] = { 4.0f, 4.0f, 2.0f };
					ImpactTexAlpha[i] = 1.0f;
				}
				ImpactTexScl[i].x += 0.2f;
				ImpactTexScl[i].y += 0.2f;
				ImpactTexAlpha[i] -= 0.02f;
				//•€‚ªŒü‚¢‚Ä‚é•ûŒü‚É“®‚­
			move[i] = {0.0f, 0.0f, 0.1f, 0.0f};
			matRot[i] = XMMatrixRotationY(XMConvertToRadians(AxeRot[i].y));

		    move[i] = XMVector3TransformNormal(move[i], matRot[i]);
		
			AxePos[i].x += move[i].m128_f32[0] * 3.0f;
			AxePos[i].z += move[i].m128_f32[2] * 3.0f;
			AxePos[i].y = 10.f;
			AxeRot[i].x =0 + sinf(3.14f * 2.f / 30.f * AxePosDownEtime) * -70;
			
			if (AttackCount > 800) {
				phase = PHASEFOUR;
			}
			}
		}
		else {
			for (int i = 0; i < axeSize; i++) {
				AxeRot[i].x = 90 + sinf(3.14f * 2.f /90.f * AxePosDownEtime) * 90;
			}
		}
	}

	
	if (phase == PHASEFOUR) {
		BossMap::GetInstance()->DrawDamageLine(false, damageLine);

		Finalize();
	//	phase = PHASENON;
	}
	else {
		for (int i = 0; i < axeSize; i++) {
			AxeObj[i]->SetPosition(AxePos[i]);
			AxeObj[i]->SetScale({ 4,3,4 });
			AxeObj[i]->SetRotation(AxeRot[i]);
			AxeObj[i]->Update({ 1,1,1,1 }, CameraControl::GetInstance()->GetCamera());
			ImpactPar[i]->CreateParticle((phase == PHASETHREE && AttackCount > 180 && AxeRot[i].x <= 10), { AxePos[i].x + move[i].m128_f32[0] * 40.0f,14.f, AxePos[i].z + move[i].m128_f32[2] * 40.0f });
			ImpactPar[i]->Upda();

			damageLine[i].start = { AxePos[i].x,AxePos[i].z };
			damageLine[i].end = { SetPos[i].x += move[i].m128_f32[0] * 8.0f,SetPos[i].z += move[i].m128_f32[2] * 8.0f };
			BossMap::GetInstance()->DrawDamageLine(phase == PHASETHREE&&AttackCount>380, damageLine);


			ImpactTex[i]->SetPosition(ImpactTexPos[i]);
			ImpactTex[i]->SetScale(ImpactTexScl[i]);
			ImpactTex[i]->SetRotation({ 90,0,0 });
			ImpactTex[i]->SetColor({ 1,1,1,ImpactTexAlpha[i] });
			ImpactTex[i]->SetBillboard(FALSE);
			ImpactTex[i]->Update(CameraControl::GetInstance()->GetCamera());
		}
	}
}
#include"imgui.h"
void KnockAttack::Draw()
{
	int i = 0;
	if ((phase == PHASETHREE && AttackCount > 179)) {
		i = 1;
	}

	Texture::PreDraw();
	for (int i = 0; i < axeSize; i++) {
		if (ImpactTex[i] == nullptr)continue;
		ImpactTex[i]->Draw();
	}
	Texture::PostDraw();
	Object3d::PreDraw();
	for (int i = 0; i < axeSize; i++) {
		if (AxeObj[i] == nullptr)continue;
		AxeObj[i]->Draw();
	}
	Object3d::PostDraw();
	for (int i = 0; i < axeSize; i++) {
		if (ImpactPar[i] == nullptr)return;
		ImpactPar[i]->Draw();
	}

}
