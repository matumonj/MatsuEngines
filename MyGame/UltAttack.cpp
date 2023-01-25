#include "UltAttack.h"
#include"CameraControl.h"
#include"EnemyControl.h"
#include "Field.h"
#include"mHelper.h"
#include "PlayerControl.h"
#define PI 3.14f

UltAttack* UltAttack::GetIns()
{
	static UltAttack ins;
	return &ins;
}

void UltAttack::Init()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	Model* l_model = Model::CreateFromOBJ("BossBeam", true);
	for (int i = 0; i < 8; i++) {
		normalAttackObj[i] = std::make_unique<Object3d>();
		normalAttackObj[i]->Initialize(camera);
		normalAttackObj[i]->SetModel(l_model);

	scalingETime[i] = 0.0f;


	}
	
}

void UltAttack::Upda()
{
	//Field::GetInstance()->SetnextStageF(true);
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	switch (phase)
	{
	case NON:
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
	SceneBloom();
	for (int i = 0; i < 8; i++) {
		if (normalAttackObj[i] != nullptr) {
			normalAttackObj[i]->SetUVf(true);
			normalAttackObj[i]->SetColor({ 0.9f, 0.9f, 0.9f, 0.8f });
			normalAttackObj[i]->SetPosition(beampos[i]);
			normalAttackObj[i]->SetRotation({ 180.0f, 0.0f, 0.0f });
			normalAttackObj[i]->SetScale(beamscl[i]);
			normalAttackObj[i]->Update({ 1.f, 1.f, 1.f, 0.5f }, camera);
		}
	}
}

void UltAttack::Phase_AreaSet()
{
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	XMFLOAT3 bpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();

	bool nextPhase = TexAlpha > 3.f;
	const float scalingSpeed = 0.05f;
	const float maxScale = 5.f;
	
	//色の明度下げたり上げたり
	ColorT++;
	for(int i=0;i<8;i++)
	{
		beampos[i] = { bpos.x+float(rand()%80-40),0.f,bpos.z + float(rand() % 80 - 40) };
		
	}
	//if (nextPhase)
	//{
		phase = BOM;
	//}

}

void UltAttack::Phase_Bom()
{
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();

	beamscl[0].y += 1.f;
	for (int i = 1; i < 8; i++) {
		beamscl[i].x = 1;
		beamscl[i].z = 1;
		if (beamscl[i - 1].y > 40.f) {
			beamscl[i].y += 2.f;// Easing::EaseOut(scalingETime[i], 0.0f, 90.0f); ;
		}
	}
	if (beamscl[7].y > 80.f)
	{
		phase = BEAMSMALL;
	}
}
#include"Feed.h"
void UltAttack::Phase_MakeSmall()
{
	beamscl[0].z += 0.5f;
	beamscl[0].x += 0.5f;
	for (int i = 1; i < 8; i++) {
		scalingETime[i] += 0.04f;
		//画面真っ白になったらシーン切り替え
		
				if (beamscl[i - 1].x > 10.f) {
					beamscl[i].z += 0.5f;
					beamscl[i].x += 0.5f;// Easing::EaseOut(scalingETime[i], 0.0f, 90.0f); ;
				
			}
	}
	

}

void UltAttack::Phase_TexFade()
{
	Feed::GetInstance()->Update_White(Feed::FEEDOUT);
	TexAlpha -= 0.02f;
	for (int i = 0; i < 8; i++) {
		beamscl[i] = {0.f,0.f,0.f};
	}
	if (Feed::GetInstance()->GetAlpha() <=0.0f)
	{
		TexScl = { 0.0f, 0.0f };
		phase = END;
	}
}
void UltAttack::SceneBloom()
{
		if (beamscl[0].x > 20) {
			BloomAlpha -= 0.02f;
		}
		else{
			BloomAlpha = 0.8f;
	}
		if (BloomAlpha <0.2f) {

			fielddestf = true;
			Feed::GetInstance()->Update_White(Feed::FEEDIN);
		}
	if(phase==BEAMSMALL)
	{
		if (Feed::GetInstance()->GetAlpha() >= 1.0f)
		{
			BloomAlpha = 1.f;
			Field::GetInstance()->SetnextStageF(true);
				phase = TEXFADE;
		}
	}
}


void UltAttack::Phase_End()
{
//TexAlpha = 0.f;
	for (int i = 0; i < 8; i++) {
		beamscl[i] = { 0,0,0 };
		scalingETime[i] = 0.0f;
	}
}

#include"imgui.h"

void UltAttack::Draw()
{
	Object3d::PreDraw();
	for (int i = 0; i < 8; i++) {
		normalAttackObj[i]->Draw();
	}
	Object3d::PostDraw();
	
}
