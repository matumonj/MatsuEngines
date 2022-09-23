#include "AltAttack.h"
#include"EnemyControl.h"
#include"CameraControl.h"
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
	BeamObj = std::make_unique<Object3d>();
	BeamObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BEAM));
	BeamObj->Initialize(CameraControl::GetInstance()->GetCamera());
}

void AltAttack::ActionJudg()
{
	EnergieCharge();
	switch (fase)
	{
	case FASENON:

		break;
	case FASEONE:
		break;
	case FASETWO:
		break;
	case FASETHREE:
		break;
	case FASEFOUR:
		break;
	default:
		break;
	}
	BeamObj->SetScale({ 1.0f,1.0f,1.0f });
	BeamObj->Update({ 1.0f,1.0f,1.0f,1.0f }, CameraControl::GetInstance()->GetCamera());

}

void AltAttack::Draw()
{
	ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	BeamParticle->Draw();
	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();
	Object3d::PreDraw();
	BeamObj->Draw();
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