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
	DebugCamera*camera= CameraControl::GetInstance()->GetCamera();
	Texture::LoadTexture(70, L"Resources/2d/icon/enemyicon.png");
	Texture::LoadTexture(66, L"Resources/2d/attackEffect/missileeffect.png");

	//テクスチャセット
	Texture* l_tex = Texture::Create(70);
	DamageTex.reset(l_tex);
	DamageTex->CreateTexture();
	DamageTex->SetAnchorPoint({ 0.5f, 0.5f });

	normalAttackObj = std::make_unique<Object3d>();
	normalAttackObj->Initialize(camera);
	normalAttackObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BEAM));
	//初期化
	TexAlpha = 0.0f;
	TexScl = { 0.0f, 0.f };

	scalingETime = 0.0f;
}

void GuardianNAttack::Upda()
{

	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	switch (phase)
	{
	case NON:
		TexScl = { 0.f, 0.f };
		TexAlpha = 0.0f;
		ColorT = 0.0f;
		break;

	case AREASET:
		Phase_AreaSet();
		break;

	case BOM:
		Phase_Bom();
		break;

	case BEAMSMALL:
		Phase_MakeSmall();
		break;
	case END:
		Phase_End();
		break;
	}

	//各パラメータのセット
	DamageTex->SetScale({ TexScl.x, TexScl.y, 1.f });
	DamageTex->SetRotation({ 90.f, 0.f, 0.f });
	DamageTex->SetColor({ 1.f, 1.f, 1.f, 1.f });
	DamageTex->SetBillboard(false);
	DamageTex->Update(CameraControl::GetInstance()->GetCamera());

	BeamObjScl.y = 50.0f;
	normalAttackObj->SetUVf(true);
	normalAttackObj->SetColor({ 0.7f,0.7f,0.7f,0.5f });
	normalAttackObj->SetRotation({ 180.0f,0.0f,0.0f });
	normalAttackObj->SetScale(BeamObjScl);
	normalAttackObj->Update({ 1.f,1.f,1.f,0.5f }, camera);
	TexAlpha = max(TexAlpha, 0.f);
}

void GuardianNAttack::Phase_AreaSet()
{
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	bool nextPhase = TexAlpha > 3.f;
	const float scalingSpeed = 0.05f;
	const float maxScale = 8.f;

	//テクスチャ拡大
	TexScl.x += scalingSpeed;
	TexScl.y += scalingSpeed;
	TexAlpha += 0.02f;

	//色の明度下げたり上げたり
	ColorT++;

	TexScl.x = min(TexScl.x, maxScale);
	TexScl.y = min(TexScl.y, maxScale);

	DamageTex->SetPosition({ ppos.x, -33.f, ppos.z });
	if (nextPhase)
	{
		phase = BOM;
	}

	EnemyControl::GetInstance()->GetGuardianEnemy()->SetColor({ BossColor.x, BossColor.y, BossColor.z, 1.0f });
}

void GuardianNAttack::Phase_Bom()
{
	TexAlpha = 0.0f;
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();
	scalingETime += 0.04f;
	if(scalingETime>=2.5f)
	{
		phase = BEAMSMALL;
	}
	if (scalingETime < 1.0f) {
		BeamObjScl.x = Easing::EaseOut(scalingETime, 0.0f, 10.0f);
		BeamObjScl.z = Easing::EaseOut(scalingETime, 0.0f, 10.0f);
	}

	BeamObjScl.x = min(BeamObjScl.x, 10.0f);
	BeamObjScl.x = max(BeamObjScl.x, 0.0f);
	BeamObjScl.z = min(BeamObjScl.z, 10.0f);
	BeamObjScl.z= max(BeamObjScl.z,  0.0f);

	normalAttackObj->SetPosition(DamageTex->GetPosition());
}

void GuardianNAttack::Phase_MakeSmall()
{
	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();

	scalingETime -= 0.04f;
	if (scalingETime <=0.0f)
	{
		phase = END;
	}
	BeamObjScl.x = Easing::EaseOut(scalingETime, 0.0f, 10.0f);
	BeamObjScl.z = Easing::EaseOut(scalingETime, 0.0f, 10.0f);


	BeamObjScl.x = min(BeamObjScl.x, 10.0f);
	BeamObjScl.x = max(BeamObjScl.x, 0.0f);
	BeamObjScl.z = min(BeamObjScl.z, 10.0f);
	BeamObjScl.z = max(BeamObjScl.z,0.0f);
	
}


void GuardianNAttack::Phase_End()
{
	EnemyControl::GetInstance()->GetGuardianEnemy()->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	TexScl = { 0.0f,0.0f };
	scalingETime = 0.0f;
	BeamObjScl = { 0.0f,50.0f,0.0f };
}

void GuardianNAttack::Draw()
{
	Texture::PreDraw();
	DamageTex->Draw();
	Texture::PostDraw();

	Object3d::PreDraw();
	normalAttackObj->Draw();
	Object3d::PostDraw();
}
