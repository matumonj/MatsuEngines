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
	//Texture::LoadTexture(21, L"Resources/EnergieSphere.png");
	for (int i = 0; i < 2; i++)
	{
		DamageAreaTex[i] = Texture::Create(21, {0.0f, 0.0f, 0.0f}, {100.0f, 100.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
		DamageAreaTex[i]->CreateTexture();
		DamageAreaTex[i]->SetAnchorPoint({0.5f, 0.5f});
	}
	ParticleManager::LoadTexture(2, L"Resources/ParticleTex/Normal.png");
	BeamParticle = ParticleManager::Create(2, L"Resources/ParticleTex/Attack.png");

		RushSphereObj = std::make_unique<Object3d>();
		RushSphereObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SPHERE));
		RushSphereObj->Initialize(CameraControl::GetInstance()->GetCamera());
	
	texAlpha = 0.7f;

	fase = FASENON;

	p = new Particle();
	p->Init();
}

void AltAttack::CollisionParamSet()
{
	
	XMFLOAT3 Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	PlayerSpehre.x = {Ppos.x};
	PlayerSpehre.y = {Ppos.z};

}

void AltAttack::ActionJudg()
{

	CollisionParamSet();
	EnergieCharge();

	//フェーズごとの処理
	switch (fase)
	{
	case FASENON:
		texAlpha = 0.7f;
		BeamEaseTime = 0.0f;
		break;
	case FASEONE:
		BossSpell::GetInstance()->SetStartSpell(BossSpell::ULTIMATE, true);
		if (BossSpell::GetInstance()->GetEndSpell(BossSpell::ULTIMATE))
		{
			fase = FASETWO;
		}
		break;
	case FASETWO:
		BeamShotStart();
		break;
	case FASETHREE:
		BeamShotEnd();
		break;
	case FASEFOUR:
		BeamParamReset();
		BossSpell::GetInstance()->SetEndSpell(BossSpell::ULTIMATE, false);

		break;
	default:
		break;
	}
	for (int i = 0; i < 2; i++)
	{
		DamageAreaTex[i]->SetUVMove(true);
		DamageAreaTex[i]->SetBillboard(false);
		DamageAreaTex[i]->SetColor({1.0f, 1.0f, 1.0f, texAlpha});

		DamageAreaTex[i]->Update(CameraControl::GetInstance()->GetCamera());
		DamageAreaTex[i]->SetPosition({0.0f, -18.0f, 0.0f});
	}
	BeamObjSetParam();


	BeamEaseTime = min(BeamEaseTime, 1.0f);
	BeamEaseTime = max(BeamEaseTime, 0.0f);
}
void AltAttack::Upda()
{
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();

	RushAttack();
	p->CreateParticle(true, RushSphereObj->GetPosition());
	p->Upda();
	RushSphereObj->SetScale({15,15,15});
	RushSphereObj->SetColor({ 0.8f,0.1f,0.1f,0.5f });
	RushSphereObj->SetUVf(true);
	RushSphereObj->Update({ 1.5f, 1.5f, 1.5f, 1.5f }, CameraControl::GetInstance()->GetCamera());

}
#include"imgui.h"
void AltAttack::Draw()
{
	//ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	//BeamParticle->Draw();
	// 3Dオブジェクト描画後処理
	//ParticleManager::PostDraw();
	ImGui::Begin("Alt");
	ImGui::Text("spherepx %f", rushpos.x);
	ImGui::Text("spherepz %f", rushpos.z);
	ImGui::End();
	Texture::PreDraw();
	for (int i = 0; i < 2; i++)
	{
		DamageAreaTex[i]->Draw();
	}
	Texture::PostDraw();
	p->Draw();
	BeamObjDraw();
}

void AltAttack::EnergieCharge()
{
	XMFLOAT3 Bpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
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

void AltAttack::BeamObjSetParam()
{
	if (RushSphereObj== nullptr)
	{
		return;
	}

	BeamPosition[0] = {60.0f, -10.0f, 60.0f};
	RushSphereObj->SetRotation({0, 45, 0});

}

void AltAttack::BeamObjDraw()
{
	if (RushSphereObj == nullptr)
	{
		return;
	}
	Object3d::PreDraw();
	
		RushSphereObj->Draw();
	
	Object3d::PostDraw();
}

void AltAttack::BeamShotStart()
{
	//イージングカウント
	const float EaseC = 0.05f;
	//被ダメージ
	const int Damage = 10;

	BeamScale.x = Easing::EaseOut(BeamEaseTime, 0.0f, 10.0f);
	BeamScale.y = Easing::EaseOut(BeamEaseTime, 0.0f, 10.0f);
	BeamScale.z = 10.0f;

	BeamEaseTime += EaseC;

	if (BeamEaseTime >= 1.0f)
	{
		BeamAttackCount++;
		if (BeamAttackCount >= 90)
		{
			fase = FASETHREE;
		}
	}
	for (int i = 0; i < 2; i++)
	{
		if (Collision::CollsionPoint2Line2D(PlayerSpehre, beamRay[i]) < 20)
		{
			PlayerControl::GetInstance()->GetPlayer()->RecvDamage(Damage);
		}
	}
}


void AltAttack::BeamShotEnd()
{
	//イージングカウント
	const float EaseC = 0.02f;

	BeamEaseTime -= EaseC;
	BeamScale.x = Easing::EaseOut(BeamEaseTime, 0.0f, 10.0f);
	BeamScale.y = Easing::EaseOut(BeamEaseTime, 0.0f, 10.0f);

	texAlpha = 0.0f;
	if (BeamEaseTime <= 0.0f)
	{
		//1度しか発動しないのであればここで破棄しても問題ないはず　メモリの無駄です
		Destroy_unique(RushSphereObj);
		fase = FASEFOUR;
	}
}

void AltAttack::Rush(Area& area,Area now,Area next,float& t)
{
	XMFLOAT3 Bpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
	
	if (area ==now) {
		if (t == 0) {
			oldp[now] = rushpos;
		}
		t += 0.05f;
		if (t < 1.0f) {
			rushpos.x= Easing::EaseOut(t, oldp[now].x, rushimpactarea[now].x);
			rushpos.z = Easing::EaseOut(t, oldp[now].z, rushimpactarea[now].z);
		}
		if (t >= 4.0f) {
			area = next;
		}
	}
	
}
void AltAttack::RushAttack()
{
	rushimpactarea[FIR] = { 0,-19,-100 };
	rushimpactarea[SEC] = { 0,-19,100 };
	rushimpactarea[THI] = { -100,-19,50 };
	rushimpactarea[FIU] = { 100,-19,50 };
	rushimpactarea[FIV] = { 0,-19,-100 };

	XMFLOAT3 Bpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();

	if (Input::GetInstance()->TriggerButton(Input::LT)) {
		atckjudg = true;
	}

	if (atckjudg) {
		Rush(area, FIR, SEC, rushEtime[FIR]);
		Rush(area, SEC, THI, rushEtime[SEC]);
		Rush(area, THI, FIU, rushEtime[THI]);
		Rush(area, FIU, FIV, rushEtime[FIU]);

		rushpos.y = 18;
		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->SetPosition(rushpos);
		RushSphereObj->SetPosition(rushpos);

	}
}
void AltAttack::BeamParamReset()
{
	texAlpha = 0.0f;
	BeamEaseTime = 0.0f;
	BeamAttackCount = 0;
	BeamScale = {0.0f, 0.0f, 0.0f};
}

void AltAttack::Finalize()
{
	//vector<Texture*>EnergieSphere
	RushSphereObj.release();
	Destroy(BeamModel);
	Destroy(BeamParticle);
	Destroy(DamageAreaTex[0]);
	Destroy(DamageAreaTex[1]);
}
