#include "GuardianNAttack.h"
#include "GuardianNAttack.h"
#include"CameraControl.h"
#include"EnemyControl.h"
#include"mHelper.h"
#include "PlayerControl.h"
#define PI 3.14f

GuardianNAttack* GuardianNAttack::GetIns()
{
	static GuardianNAttack ins;
	return &ins;
}

void GuardianNAttack::TexSet()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	Texture::LoadTexture(70, L"Resources/2d/icon/enemyicon.png");
	Texture::LoadTexture(66, L"Resources/2d/attackEffect/missileeffect.png");

	//テクスチャセット
	Texture* l_tex = Texture::Create(70);
	DamageTex.reset(l_tex);
	DamageTex->CreateTexture();
	DamageTex->SetAnchorPoint({0.5f, 0.5f});

	normalAttackObj = std::make_unique<Object3d>();
	normalAttackObj->Initialize(camera);
	normalAttackObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BEAM));
	//初期化
	TexAlpha = 0.0f;
	TexScl = {0.0f, 0.f};

	scalingETime = 0.0f;
}

void GuardianNAttack::Upda()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	XMFLOAT3 epos = EnemyControl::GetIns()->GetGuardianEnemy()->GetPosition();
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	switch (phase)
	{
	case NON:
		TexScl = {0.f, 0.f};
		TexAlpha = 0.0f;
		ColorT = 0.0f;
		break;

	case AREASET:
		Phase_AreaSet();
		break;

	case BOM:
		//PlayerControl::GetIns()->GetPlayer()->RecvDamage(50);
		Phase_Bom();
		break;

	case BEAMSMALL:
		Phase_MakeSmall();
		break;
	case TEXFADE:
		Phase_TexFade();
		break;
	case END:
		Phase_End();
		break;
	}

	//各パラメータのセット
	DamageTex->SetScale({TexScl.x, TexScl.y, 1.f});
	TexRotZ++;
	DamageTex->SetRotation({90.f, 0.f, TexRotZ});
	DamageTex->SetColor({1.f, 1.f, 1.f, TexAlpha});
	DamageTex->SetBillboard(false);
	DamageTex->Update(CameraControl::GetIns()->GetCamera());

	BeamObjScl.y = 50.0f;
	normalAttackObj->SetUVf(true);
	normalAttackObj->SetColor({0.7f, 0.7f, 0.7f, 0.5f});
	normalAttackObj->SetRotation({180.0f, 0.0f, 0.0f});
	normalAttackObj->SetScale(BeamObjScl);
	normalAttackObj->Update(camera);

	TexAlpha = max(TexAlpha, 0.f);
}

void GuardianNAttack::Phase_AreaSet()
{
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	bool nextPhase = TexAlpha > 3.f;
	const float scalingSpeed = 0.05f;
	const float maxScale = 5.f;
	if (TexAlpha == 0.f)
	{
		DamageTex->SetPosition({ppos.x, -33.f, ppos.z});
	}

	//テクスチャ拡大
	TexScl.x += scalingSpeed;
	TexScl.y += scalingSpeed;
	TexAlpha += 0.02f;

	//色の明度下げたり上げたり
	ColorT++;

	TexScl.x = min(TexScl.x, maxScale);
	TexScl.y = min(TexScl.y, maxScale);

	if (nextPhase)
	{
		phase = BOM;
	}

	EnemyControl::GetIns()->GetGuardianEnemy()->SetColors({BossColor.x, BossColor.y, BossColor.z, 1.0f});
}

void GuardianNAttack::Phase_Bom()
{
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	XMFLOAT3 epos = EnemyControl::GetIns()->GetGuardianEnemy()->GetPosition();
	scalingETime += 0.04f;
	if (scalingETime >= 2.5f)
	{
		phase = BEAMSMALL;
	}
	if (scalingETime < 1.0f)
	{
		BeamObjScl.x = Easing::EaseOut(scalingETime, 0.0f, 10.0f);
		BeamObjScl.z = Easing::EaseOut(scalingETime, 0.0f, 10.0f);
	}

	BeamObjScl.x = min(BeamObjScl.x, 10.0f);
	BeamObjScl.x = max(BeamObjScl.x, 0.0f);
	BeamObjScl.z = min(BeamObjScl.z, 10.0f);
	BeamObjScl.z = max(BeamObjScl.z, 0.0f);

	if (Collision::GetLength(ppos, normalAttackObj->GetPosition()) < 14.f)
	{
		PlayerControl::GetIns()->GetPlayer()->RecvDamage(5);
	}
	normalAttackObj->SetPosition(DamageTex->GetPosition());
}

void GuardianNAttack::Phase_MakeSmall()
{
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	XMFLOAT3 epos = EnemyControl::GetIns()->GetGuardianEnemy()->GetPosition();
	if (Collision::GetLength(ppos, normalAttackObj->GetPosition()) < 14.f)
	{
		PlayerControl::GetIns()->GetPlayer()->RecvDamage(5);
	}
	TexAlpha = 1.f;
	scalingETime -= 0.04f;
	if (scalingETime <= 0.0f)
	{
		phase = TEXFADE;
	}
	BeamObjScl.x = Easing::EaseOut(scalingETime, 0.0f, 10.0f);
	BeamObjScl.z = Easing::EaseOut(scalingETime, 0.0f, 10.0f);

	BeamObjScl.x = min(BeamObjScl.x, 10.0f);
	BeamObjScl.z = min(BeamObjScl.z, 10.0f);

	BeamObjScl.x = max(BeamObjScl.x, 0.0f);
	BeamObjScl.z = max(BeamObjScl.z, 0.0f);
}

void GuardianNAttack::Phase_TexFade()
{
	TexAlpha -= 0.02f;
	if (TexAlpha < 0.0f)
	{
		TexScl = {0.0f, 0.0f};
		phase = END;
	}
}


void GuardianNAttack::Phase_End()
{
	EnemyControl::GetIns()->GetGuardianEnemy()->SetColors({1.0f, 1.0f, 1.0f, 1.0f});
	TexAlpha = 0.f;
	scalingETime = 0.0f;
	BeamObjScl = {0.0f, 50.0f, 0.0f};
}

#include"imgui.h"

void GuardianNAttack::Draw()
{
	ImGui::Begin("scl");
	ImGui::SliderFloat("x", &BeamMaxScl.x, 0.f, 6.f);
	ImGui::SliderFloat("x", &BeamMaxScl.z, 0.f, 6.f);
	ImGui::End();

	Object3d::PreDraw();
	normalAttackObj->Draw();
	Object3d::PostDraw();

	Texture::PreDraw();
	DamageTex->Draw();
	Texture::PostDraw();
}
