#include "RushAttack.h"
#include"EnemyControl.h"
#include"CameraControl.h"
#include"PlayerControl.h"
#include"mHelper.h"
#include"BossMap.h"

RushAttack* RushAttack::GetInstance()
{
	static RushAttack ins;
	return &ins;
}

void RushAttack::Initialize()
{
	RushSphereObj = std::make_unique<Object3d>();
	RushSphereObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SPHERE));
	RushSphereObj->Initialize(CameraControl::GetInstance()->GetCamera());

	phase = PHASENON;
	area = FIR;
	for (int i = 0; i < 5; i++)
	{
		rushEtime[i] = 0.0f;
	}
	rushspherealpha = 0.0f;
	DestTime = 0.0f;
	RushEaseTime = 0.0f;
	RushAttackCount = 0;

	RushSphereObj->SetDestFlag(false);
	RushSphereObj->SetDestTime(DestTime);
	rushspherescl = {0.0f, 0.0f, 0.0f};
	p = new Particle();
	p->Init(64);
}

void RushAttack::CollisionParamSet()
{
}

void RushAttack::ActionJudg()
{
	CollisionParamSet();

	//フェーズごとの処理
	switch (phase)
	{
	case PHASENON:
		RushEaseTime = 0.0f;
		break;
	case PHASEONE:
		//CameraControl::GetInstance()->SetCameraState(CameraControl::RUSHSCENE);

		phase = PHASETWO;
		break;
	case PHASETWO:
		RushStart();
		break;
	case PHASETHREE:

		RushMoving();
		break;
	case PHASEFOUR:

		RushParamReset();

		break;
	default:
		break;
	}


	RushEaseTime = min(RushEaseTime, 1.0f);
	RushEaseTime = max(RushEaseTime, 0.0f);
}

void RushAttack::Upda()
{
	Enemy* boss = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0].get();
	if (boss == nullptr)
	{
		return;
	}
	if (area == FIV)
	{
		DestTime += 0.2f;
		rushspherealpha -= 0.02f;
		RushSphereObj->SetDestFlag(TRUE);
		RushSphereObj->SetDestTime(DestTime);
	}

	RushSphereObj->SetScale(rushspherescl);
	RushSphereObj->SetColor({1.f, 1.f, 1.f, rushspherealpha});
	RushSphereObj->SetUVf(true);
	RushSphereObj->Update({1.5f, 1.5f, 1.5f, 1.5f}, CameraControl::GetInstance()->GetCamera());
}

#include"imgui.h"

void RushAttack::Draw()
{
	//ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	//RushParticle->Draw();
	// 3Dオブジェクト描画後処理
	//ParticleManager::PostDraw();

	//p->Draw();
	RushObjDraw();
}


void RushAttack::RushObjDraw()
{
	if (RushSphereObj == nullptr)
	{
		return;
	}
	Object3d::PreDraw();
	if (rushspherealpha > 0.0f)
	{
		RushSphereObj->Draw();
	}
	Object3d::PostDraw();
}

void RushAttack::RushStart()
{
	//atckjudg = true;
	rushspherealpha = 0.8f;
	//イージングカウント
	const float EaseC = 0.005f;
	//被ダメージ
	const int Damage = 10;
	rushspherescl.x = Easing::EaseOut(RushEaseTime, 0.0f, 30.0f);
	rushspherescl.y = Easing::EaseOut(RushEaseTime, 0.0f, 30.0f);
	rushspherescl.z = Easing::EaseOut(RushEaseTime, 0.0f, 30.0f);

	RushEaseTime += EaseC;

	rushpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
	rushpos.y = 8;
	//EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->SetPosition(rushpos);
	RushSphereObj->SetPosition(rushpos);


	if (RushEaseTime >= 1.0f)
	{
		RushAttackCount++;
		if (RushAttackCount >= 90)
		{
			phase = PHASETHREE;
		}
	}
}


void RushAttack::Rush(Area& area, Area now, Area next, float& t)
{
	XMFLOAT3 Bpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();

	if (area == now)
	{
		if (t == 0)
		{
			oldp[now] = rushpos;
		}
		t += 0.05f;
		if (t < 1.0f)
		{
			rushpos.x = Easing::EaseOut(t, oldp[now].x, rushimpactarea[now].x);
			rushpos.z = Easing::EaseOut(t, oldp[now].z, rushimpactarea[now].z);
		}
		if (now != END)
		{
			if (t >= 3.0f)
			{
				DamageLine.end = {rushimpactarea[next].x, rushimpactarea[next].z};
			}
			if (t >= 6.0f)
			{
				area = next;
			}
		}
	}
}

void RushAttack::RushMoving()
{
	rushimpactarea[FIR] = {0, -14.75, -100};
	rushimpactarea[SEC] = {0, -14.75, 100};
	rushimpactarea[THI] = {-140, -14.75, -20};
	rushimpactarea[FIU] = {140, -14.75, -20};
	rushimpactarea[FIV] = {0, -14.75, -100};

	XMFLOAT3 Bpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();


	DamageLine.start = {Bpos.x, Bpos.z};

	if (Collision::GetLength(Bpos, PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 30.f)
	{
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(20);
	}
	Rush(area, FIR, SEC, rushEtime[FIR]);
	Rush(area, SEC, THI, rushEtime[SEC]);
	Rush(area, THI, FIU, rushEtime[THI]);
	Rush(area, FIU, FIV, rushEtime[FIU]);
	Rush(area, FIV, END, rushEtime[FIV]);

	rushpos.y = 14.75f;
	EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->SetPosition(rushpos);
	RushSphereObj->SetPosition(rushpos);


	if (rushEtime[FIV] >= 1.0f)
	{
		BossMap::GetInstance()->DrawDamageLine(false, DamageLine);
		phase = PHASEFOUR;
	}
	else
	{
		BossMap::GetInstance()->DrawDamageLine(true, DamageLine);
	}
}

void RushAttack::RushParamReset()
{
	RushEaseTime = 0.0f;
	RushAttackCount = 0;
}

void RushAttack::Finalize()
{
	//vector<Texture*>EnergieSphere
	RushSphereObj.release();
}
