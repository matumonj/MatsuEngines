#include "AltAttack.h"
#include"EnemyControl.h"
#include"CameraControl.h"
#include"PlayerControl.h"
#include"mHelper.h"
#include"BossSpell.h"
#include"Destroy.h"
AltAttack* AltAttack::GetInstance()
{
	static AltAttack ins;
	return &ins;
}
void AltAttack::Initialize()
{
	Texture::LoadTexture(21, L"Resources/EnergieSphere.png");
	for (int i = 0; i < 2; i++) {
		DamageAreaTex[i] = Texture::Create(21, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
		DamageAreaTex[i]->CreateTexture();
		DamageAreaTex[i]->SetAnchorPoint({ 0.5f,0.5f });
	}
	ParticleManager::LoadTexture(2, L"Resources/ParticleTex/Normal.png");
	BeamParticle = ParticleManager::Create(2, L"Resources/ParticleTex/Attack.png");
	for (int i = 0; i < 2; i++) {
		BeamObj[i] = std::make_unique<Object3d>();
		BeamObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BEAM));
		BeamObj[i]->Initialize(CameraControl::GetInstance()->GetCamera());
	}
	fase = FASENON;
}

void AltAttack::CollisionParamSet()
{
	beamRay[0].start = { 60,BeamPosition[0].z };
	beamRay[1].start = {-60,BeamPosition[1].z };

	beamRay[0].end = { -60,-60 };
	beamRay[1].end = { 60,-60 };

	XMFLOAT3 Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	PlayerSpehre.x = { Ppos.x};
	PlayerSpehre.y = { Ppos.z };

}
void AltAttack::ActionJudg()
{
	BeamPosition[0] = { 60.0f,-10.0f,60.0f };
	BeamPosition[1] = { -60.0f,-10.0f,60.0f };
	BeamObj[0]->SetRotation({ 0,45,0 });
	BeamObj[1]->SetRotation({ 0,-45,0 });

	DamageAreaTex[0]->SetRotation({ 90,0,45 });
	DamageAreaTex[1]->SetRotation({ 90,0,-45 });
	DamageAreaTex[0]->SetScale({ 6,13,3 });
	DamageAreaTex[1]->SetScale({ 6,13,3 });
	if (Input::GetInstance()->TriggerButton(Input::Button_X)) {
		fase = FASEONE;
	}
	CollisionParamSet();
	EnergieCharge();

	//フェーズごとの処理
	switch (fase)
	{
	case FASENON:
		BeamEaseTime = 0.0f;
		break;
	case FASEONE:
		BossSpell::GetInstance()->SetStartSpell_UA(true);
		if (BossSpell::GetInstance()->GetEndSpell_UA()) {
			fase = FASETWO;
		}
		//fase = FASETWO;
		break;
	case FASETWO:
		BeamShotStart();
		break;
	case FASETHREE:
		BeamShotEnd();
		break;
	case FASEFOUR:
		BeamParamReset();
		BossSpell::GetInstance()->SetEndSpell_UA(false);

		break;
	default:
		break;
	}
	for (int i = 0; i < 2; i++) {
		DamageAreaTex[i]->SetUVMove(true);
		DamageAreaTex[i]->SetBillboard(false);
		DamageAreaTex[i]->SetColor({ 1.0f ,1.0f ,1.0f ,0.6f });

		DamageAreaTex[i]->Update(CameraControl::GetInstance()->GetCamera());
		DamageAreaTex[i]->SetPosition({ 0.0f ,-18.0f ,0.0f });
	}
	for (int i = 0; i < 2; i++) {
		BeamObj[i]->SetPosition(BeamPosition[i]);
		BeamObj[i]->SetScale(BeamScale);
		BeamObj[i]->SetUVf(true);
		BeamObj[i]->Update({1.5f,1.5f,1.5f,1.5f}, CameraControl::GetInstance()->GetCamera());
	}

	BeamEaseTime = min(BeamEaseTime, 1.0f);
	BeamEaseTime = max(BeamEaseTime, 0.0f);
}

void AltAttack::Draw()
{
	//ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	//BeamParticle->Draw();
	// 3Dオブジェクト描画後処理
	//ParticleManager::PostDraw();

	Texture::PreDraw();
	for (int i = 0; i < 2; i++) {
		DamageAreaTex[i]->Draw();
	}
	Texture::PostDraw();

	Object3d::PreDraw();
	for (int i = 0; i < 2; i++) {
		BeamObj[i]->Draw();
	}
	Object3d::PostDraw();
}

void AltAttack::EnergieCharge()
{
	XMFLOAT3 Bpos = EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition();
	//for (int i = 0; i < 1; i++) {
	//const float rnd_pos = 30.0f;
	//	XMFLOAT3 pos{};
	//	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	//	const float rnd_vel = 0.1f;
	//	XMFLOAT3 vel{};
	//	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	//	XMFLOAT3 acc{};
	//	const float rnd_acc = 0.001f;
	//	acc.y = -(float)rand() / RAND_MAX * rnd_acc;

	//	//追加
	//	BeamParticle->Add(120, { Bpos.x + pos.x,Bpos.y + pos.y,Bpos.z }, vel, acc, 3.0f, 0.0f);
	//	BeamParticle->SetColor({1,1,1,1});
	////}
	//BeamParticle->Update(BeamParticle->CHARGE,Bpos);
}

void AltAttack::BeamShotStart()
{
	BeamScale.x = Easing::EaseOut(BeamEaseTime, 0.0f, 10.0f);
	BeamScale.y = Easing::EaseOut(BeamEaseTime, 0.0f, 10.0f);
	BeamScale.z = 10.0f;
	
	BeamEaseTime += 0.05f;
	
	if(BeamEaseTime>=1.0f){
		BeamAttackCount++;
		if (BeamAttackCount >= 90) {
			fase = FASETHREE;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (Collision::CollsionPoint2Line2D(PlayerSpehre, beamRay[i]) < 20) {
			PlayerControl::GetInstance()->GetPlayer()->RecvDamage(5);
		}
	}
}


void AltAttack::BeamShotEnd()
{
	BeamEaseTime -= 0.02f;
	BeamScale.x = Easing::EaseOut(BeamEaseTime, 0.0f, 10.0f);
	BeamScale.y = Easing::EaseOut(BeamEaseTime, 0.0f, 10.0f);
	if (BeamEaseTime <= 0.0f) {
		fase = FASEFOUR;
	}
}

void AltAttack::BeamParamReset()
{
	texAlpha = 0.0f;
	BeamEaseTime = 0.0f;
	BeamAttackCount = 0;
	BeamScale = { 0.0f,0.0f,0.0f };
}

void AltAttack::Finalize()
{
	//vector<Texture*>EnergieSphere
	BeamObj[0].release();
	BeamObj[1].release();
	Destroy(BeamModel);
	Destroy(BeamParticle);
	Destroy(DamageAreaTex[0]);
	Destroy(DamageAreaTex[1]);

}