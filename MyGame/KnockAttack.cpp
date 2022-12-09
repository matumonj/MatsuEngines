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

KnockAttack* KnockAttack::GetInstance()
{
	static KnockAttack instance;
	return &instance;
}

void KnockAttack::Initialize()
{
	AxePos[0] = { 40,200,40 };
	AxePos[1] = { -40,200,40 };
	AxePos[2] = { 40,200,-40 };
	AxePos[3] = { -40,200,-40 };
	for (int i = 0; i < axeSize; i++) {
		AxeObj[i] = std::make_unique<Object3d>();
		AxeObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SMALLSWORD));
		AxeObj[i]->Initialize(CameraControl::GetInstance()->GetCamera());

		AxeRot[i] = { 90.f,float(i) * 90.f,90.f };

		ImpactPar[i] = std::make_unique<Particle>();
		ImpactPar[i]->Init();
		SetPos[i] = AxePos[i];
	}
	

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
		AxePosDownEtime += 0.002f;

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
		AxePosDownEtime += 0.5f;

		
		AttackCount++;
		if (AttackCount > 380) {
			for (int i = 0; i < axeSize; i++) {
			//•€‚ªŒü‚¢‚Ä‚é•ûŒü‚É“®‚­
			move[i] = {0.0f, 0.0f, 0.1f, 0.0f};
			matRot[i] = XMMatrixRotationY(XMConvertToRadians(AxeRot[i].y));

		    move[i] = XMVector3TransformNormal(move[i], matRot[i]);
		
			AxePos[i].x += move[i].m128_f32[0] * 5.0f;
			AxePos[i].z += move[i].m128_f32[2] * 5.0f;
			AxePos[i].y = 10.f;
			AxeRot[i].x =0 + sinf(3.14f * 2.f / 30.f * AxePosDownEtime) * -70;
			if (AxeRot[i].x <= -50) {
				CameraControl::GetInstance()->ShakeCamera();
			}
			if (AttackCount > 800) {
				phase = PHASEFOUR;
			}
			}
		}
		else {
			for (int i = 0; i < axeSize; i++) {
				AxeRot[i].x = 90 + sinf(3.14f * 2.f / 120.f * AxePosDownEtime) * 90;
			}
		}
	}

	if (phase == PHASEFOUR) {

	}
	for (int i = 0; i < axeSize; i++) {
		AxeObj[i]->SetPosition(AxePos[i]);
		AxeObj[i]->SetScale({ 4,3,4 });
		AxeObj[i]->SetRotation(AxeRot[i]);
		AxeObj[i]->Update({ 1,1,1,1 }, CameraControl::GetInstance()->GetCamera());
		ImpactPar[i]->CreateParticle((phase==PHASETHREE&&AttackCount>180&&AxeRot[i].x<=10), {AxePos[i].x + move[i].m128_f32[0] * 40.0f,14.f, AxePos[i].z + move[i].m128_f32[2] * 40.0f});
		ImpactPar[i]->Upda();
		damageLine[i].start = { AxePos[i].x,AxePos[i].z };
		damageLine[i].end = { SetPos[i].x+= move[i].m128_f32[0] * 8.0f,SetPos[i].z+= move[i].m128_f32[2] * 8.0f };
		BossMap::GetInstance()->DrawDamageLine( phase == PHASETHREE, damageLine);
	}
	
}
#include"imgui.h"
void KnockAttack::Draw()
{
	int i = 0;
	if ((phase == PHASETHREE && AttackCount > 179)) {
		i = 1;
	}
	ImGui::Begin("k");
	
	ImGui::Text("%d", i);
	ImGui::End();
	Object3d::PreDraw();
	for (int i = 0; i < axeSize; i++) {
		AxeObj[i]->Draw();
	}
	Object3d::PostDraw();
	for (int i = 0; i < axeSize; i++) {
		ImpactPar[i]->Draw();
	}
}
