#include "BronzeAttack.h"

#include <algorithm>

#include"CameraControl.h"
#include"EnemyControl.h"
#include"mHelper.h"
#include "PlayerControl.h"
#define PI 3.14f

BronzeAttack* BronzeAttack::GetIns()
{
	static BronzeAttack ins;
	return &ins;
}

void BronzeAttack::Init()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	Texture::LoadTexture(103, L"Resources/2d/attackEffect/ma1b-skyblue.png");
	//テクスチャセット
	for (int i = 0; i < MagicTex.size(); i++)
	{
		MagicTex[i].reset(Texture::Create(103));
		MagicTex[i]->CreateTexture();
		MagicTex[i]->SetAnchorPoint({0.5f, 0.5f});
	}

	for (int i = 0; i < BeamObj.size(); i++)
	{
		BeamObj[i] = std::make_unique<Object3d>();
		BeamObj[i]->Initialize(camera);
		BeamObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BEAM));
	}
	for (int i = 0; i < chargesphere.size(); i++)
	{
		chargesphere[i] = std::make_unique<Object3d>();
		chargesphere[i]->Initialize(camera);
		chargesphere[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NAIL));
		chargesphere[i]->SetScale({1.f, 1.f, 1.f});
	}
	//初期化
	TexAlpha = 0.0f;
	TexScl = {0.0f, 0.f};
	for (int i = 0; i < BeamObj.size(); i++)
	{
		scalingETime[i] = 0.0f;
	}
	ChargeCenter.reset(new Particle());
	ChargeCenter->Init(64);
}

void BronzeAttack::Upda()
{
	switch (_phase)
	{
	case PHASE_NON:
		TexScl = {0.f, 0.f};
		TexAlpha = 0.0f;
		ColorT = 0.0f;
		break;

	case PHASE_ONE:
		Phase_AreaSet();
		break;

	case PHASE_TWO:
		//PlayerControl::GetIns()->GetPlayer()->RecvDamage(50);
		Phase_Bom();
		break;

	case PHASE_THREE:
		Phase_MakeSmall();
		break;
	case PHASE_FOUR:
		Phase_TexFade();
		break;
	}

	ObjUpda();
}

void BronzeAttack::ObjUpda()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	if (_phase == PHASE_FOUR && TexAlpha <= 0.f)
	{
		return;
	}
	TexRotZ++;
	if (AttackDir == HEIGHT)
	{
		TexPos[0] = {0.f, 14.7f, 110.f};
		TexPos[1] = {0.f, 14.7f, -110.f};

		MagicTex[0]->SetRotation({0.f, 0.f, TexRotZ});
		MagicTex[1]->SetRotation({0.f, 0.f, TexRotZ});


		for (int i = 0; i < BeamObj.size(); i++)
		{
			BeamObj[i]->SetRotation({90.0f, 0.0f, 0.0f});
		}
	}
	else if (AttackDir == WIDTH)
	{
		TexPos[0] = {110.f, 14.7f, 0.f};
		TexPos[1] = {-110.f, 14.7f, 0.f};
		MagicTex[0]->SetRotation({0.f, 90.f, TexRotZ});
		MagicTex[1]->SetRotation({0.f, 90.f, TexRotZ});
		for (int i = 0; i < BeamObj.size(); i++)
		{
			BeamObj[i]->SetRotation({0.0f, 0.0f, 90.0f});
		}
	}

	//各パラメータのセット
	for (int i = 0; i < MagicTex.size(); i++)
	{
		MagicTex[i]->SetScale({TexScl.x, TexScl.y, 1.f});
		MagicTex[i]->SetPosition({TexPos[i]});
		MagicTex[i]->SetColor({1.f, 1.f, 1.f, TexAlpha});
		MagicTex[i]->SetBillboard(false);
		MagicTex[i]->Update(CameraControl::GetIns()->GetCamera());
	}
	for (int i = 0; i < BeamObj.size(); i++)
	{
		BeamObjScl[i].y = 1500.0f;
		BeamObj[i]->SetShadowF(false);
		BeamObj[i]->SetUVf(true);
		BeamObj[i]->SetColor({1.f, 1.f, 1.f, 1.f});
		BeamObj[i]->SetScale(BeamObjScl[i]);
		BeamObj[i]->Update(camera);
	}

	TexAlpha = max(TexAlpha, 0.f);
	XMFLOAT3 bpos = {EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition()};


	ChargeCenter->CreateParticle((_phase == PHASE_ONE), {bpos});
	ChargeCenter->Upda();
}

void BronzeAttack::SetAction(bool f, Direction dir)
{
	if (_phase != PHASE_ONE)
	{
		_phase = PHASE_ONE;
		AttackDir = dir;
	}
}

void BronzeAttack::SphereMoving()
{
	XMFLOAT3 bpos = {EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition()};
	for (int i = 0; i < 6; i++)
	{
		if (chargespheremoveF[i])
		{
			chargesphereMovingE[i] += 0.05f;
			chargespherepos[i].x = Easing::EaseOut(chargesphereMovingE[i], bpos.x, TexPos[0].x);
			chargespherepos[i].z = Easing::EaseOut(chargesphereMovingE[i], bpos.z, TexPos[0].z);

			if (chargesphereMovingE[i] > 1.f)
			{
				chargespheremoveF[i] = false;
			}
			if (AttackDir == WIDTH)
			{
				chargesphere[i]->SetRotation({0.f, 0.f, 90.f});
			}
			else
			{
				chargesphere[i]->SetRotation({0.f, 90.f, -90.f});
			}
		}
		else
		{
			chargespherepos[i] = {bpos.x, bpos.y, bpos.z};
			chargesphereMovingE[i] = 0.f;

			chargespheremoveF[i] = true;
			break;
		}
	}
	//
	for (int i = 6; i < chargesphere.size(); i++)
	{
		if (chargespheremoveF[i])
		{
			chargesphereMovingE[i] += 0.05f;
			chargespherepos[i].x = Easing::EaseOut(chargesphereMovingE[i], bpos.x, TexPos[1].x);
			chargespherepos[i].z = Easing::EaseOut(chargesphereMovingE[i], bpos.z, TexPos[1].z);
			if (chargesphereMovingE[i] > 1.f)
			{
				chargespheremoveF[i] = false;
			}
			if (AttackDir == WIDTH)
			{
				chargesphere[i]->SetRotation({0.f, 0.f, -90.f});
			}
			else
			{
				chargesphere[i]->SetRotation({0.f, 180.f, 90.f});
			}
		}
		else
		{
			chargespherepos[i] = {bpos.x, bpos.y, bpos.z};
			chargesphereMovingE[i] = 0.f;
			chargespheremoveF[i] = true;
			break;
		}
	}
	for (int i = 0; i < chargesphere.size(); i++)
	{
		chargesphere[i]->SetPosition(chargespherepos[i]);
		chargesphere[i]->SetColor({1.f, 1.f, 1.f, 0.8f});
		chargesphere[i]->SetScale({1.f, 1.f, 1.f});
		chargesphere[i]->Update(CameraControl::GetIns()->GetCamera());
	}
}

void BronzeAttack::Phase_AreaSet()
{
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	bool nextPhase = TexAlpha > 9.f;
	const float scalingSpeed = 0.05f;
	const float maxScale = 9.f;
	if (TexAlpha == 0.f)
	{
		//	MagicTex->SetPosition({ ppos.x, -33.f, ppos.z });
	}

	SphereMoving();
	//テクスチャ拡大
	TexScl.x += scalingSpeed;
	TexScl.y += scalingSpeed;
	TexAlpha += 0.02f;

	//色の明度下げたり上げたり
	ColorT++;

	TexScl.x = min(TexScl.x, maxScale);
	TexScl.y = min(TexScl.y, maxScale);
	for (int i = 0; i < BeamObj.size(); i++)
	{
		BeamObj[i]->SetPosition({
			MagicTex[0]->GetPosition().x + sinf(static_cast<float>(i) * 360.f / 5.f * (PI / 180.0f)) * 30.0f,

			MagicTex[0]->GetPosition().y + cosf(static_cast<float>(i) * 360.f / 5.f * (PI / 180.0f)) * 30.0f,
			MagicTex[0]->GetPosition().z
		});
	}
	if (nextPhase)
	{
		_phase = PHASE_TWO;
	}
}

void BronzeAttack::Phase_Bom()
{
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	for (int i = 0; i < BeamObj.size(); i++)
	{
		scalingETime[i] +=l_EaseSpeed;
		if (scalingETime[i] >= l_maxeaseCount)
		{
			_phase = PHASE_THREE;
		}
		if (scalingETime[i] < 1.0f)
		{
			BeamObjScl[i].x = Easing::EaseOut(scalingETime[i], l_minScl, l_maxScl);
			BeamObjScl[i].z = Easing::EaseOut(scalingETime[i], l_minScl, l_maxScl);
		}

		BeamObjScl[i].x = std::clamp(BeamObjScl[i].x, l_minScl, l_maxScl);
		BeamObjScl[i].z = std::clamp(BeamObjScl[i].z, l_minScl, l_maxScl);

		if (Collision::GetLength(ppos, BeamObj[i]->GetPosition()) < 14.f)
		{
			PlayerControl::GetIns()->GetPlayer()->RecvDamage(5);
		}
	}
}

void BronzeAttack::Phase_MakeSmall()
{
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	TexAlpha = 1.f;

	for (int i = 0; i < BeamObj.size(); i++)
	{
		scalingETime[i] -= l_EaseSpeed;
		BeamObjScl[i].x = Easing::EaseOut(scalingETime[i], l_minScl, l_maxScl);
		BeamObjScl[i].z = Easing::EaseOut(scalingETime[i], l_minScl, l_maxScl);

		BeamObjScl[i].x = std::clamp(BeamObjScl[i].x, 0.f, l_maxScl);
		BeamObjScl[i].z = std::clamp(BeamObjScl[i].z, l_minScl, l_maxScl);
	}
	if (scalingETime[4] <= 0.0f)
	{
		_phase = PHASE_FOUR;
	}
}

void BronzeAttack::Phase_TexFade()
{
	//初期化諸々
	TexAlpha -= 0.02f;

	bool l_initJudg = TexAlpha < 0.f;

	if (l_initJudg)
	{
		TexScl = {0.0f, 0.0f};
		TexAlpha = 0.f;
		for (int i = 0; i < BeamObj.size(); i++)
		{
			scalingETime[i] = 0.0f;
			BeamObjScl[i] = {0.0f, 50.0f, 0.0f};
		}
	}
}


void BronzeAttack::Phase_End()
{
	TexAlpha = 0.f;
	for (int i = 0; i < BeamObj.size(); i++)
	{
		scalingETime[i] = 0.0f;
		BeamObjScl[i] = {0.0f, 50.0f, 0.0f};
	}
}


#include"imgui.h"

void BronzeAttack::Draw()
{
	if (_phase == PHASE_FOUR && TexAlpha <= 0.f)
	{
		return;
	}
	Object3d::PreDraw();
	for (int i = 0; i < BeamObj.size(); i++)
	{
		if (BeamObj[i] == nullptr)
		{
			continue;
		}
		BeamObj[i]->Draw();
	}
	if (_phase == PHASE_ONE)
	{
		for (int i = 0; i < chargesphere.size(); i++)
		{
			chargesphere[i]->Draw();
		}
	}
	Object3d::PostDraw();

	Texture::PreDraw();
	MagicTex[0]->Draw();
	MagicTex[1]->Draw();
	Texture::PostDraw();

	ChargeCenter->Draw();
}
