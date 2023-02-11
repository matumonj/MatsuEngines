#include "KnockAttack.h"
#include"mHelper.h"
#include"BossMap.h"
#include <BossEnemy.h>

#include "CameraControl.h"
#include "EnemyControl.h"
#include "PlayerControl.h"

KnockAttack::KnockAttack()
{
	Texture::LoadTexture(30, L"Resources/2d/attackeffect/inpact.png");
	Texture::LoadTexture(31, L"Resources/2d/damage/HammerDamageArea.png");
	
}

KnockAttack::~KnockAttack()
{
	//delete KnockTex;
}

void KnockAttack::Finalize()
{
	for (int i = 0; i < axeSize; i++)
	{
		AxeObj[i].reset(nullptr);
		ImpactTex[i].reset(nullptr);
		ImpactPar[i].reset(nullptr);
	}
}

KnockAttack* KnockAttack::GetIns()
{
	static KnockAttack instance;
	return &instance;
}

void KnockAttack::Initialize()
{
	AxePos[0] = {50.f, 200.f, 50.f};
	AxePos[1] = {-50.f, 200.f, 50.f};
	AxePos[2] = {50.f, 200.f, -50.f};
	AxePos[3] = {-50.f, 200.f, -50.f};

	Texture* l_tex[4];
	Texture* l_tex2[4];
	
	for (int i = 0; i < axeSize; i++)
	{
		AxeObj[i] = std::make_unique<Object3d>();
		AxeObj[i]->Initialize(CameraControl::GetIns()->GetCamera());
		AxeObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SMALLSWORD));
		
		l_tex[i] = Texture::Create(30);
		l_tex2[i] = Texture::Create(31);
		ImpactTex[i].reset(l_tex[i]);
		ImpactTex[i]->CreateTexture();
		ImpactTex[i]->SetAnchorPoint({0.5f, 0.5f});

		AxeDirectionTex[i].reset(l_tex2[i]);
		AxeDirectionTex[i]->CreateTexture();
		AxeDirectionTex[i]->SetAnchorPoint({0.5f, 0.5f});
		ImpactPar[i] = std::make_unique<Particle>();
		ImpactPar[i]->Init(64);
		SetPos[i] = AxePos[i];
	}
	AxeRot[0] = {0.f, 180.f, 90.f};
	AxeRot[1] = {0.f, 90.f, 90.f};
	AxeRot[2] = {0.f, 270.f, 90.f};
	AxeRot[3] = {0.f, 0.f, 90.f};

	AxePosDownEtime = 0.f;
	AttackCount = 0;
	axeDirectionTexAlpha = 0.f;
	phase = Phase::PHASENON;
}

void KnockAttack::ActionJudg()
{
	if (phase == PHASEONE)
	{
		bool nextPhase = AttackCount > 120;
		for (int i = 0; i < axeSize; i++)
		{
			AxePos[i].y = 200.f;
		}
		AttackCount++;
		if (nextPhase)
		{
			phase = PHASETWO;
		}
		EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->SetAnimation(BossEnemy::NowAttackMotion::MAGIC, 1.f, false);
	}

	if (phase == PHASETWO)
	{
		EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->SetAnimation(BossEnemy::NowAttackMotion::MAGIC,1.f,false);
	
		AttackCount = 0;
		AxePosDownEtime += 0.02f;

		for (int i = 0; i < axeSize; i++)
		{
			AxePos[i].y = Easing::EaseOut(AxePosDownEtime, 200.f, 20);
		}
		for (int i = 0; i < axeSize; i++)
		{
			//•€‚ªŒü‚¢‚Ä‚é•ûŒü‚É“®‚­
			move[i] = {0.0f, 0.0f, 0.1f, 0.0f};
			matRot[i] = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(AxeRot[i].y));

			move[i] = XMVector3TransformNormal(move[i], matRot[i]);
		}
		if (AxePosDownEtime >= 1.0f)
		{
			phase = PHASETHREE;
		}
	}

	if (phase == PHASETHREE)
	{
		AxePosDownEtime += 1.f;


		AttackCount++;
		if (AttackCount > 380)
		{
			axeDirectionTexAlpha -= 0.02f;
			for (int i = 0; i < axeSize; i++)
			{
				if (AxeRot[i].x <= 0)
				{
					CameraControl::GetIns()->ShakeCamera();
					ImpactTexPos[i].x = AxePos[i].x + move[i].m128_f32[0] * 400.f;
					ImpactTexPos[i].y = 15.f;
					ImpactTexPos[i].z = AxePos[i].z + move[i].m128_f32[2] * 400.f;
					ImpactTexScl[i] = {4.0f, 4.0f, 2.0f};
					ImpactTexAlpha[i] = 1.0f;
				}
				ImpactTexScl[i].x += 0.2f;
				ImpactTexScl[i].y += 0.2f;
				ImpactTexAlpha[i] -= 0.02f;
				//•€‚ªŒü‚¢‚Ä‚é•ûŒü‚É“®‚­
				move[i] = {0.0f, 0.0f, 0.1f, 0.0f};
				matRot[i] = XMMatrixRotationY(XMConvertToRadians(AxeRot[i].y));

				move[i] = XMVector3TransformNormal(move[i], matRot[i]);

				AxePos[i].x += move[i].m128_f32[0] * 6.0f;
				AxePos[i].z += move[i].m128_f32[2] * 6.0f;
				AxePos[i].y = 10.f;

				AxeRot[i].x += 6.f;
				if (AttackCount > 800)
				{
					phase = PHASEFOUR;
				}
			}
		}
		else
		{
			axeDirectionTexAlpha += 0.02f;
			for (int i = 0; i < axeSize; i++)
			{
				AxeRot[i].x = 90 + sinf(3.14f * 2.f / 90.f * AxePosDownEtime) * 90;
			}
		}
	}


	if (phase == PHASEFOUR)
	{
		BossMap::GetIns()->DrawDamageLine(false, damageLine);

		Finalize();
		//	phase = PHASENON;
	}
	else
	{
		for (int i = 0; i < axeSize; i++)
		{
			if (AxeObj[i] == nullptr)continue;
			if(Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(),AxePos[i])<20.f)
			{
				PlayerControl::GetIns()->GetPlayer()->RecvDamage(10);
			}
			AxeObj[i]->SetPosition(AxePos[i]);
			AxeObj[i]->SetScale({8.f, 8.f, 8.f});
			AxeObj[i]->SetRotation(AxeRot[i]);
			AxeObj[i]->Update( CameraControl::GetIns()->GetCamera());
			ImpactPar[i]->CreateParticle((phase == PHASETHREE && AttackCount > 180 && AxeRot[i].x <= 10), {
				                             AxePos[i].x + move[i].m128_f32[0] * 40.0f, 14.f,
				                             AxePos[i].z + move[i].m128_f32[2] * 40.0f
			                             });
			ImpactPar[i]->Upda();

			damageLine[i].start = {AxePos[i].x, AxePos[i].z};
			damageLine[i].end = {SetPos[i].x += move[i].m128_f32[0] * 8.0f, SetPos[i].z += move[i].m128_f32[2] * 8.0f};
			BossMap::GetIns()->DrawDamageLine(phase == PHASETHREE && AttackCount > 380, damageLine);

			AxeDirectionTex[i]->SetPosition({AxePos[i].x, 15.f, AxePos[i].z});
			AxeDirectionTex[i]->SetScale({7.f, 7.f, 0.f});
			AxeDirectionTex[i]->SetColor({1.f, 1.f, 1.f, axeDirectionTexAlpha});
			AxeDirectionTex[i]->SetBillboard(FALSE);
			AxeDirectionTex[i]->SetUVMove(TRUE);
			AxeDirectionTex[i]->Update(CameraControl::GetIns()->GetCamera());

			ImpactTex[i]->SetPosition(ImpactTexPos[i]);
			ImpactTex[i]->SetScale(ImpactTexScl[i]);
			ImpactTex[i]->SetRotation({90.f, 0.f, 0.f});
			ImpactTex[i]->SetColor({1.f, 1.f, 1.f, ImpactTexAlpha[i]});
			ImpactTex[i]->SetBillboard(FALSE);
			ImpactTex[i]->Update(CameraControl::GetIns()->GetCamera());
		}
		AxeDirectionTex[0]->SetRotation({90.f, 0.f, AxeRot[0].y + 180});
		AxeDirectionTex[1]->SetRotation({90.f, 0.f, AxeRot[1].y});
		AxeDirectionTex[2]->SetRotation({90, 0, AxeRot[2].y});
		AxeDirectionTex[3]->SetRotation({-90, 180, AxeRot[3].y + 180});
	}

	axeDirectionTexAlpha = min(axeDirectionTexAlpha, 1.0f);
	axeDirectionTexAlpha = max(axeDirectionTexAlpha, 0.0f);
}

#include"imgui.h"

void KnockAttack::Draw()
{
	int i = 0;
	if ((phase == PHASETHREE && AttackCount > 179))
	{
		i = 1;
	}

	Texture::PreDraw();
	for (int i = 0; i < axeSize; i++)
	{
		if (ImpactTex[i] == nullptr)
		{
			continue;
		}
		ImpactTex[i]->Draw();
		AxeDirectionTex[i]->Draw();
	}
	Texture::PostDraw();
	Object3d::PreDraw();
	for (int i = 0; i < axeSize; i++)
	{
		if (AxeObj[i] == nullptr)
		{
			continue;
		}
		AxeObj[i]->Draw();
	}
	Object3d::PostDraw();
	for (int i = 0; i < axeSize; i++)
	{
		if (ImpactPar[i] == nullptr)
		{
			return;
		}
		ImpactPar[i]->Draw();
	}
}
