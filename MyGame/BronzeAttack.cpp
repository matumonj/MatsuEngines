#include "BronzeAttack.h"
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
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	Texture::LoadTexture(103, L"Resources/2d/attackEffect/ma1b-skyblue.png");
	//テクスチャセット
	Texture* l_tex = Texture::Create(103);
	MagicTex.reset(l_tex);
	MagicTex->CreateTexture();
	MagicTex->SetAnchorPoint({ 0.5f, 0.5f });

	BeamObj = std::make_unique<Object3d>();
	BeamObj->Initialize(camera);
	BeamObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BEAM));
	//初期化
	TexAlpha = 0.0f;
	TexScl = { 0.0f, 0.f };

	scalingETime = 0.0f;
}

void BronzeAttack::Upda()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	if (Input::GetInstance()->TriggerButton(Input::Y))
	{
		phase = AREASET;
	}
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
		//PlayerControl::GetInstance()->GetPlayer()->RecvDamage(50);
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
	MagicTex->SetScale({ TexScl.x, TexScl.y, 1.f });
	MagicTex->SetPosition({TexPos.x,20.f,TexPos.y });
	TexRotZ++;
	MagicTex->SetRotation({ 0.f, 0.f, TexRotZ });
	MagicTex->SetColor({ 1.f, 1.f, 1.f, TexAlpha });
	MagicTex->SetBillboard(false);
	MagicTex->Update(CameraControl::GetInstance()->GetCamera());

	BeamObjScl.y = 50.0f;
	BeamObj->SetUVf(true);
	BeamObj->SetColor({ 0.7f, 0.7f, 0.7f, 0.5f });
	BeamObj->SetRotation({ 90.0f, 0.0f, 0.0f });
	BeamObj->SetScale(BeamObjScl);
	BeamObj->Update({ 1.f, 1.f, 1.f, 0.5f }, camera);

	TexAlpha = max(TexAlpha, 0.f);
}

void BronzeAttack::Phase_AreaSet()
{
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	bool nextPhase = TexAlpha > 3.f;
	const float scalingSpeed = 0.05f;
	const float maxScale = 9.f;
	if (TexAlpha == 0.f)
	{
	//	MagicTex->SetPosition({ ppos.x, -33.f, ppos.z });
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
}

void BronzeAttack::Phase_Bom()
{
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	scalingETime += 0.04f;
	if (scalingETime >= 2.5f)
	{
		phase = BEAMSMALL;
	}
	if (scalingETime < 1.0f)
	{
		BeamObjScl.x = Easing::EaseOut(scalingETime, 0.0f, 30.0f);
		BeamObjScl.z = Easing::EaseOut(scalingETime, 0.0f, 30.0f);
	}

	BeamObjScl.x = min(BeamObjScl.x, 30.0f);
	BeamObjScl.x = max(BeamObjScl.x, 0.0f);
	BeamObjScl.z = min(BeamObjScl.z, 30.0f);
	BeamObjScl.z = max(BeamObjScl.z, 0.0f);

	if (Collision::GetLength(ppos, BeamObj->GetPosition()) < 14.f)
	{
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(5);
	}
	BeamObj->SetPosition(MagicTex->GetPosition());
}

void BronzeAttack::Phase_MakeSmall()
{
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	if (Collision::GetLength(ppos, BeamObj->GetPosition()) < 14.f)
	{
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(5);
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

void BronzeAttack::Phase_TexFade()
{
	TexAlpha -= 0.02f;
	if (TexAlpha < 0.0f)
	{
		TexScl = { 0.0f, 0.0f };
		phase = END;
	}
}


void BronzeAttack::Phase_End()
{
	TexAlpha = 0.f;
	scalingETime = 0.0f;
	BeamObjScl = { 0.0f, 50.0f, 0.0f };
}

#include"imgui.h"

void BronzeAttack::Draw()
{
	

	Object3d::PreDraw();
	BeamObj->Draw();
	Object3d::PostDraw();

	Texture::PreDraw();
	MagicTex->Draw();
	Texture::PostDraw();
}
