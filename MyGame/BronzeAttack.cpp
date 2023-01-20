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
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	Texture::LoadTexture(103, L"Resources/2d/attackEffect/ma1b-skyblue.png");
	//テクスチャセット
	Texture* l_tex = Texture::Create(103);
	MagicTex.reset(l_tex);
	MagicTex->CreateTexture();
	MagicTex->SetAnchorPoint({ 0.5f, 0.5f });

	for (int i = 0; i < BeamObj.size(); i++) {
		BeamObj[i] = std::make_unique<Object3d>();
		BeamObj[i]->Initialize(camera);
		BeamObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BEAM2));


	}
	for(int i=0;i<chargesphere.size();i++)
	{
		chargesphere[i] = std::make_unique<Object3d>();
		chargesphere[i]->Initialize(camera);
		chargesphere[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SPHERE));
		chargesphere[i]->SetScale({ 1.f,1.f,1.f });
	}
	//初期化
	TexAlpha = 0.0f;
	TexScl = { 0.0f, 0.f };
	for (int i = 0; i < BeamObj.size(); i++) {
		scalingETime[i] = 0.0f;
	}
}

void BronzeAttack::Upda()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
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

	
	if (phase != END) {
		//各パラメータのセット
		MagicTex->SetScale({ TexScl.x, TexScl.y, 1.f });
		MagicTex->SetPosition({ TexPos.x,20.f,110.f });
		TexRotZ++;
		MagicTex->SetRotation({ 0.f, 0.f, TexRotZ });
		MagicTex->SetColor({ 1.f, 1.f, 1.f, TexAlpha });
		MagicTex->SetBillboard(false);
		MagicTex->Update(CameraControl::GetInstance()->GetCamera());

		for (int i = 0; i < BeamObj.size(); i++) {

			BeamObjScl[i].y = 1500.0f;
			BeamObj[i]->SetShadowF(false);
			BeamObj[i]->SetUVf(true);
			BeamObj[i]->SetColor({1.f,1.f,1.f, 1.f });
			BeamObj[i]->SetRotation({ 90.0f, 0.0f, 0.0f });
			BeamObj[i]->SetScale(BeamObjScl[i]);
			BeamObj[i]->Update({ 1.f, 1.f, 1.f, 0.5f }, camera);
		}
	}
	TexAlpha = max(TexAlpha, 0.f);
}

void BronzeAttack::SphereMoving()
{
	XMFLOAT3 bpos_right ={EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->HandRightPos()};
	XMFLOAT3 bpos_left = { EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->HandLeftPos() };
	for(int i=0;i<15;i++)
	{
		if(chargespheremoveF[i])
		{
			chargesphereangle[i]++;
			chargespherepos[i].z+=2.f;
			if(Collision::GetLength(chargespherepos[i],MagicTex->GetPosition())<15.f)
			{
				chargespheremoveF[i] = false;
			}
		}
		else
		{
			chargespherepos[i] = { bpos_right.x ,bpos_right.y, bpos_right.z};

			
			chargespheremoveF[i] = true;
			break;
		}
	}
	//
	for (int i = 15; i < chargesphere.size() ; i++)
	{
		if (chargespheremoveF[i])
		{
			chargesphereangle[i]++;
			chargespherepos[i].z-=2.f;
			if (Collision::GetLength(chargespherepos[i], { bpos_left.x,14.75f,-90.f}) < 15.f)
			{
				chargespheremoveF[i] = false;
			}
		} else
		{
			chargespherepos[i] = { bpos_left.x ,bpos_left.y , bpos_left.z };

			chargespheremoveF[i] = true;
			break;
		}
	}
	for(int i=0;i<chargesphere.size();i++)
	{
		chargesphere[i]->SetPosition(chargespherepos[i]);
		chargesphere[i]->SetColor({ 1.f,1.f,1.f,0.8f });
		chargesphere[i]->SetScale({ 1.f,1.f,1.f });
		chargesphere[i]->Update({ 1.f,1.f,1.f,1.f }, CameraControl::GetInstance()->GetCamera());
	}
}

void BronzeAttack::Phase_AreaSet()
{
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

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
	for (int i = 0; i < BeamObj.size(); i++) {
		BeamObj[i]->SetPosition({ MagicTex->GetPosition().x + sinf(float(i) * 360.f / 5.f * (PI / 180.0f)) * 10.0f,
		
			MagicTex->GetPosition().y + cosf(float(i) * 360.f/5.f*(PI / 180.0f)) * 10.0f,
			MagicTex->GetPosition().z });
	}
	if (nextPhase)
	{
		phase = BOM;
	}
}

void BronzeAttack::Phase_Bom()
{
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	for (int i = 0; i < BeamObj.size(); i++) {
		scalingETime[i] += 0.04f;
		if (scalingETime[i] >= 2.5f)
		{
			phase = BEAMSMALL;
		}
		if (scalingETime[i] < 1.0f)
		{
			BeamObjScl[i].x = Easing::EaseOut(scalingETime[i], 0.0f, 4.f);
			BeamObjScl[i].z = Easing::EaseOut(scalingETime[i], 0.0f, 4.f);
		}

		BeamObjScl[i].x = std::clamp(BeamObjScl[i].x, 0.f,4.f);
		BeamObjScl[i].z = std::clamp(BeamObjScl[i].z, 0.0f,4.f);

		if (Collision::GetLength(ppos, BeamObj[i]->GetPosition()) < 14.f)
		{
			PlayerControl::GetInstance()->GetPlayer()->RecvDamage(5);
		}
	}
}
void BronzeAttack::Phase_MakeSmall()
{
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	TexAlpha = 1.f;
	for (int i = 0; i < BeamObj.size(); i++) {
		scalingETime[i] -= 0.04f;
		
		BeamObjScl[i].x = Easing::EaseOut(scalingETime[i], 0.0f, 4.f);
		BeamObjScl[i].z = Easing::EaseOut(scalingETime[i], 0.0f, 4.f);

		BeamObjScl[i].x = std::clamp(BeamObjScl[i].x,0.f, 4.f);
		BeamObjScl[i].z = std::clamp(BeamObjScl[i].z, 0.f,4.f);
		
	}
	if (scalingETime[4] <= 0.0f)
	{
		phase = TEXFADE;
	}
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
	for (int i = 0; i < BeamObj.size(); i++) {
		scalingETime[i] = 0.0f;
		BeamObjScl[i] = {0.0f, 50.0f, 0.0f};
	}
}


#include"imgui.h"

void BronzeAttack::Draw()
{
	
	if (phase != END) {
		Object3d::PreDraw();
		for (int i = 0; i < BeamObj.size(); i++) {
			if (BeamObj[i] == nullptr)continue;
			BeamObj[i]->Draw();
		}
		if (phase == AREASET) {
			for (int i = 0; i < chargesphere.size(); i++)
			{
				chargesphere[i]->Draw();
			}
		}
		Object3d::PostDraw();

		Texture::PreDraw();
		MagicTex->Draw();
		Texture::PostDraw();
	}
}
