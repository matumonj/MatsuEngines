#include "AltAttack.h"
#include"EnemyControl.h"
#include"CameraControl.h"
#include"PlayerControl.h"
#include"mHelper.h"
AltAttack* AltAttack::GetInstance()
{
	static AltAttack ins;
	return &ins;
}
void AltAttack::Initialize()
{
///	Texture::LoadTexture(45, L"Resources/EnergieSphere.png");

	ParticleManager::LoadTexture(2, L"Resources/ParticleTex/Normal.png");
	BeamParticle = ParticleManager::Create(2, L"Resources/ParticleTex/Attack.png");
	for (int i = 0; i < 2; i++) {
		BeamObj[i] = std::make_unique<Object3d>();
		BeamObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BEAM));
		BeamObj[i]->Initialize(CameraControl::GetInstance()->GetCamera());
	}
	fase = FASEONE;
}

void AltAttack::CollisionParamSet()
{
	beamRay[0].start = { BeamPosition[0].x,BeamPosition[0].y,BeamPosition[0].z };
	beamRay[1].start = { BeamPosition[1].x,BeamPosition[1].y,BeamPosition[1].z };

	beamRay[0].dir = { -45.0f,0.0f, -90.0f };
	beamRay[1].dir = { -45.0f, 0.0f, -90.0f };

	XMFLOAT3 Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	PlayerSpehre.center = { Ppos.x,Ppos.y, Ppos.z, 1.0f };
	PlayerSpehre.radius = 10.2f;

}
void AltAttack::ActionJudg()
{
	CollisionParamSet();
	EnergieCharge();
	switch (fase)
	{
	case FASENON:
		BeamEaseTime = 0.0f;
		break;
	case FASEONE:
		
		BeamScale = { 3.0f,3.0f,1.0f };
		if (Input::GetInstance()->TriggerButton(Input::Button_Y)) {
			fase = FASETWO;
		}
		break;
	case FASETWO:
		BeamEaseTime += 0.01f;
		BeamScale.x = Easing::EaseOut(BeamEaseTime, 3.0f, 18.0f);
		BeamScale.y = Easing::EaseOut(BeamEaseTime, 3.0f, 18.0f);
		BeamScale.z = 22.0f;
		if (BeamEaseTime >= 1.0f) {
			fase = FASETHREE;
		}
		break;
	case FASETHREE:
		break;
	case FASEFOUR:
		break;
	default:
		break;
	}
	for (int i = 0; i < 2; i++) {
		if (Collision::CheckRay2Sphere(beamRay[i], PlayerSpehre)) {
			PlayerControl::GetInstance()->GetPlayer()->RecvDamage(20);
		}
	}
	BeamObj[0]->SetPosition({60.0f,-10.0f,120.0f});
	BeamObj[0]->SetRotation({ 0.0f, 45.0f, 0.0f });
	BeamObj[1]->SetPosition({ -60.0f,-10.0f,120.0f });
	BeamObj[1]->SetRotation({ 0.0f, -45.0f, 180.0f });


	for (int i = 0; i < 2; i++) {
		BeamObj[i]->SetScale(BeamScale);
		BeamObj[i]->SetUVf(true);
		BeamObj[i]->Update({1.0f,1.0f,1.0f,1.0f}, CameraControl::GetInstance()->GetCamera());
	}
}

void AltAttack::Draw()
{
	ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	BeamParticle->Draw();
	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();
	Object3d::PreDraw();
	for (int i = 0; i < 2; i++) {
		BeamObj[i]->Draw();
	}
	Object3d::PostDraw();
}

void AltAttack::BeamAttack()
{

}

void AltAttack::EnergieCharge()
{
	XMFLOAT3 Bpos = EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition();
	//for (int i = 0; i < 1; i++) {
		const float rnd_pos = 30.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		BeamParticle->Add(120, { Bpos.x + pos.x,Bpos.y + pos.y,Bpos.z }, vel, acc, 3.0f, 0.0f);
		BeamParticle->SetColor({1,1,1,1});
	//}
	BeamParticle->Update(BeamParticle->CHARGE,Bpos);
}