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
	//�e�N�X�`���Z�b�g
	for (int i = 0; i < MagicTex.size(); i++) {
		MagicTex[i].reset(Texture::Create(103));
		MagicTex[i]->CreateTexture();
		MagicTex[i]->SetAnchorPoint({0.5f, 0.5f});
	}

	for (int i = 0; i < BeamObj.size(); i++) {
		BeamObj[i] = std::make_unique<Object3d>();
		BeamObj[i]->Initialize(camera);
		BeamObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BEAM));


	}
	for(int i=0;i<chargesphere.size();i++)
	{
		chargesphere[i] = std::make_unique<Object3d>();
		chargesphere[i]->Initialize(camera);
		chargesphere[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NAIL));
		chargesphere[i]->SetScale({ 1.f,1.f,1.f });
	}
	//������
	TexAlpha = 0.0f;
	TexScl = { 0.0f, 0.f };
	for (int i = 0; i < BeamObj.size(); i++) {
		scalingETime[i] = 0.0f;
	}
	ChargeCenter.reset(new Particle());
	ChargeCenter->Init(64);
}

void BronzeAttack::Upda()
{

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

	ObjUpda();
}
void BronzeAttack::ObjUpda()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	if (phase == END)return; 
		TexRotZ++;
		if (AttackDir == HEIGHT) {
			TexPos[0] = { 0.f,14.7f,110.f };
			TexPos[1] = { 0.f,14.7f,-110.f };

			MagicTex[0]->SetRotation({ 0.f, 0.f, TexRotZ });
			MagicTex[1]->SetRotation({ 0.f, 0.f, TexRotZ });


			for (int i = 0; i < BeamObj.size(); i++) {
				BeamObj[i]->SetRotation({ 90.0f, 0.0f, 0.0f });
			}
		} else if (AttackDir == WIDTH)
		{
			TexPos[0] = { 110.f,14.7f,0.f };
			TexPos[1] = { -110.f,14.7f,0.f };
			MagicTex[0]->SetRotation({ 0.f, 90.f, TexRotZ });
			MagicTex[1]->SetRotation({ 0.f, 90.f, TexRotZ });
			for (int i = 0; i < BeamObj.size(); i++) {
				BeamObj[i]->SetRotation({ 0.0f, 0.0f, 90.0f });
			}
		}

		//�e�p�����[�^�̃Z�b�g
		for (int i = 0; i < MagicTex.size(); i++) {
			MagicTex[i]->SetScale({ TexScl.x, TexScl.y, 1.f });
			MagicTex[i]->SetPosition({ TexPos[i] });
			MagicTex[i]->SetColor({ 1.f, 1.f, 1.f, TexAlpha });
			MagicTex[i]->SetBillboard(false);
			MagicTex[i]->Update(CameraControl::GetInstance()->GetCamera());
		}
		for (int i = 0; i < BeamObj.size(); i++) {

			BeamObjScl[i].y = 1500.0f;
			BeamObj[i]->SetShadowF(false);
			BeamObj[i]->SetUVf(true);
			BeamObj[i]->SetColor({ 1.f,1.f,1.f, 1.f });
			BeamObj[i]->SetScale(BeamObjScl[i]);
			BeamObj[i]->Update({ 1.f, 1.f, 1.f, 0.5f }, camera);
		}
	
	TexAlpha = max(TexAlpha, 0.f);
	XMFLOAT3 bpos = { EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition() };


	ChargeCenter->CreateParticle((phase == AREASET), { bpos });
	ChargeCenter->Upda();
}

void BronzeAttack::SphereMoving()
{

	XMFLOAT3 bpos ={EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition()};
	for(int i=0;i<15;i++)
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
				chargesphere[i]->SetRotation({ 0.f,0.f,90.f });
			} else
			{
				chargesphere[i]->SetRotation({ 0.f,90.f,-90.f });
			}
		}
		else
		{
			chargespherepos[i] = { bpos.x ,bpos.y, bpos.z};
			chargesphereMovingE[i] = 0.f;
			
			chargespheremoveF[i] = true;
			break;
		}
	}
	//
	for (int i = 15; i < chargesphere.size() ; i++)
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
				chargesphere[i]->SetRotation({ 0.f,0.f,-90.f });
			} else
			{
				chargesphere[i]->SetRotation({ 0.f,180.f,90.f });
			}
		} else
		{
			chargespherepos[i] = { bpos.x ,bpos.y , bpos.z };
			chargesphereMovingE[i] = 0.f;
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
	//�e�N�X�`���g��
	TexScl.x += scalingSpeed;
	TexScl.y += scalingSpeed;
	TexAlpha += 0.02f;

	//�F�̖��x��������グ����
	ColorT++;

	TexScl.x = min(TexScl.x, maxScale);
	TexScl.y = min(TexScl.y, maxScale);
	for (int i = 0; i < BeamObj.size(); i++) {
		BeamObj[i]->SetPosition({ MagicTex[0]->GetPosition().x + sinf(float(i) * 360.f / 5.f * (PI / 180.0f)) * 30.0f,
		
			MagicTex[0]->GetPosition().y + cosf(float(i) * 360.f / 5.f * (PI / 180.0f)) * 30.0f,
			MagicTex[0]->GetPosition().z});
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
			BeamObjScl[i].x = Easing::EaseOut(scalingETime[i], 0.0f, 6.f);
			BeamObjScl[i].z = Easing::EaseOut(scalingETime[i], 0.0f, 6.f);
		}

		BeamObjScl[i].x = std::clamp(BeamObjScl[i].x, 0.f,6.f);
		BeamObjScl[i].z = std::clamp(BeamObjScl[i].z, 0.0f,6.f);

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
		
		BeamObjScl[i].x = Easing::EaseOut(scalingETime[i], 0.0f, 6.f);
		BeamObjScl[i].z = Easing::EaseOut(scalingETime[i], 0.0f, 6.f);

		BeamObjScl[i].x = std::clamp(BeamObjScl[i].x,0.f, 6.f);
		BeamObjScl[i].z = std::clamp(BeamObjScl[i].z, 0.f,6.f);
		
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
	
	if (phase == END) return;
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
		MagicTex[0]->Draw();
		MagicTex[1]->Draw();
		Texture::PostDraw();

		ChargeCenter->Draw();
	
}