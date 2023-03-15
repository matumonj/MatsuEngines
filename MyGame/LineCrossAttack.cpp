#include "LineCrossAttack.h"

#include <algorithm>

#include"mHelper.h"
#include"BossMap.h"
#include <BossEnemy.h>

#include "CameraControl.h"
#include "EnemyControl.h"
#include "PlayerControl.h"

LineCrossAttack::LineCrossAttack()
{
	Texture::LoadTexture(30, L"Resources/2d/attackeffect/inpact.png");
	
}

LineCrossAttack::~LineCrossAttack()
{
	//delete KnockTex;
}

void LineCrossAttack::Finalize()
{
	for (int i = 0; i < axeSize; i++)
	{
		DamageTex[i].reset(nullptr);
		ImpactPar[i].reset(nullptr);
	}
}

LineCrossAttack* LineCrossAttack::GetIns()
{
	static LineCrossAttack instance;
	return &instance;
}

void LineCrossAttack::Init()
{
	Texture::LoadTexture(35, L"Resources/2d/gauge/bosshp.png");
	AxePos[0] = { 50.f, 200.f, 50.f };
	AxePos[1] = { -50.f, 200.f, 50.f };
	AxePos[2] = { 50.f, 200.f, -50.f };
	AxePos[3] = { -50.f, 200.f, -50.f };
	DamageTex[0].reset(Texture::Create(35));
	DamageTex[1].reset(Texture::Create(35));
		DamageTex[0]->CreateTexture();
		DamageTex[1]->CreateTexture();

	for (int i = 0; i < axeSize; i++)
	{
		
	ImpactPar[i] = std::make_unique<Particle>();
		ImpactPar[i]->Init(64);
		SetPos[i] = AxePos[i];
	}

	AxePosDownEtime = 0.f;
	AttackCount = 0;
	axeDirectionTexAlpha = 0.f;
	_phase = PHASE_NON;
}

void LineCrossAttack::NonInit()
{
	_phase = PHASE_TWO;
}
void LineCrossAttack::SetDamageArea()
{
//	for (auto i = 0; i < axeSize; i++) {
		ImpactTexAlpha[0] = 1.f;
		ImpactTexAlpha[1] = 0.2f;
	//}
		AttackCount++;
		if (AttackCount > 90) {
		_phase = PHASE_TWO;
		}
}

void LineCrossAttack::CrossAttack()
{
	Enemy* enemy = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0].get();
	//if (enemy->GetAnimeState() != enemy->SWING)return;
	AttackCount = 0;
	for (auto i = 0; i < axeSize; i++) {
		ImpactTexAlpha[i] -= 0.02f;
		ImpactTexAlpha[i] = std::clamp(ImpactTexAlpha[i], 0.f, 1.f);

	}

	double EndTime= enemy->GetFbxTimeEnd();
	if(enemy->GetAnimationTime()>=EndTime-0.1)
	{
		_phase = PHASE_THREE;
	}
}

void LineCrossAttack::AttackEnd()
{
	for (auto i = 0; i < axeSize; i++) {
		ImpactTexAlpha[i] -= 0.02f;
		ImpactTexAlpha[i] = std::clamp(ImpactTexAlpha[i], 0.f, 1.f);

	}
}




void LineCrossAttack::Upda()
{

	Enemy* enemy = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0].get();
//	Enemy* enemy = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0].get();
	if (enemy->GetNowMotion() !=enemy->SWING)return;
	XMVECTOR positionA = {
	PlayerControl::GetIns()->GetPlayer()->GetPosition().x,
	PlayerControl::GetIns()->GetPlayer()->GetPosition().y,
	PlayerControl::GetIns()->GetPlayer()->GetPosition().z
	};
	XMVECTOR positionB = { enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z };

	if (enemy->GetAnimationTime() <= 0.6) {
		Add_RotVal = FollowRot::FollowA_B(positionA, positionB);

		//移動ベクトルをy軸周りの角度で回転
		move = { 0.0f, 0.0f, 0.1f, 0.0f };

		matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

		move = XMVector3TransformNormal(move, matRot);
	} else {
		if (enemy->GetAnimationTime() <= 1.3) {

			enemy->SetPosition({
					enemy->GetPosition().x + move.m128_f32[0] * 6.f,
					enemy->GetPosition().y,
					enemy->GetPosition().z + move.m128_f32[2] * 6.f
				}
			);

		}
	}
	//フェーズごとの処理
	(this->*actionTable[_phase])();


	for (int i = 0; i < axeSize; i++)
	{
		if (DamageTex[i] == nullptr)
		{
			continue;

		}
		AxeRot[i] = {
	90.f,
	Add_RotVal * 55.f,
	enemy->GetRotation().z

		};
	
	}
DamageTex[0]->SetUVMove(true);
	for (int i = 0; i < axeSize; i++)
	{
		if (DamageTex[i] == nullptr)
		{
			continue;
		}
		DamageTex[i]->SetRotation(AxeRot[0]);

		DamageTex[i]->SetPosition({ enemy->GetPosition().x,enemy->GetPosition().y+4.f,enemy->GetPosition().z });
		DamageTex[i]->SetScale({ 3.f, 8.f, 8.f });
		DamageTex[i]->SetColor({ 1.f,1.f,1.f,ImpactTexAlpha[i] });
		DamageTex[i]->SetAnchorPoint({ 0.5f,0.f });
		DamageTex[i]->Update(CameraControl::GetIns()->GetCamera());
		DamageTex[i]->SetBillboard(false);
	}
}

#include"imgui.h"

void (LineCrossAttack::* LineCrossAttack::actionTable[])() = {
	&LineCrossAttack::NonInit, //待機
	& LineCrossAttack::SetDamageArea, //エリア指定
	& LineCrossAttack::CrossAttack, //
	& LineCrossAttack::AttackEnd //終了
};

void LineCrossAttack::Draw()
{
Texture::PreDraw();
	for (int i = 0; i < axeSize; i++)
	{
		if (DamageTex[i] == nullptr)
		{
			continue;
		}
		DamageTex[i]->Draw();
	}
	Texture::PostDraw();
	for (int i = 0; i < axeSize; i++)
	{
		if (ImpactPar[i] == nullptr)
		{
			return;
		}
		ImpactPar[i]->Draw();
	}
}
