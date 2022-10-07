#include "SelectSword.h"
#include"BigSword.h"
#include"SmallSword.h"
#include"NormalSword.h"
#include"CameraControl.h"
#include"mHelper.h"
#include"ModelManager.h"
SelectSword::~SelectSword()
{
	delete SwordSample[0], SwordSample[1], SwordSample[2], Frame;
}

SelectSword* SelectSword::GetInstance()
{
	static SelectSword instance;
	return &instance;
}
void SelectSword::Finalize()
{
	delete SwordSample[0], SwordSample[1], SwordSample[2], Frame;
	Destroy_unique(PedestalObj);
	for (int i = 0; i < 3; i++) {
		Destroy_unique(SampleSword[i]);
	}
}

void SelectSword::SpriteSet()
{
	Sprite::LoadTexture(230, L"Resources/SwordA.png");
	Sprite::LoadTexture(231, L"Resources/swordB.png");
	Sprite::LoadTexture(232, L"Resources/swordC.png");

	Sprite::LoadTexture(233, L"Resources/SelSwordFrame.png");
	SwordSample[0] = Sprite::Create(230, { 1.0f,1.0f });
	SwordSample[1] = Sprite::Create(231, { 1.0f,1.0f });
	SwordSample[2] = Sprite::Create(232, { 1.0f,1.0f });
	Frame = Sprite::Create(233, { 1.0f,1.0f });
	for (int i = 0; i < 3; i++) {
		//フィールドにモデル割り当て
		//SwordSample[i]->Initialize();
		SwordSample[i]->SetSize({ 400.0f,400.0f });
		SwordSample[i]->SetAnchorPoint({ 0.5f,0.5f });
		SwordSample[i]->SetPosition({ Position[i] });
	}
	Frame->SetSize({ 450.0f,450.0f });
	Frame->SetPosition(Position[0]);
	Frame->SetAnchorPoint({ 0.5f,0.5f });
}

void SelectSword::pedestalSet()
{
	for (int i = 0; i < 3; i++) {
		SampleSword[i] = std::make_unique<Object3d>();
	}
	SampleSword[SwordScale::SMALL]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SMALLSWORD));
	SampleSword[SwordScale::SMALL]->Initialize(CameraControl::GetInstance()->GetCamera());
	sampleSwordRot[SwordScale::SMALL] = { -60.0f,100.0f,-90.0f };

	SampleSword[SwordScale::NORMAL]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NORMALSWORD));
	SampleSword[SwordScale::NORMAL]->Initialize(CameraControl::GetInstance()->GetCamera());
	sampleSwordRot[SwordScale::NORMAL] = { -60.0f,100.0f,-90.0f };

	SampleSword[SwordScale::BIG]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGSWORD));
	SampleSword[SwordScale::BIG]->Initialize(CameraControl::GetInstance()->GetCamera());
	sampleSwordRot[SwordScale::BIG] = { -60.0f,100.0f,-90.0f };

	PedestalObj = std::make_unique<Object3d>();
	PedestalObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::PEDESTAL));
	PedestalObj->Initialize(CameraControl::GetInstance()->GetCamera());

	RotAngle = 0.0f;
	RotationF = false;
	EaseTime = 0.0f;
}

void SelectSword::Initialize()
{
	Sword = std::make_unique<NormalSword>();
	Sword->Initialize(CameraControl::GetInstance()->GetCamera());
	
	SpriteSet();
	pedestalSet();
	input = Input::GetInstance();
	index = 0;
}

void SelectSword::SetSword(SwordScale nowsword)
{
	switch (nowsword)
	{
	case SelectSword::SMALL:
		Sword = std::make_unique<SmallSword>();
		break;
	case SelectSword::NORMAL:
		Sword = std::make_unique<NormalSword>();
		break;
	case SelectSword::BIG:
		Sword = std::make_unique<BigSword>();
		break;
	default:
		break;
	}
	Sword->Initialize(CameraControl::GetInstance()->GetCamera());
	
}

void SelectSword::Update()
{

	if (SelectJudg) {
		if (input->TriggerCrossKey(input->Cross_Right)) {
			index++;
		} else if (input->TriggerCrossKey(input->Cross_Left)) {
			index--;
		}

		if (index == SwordScale::SMALL) {
			NowSelectSword = SMALL;
		} else if (index == SwordScale::NORMAL) {
			NowSelectSword = NORMAL;
		} else if (index == SwordScale::BIG) {
			NowSelectSword = BIG;
		}
		if (input->TriggerButton(input->Button_A)) {
			SetSword(NowSelectSword);
		}
		Frame->SetPosition(Position[index]);
	}
	Sword->Update(CameraControl::GetInstance()->GetCamera());

	PedestalUpdate();

	index = min(index, 2);
	index = max(index, 0);
}

void SelectSword::PedestalUpdate()
{
	//台座座標
	const XMFLOAT3 PedestalPos = { CameraControl::GetInstance()->GetCamera()->GetEye().x+20.0f,
	CameraControl::GetInstance()->GetCamera()->GetEye().y-20.0f ,
	CameraControl::GetInstance()->GetCamera()->GetEye().z + 30.0f };
	//サンプル剣回転時の中心座標からの距離(半径)
	const float Radius = 3.0f;
	const float Height = 3.0f;
	//円回転するやつ
	//右回転
	if (input->TriggerCrossKey(input->Cross_Right)) {
		rotDir = RIGHT;
	}
	//左回転
	if (input->TriggerCrossKey(input->Cross_Left)) {
		rotDir = LEFT;
	}
	//イージングカウントリセット
	if (rotDir==NON) {
		EaseTime = 0.0f;
	}
	SwordRot();
	
	/*-----------剣の座標---------------------------------------------*/
	XMFLOAT3 samplePos_big= { PedestalPos.x + cosf((float)(RotAngle) * PI / 180.0f) * Radius,
	PedestalPos.y + Height,
	PedestalPos.z + sinf((float)(RotAngle) * PI / 180.0f) * Radius };
	
	XMFLOAT3 samplePos_normal = { PedestalPos.x + cosf((float)(RotAngle+120.0f)*PI / 180.0f) * Radius,
	PedestalPos.y + Height,
	PedestalPos.z + sinf((float)(RotAngle+120.0f)*PI / 180.0f) * Radius };

	XMFLOAT3 samplePos_small = { PedestalPos.x + cosf((float)(RotAngle+240.0f)*PI / 180.0f) * Radius,
		PedestalPos.y + Height,
		PedestalPos.z + sinf((float)(RotAngle+240.0f)*PI / 180.0f) * Radius };
	/*-------------------------------------------------------------*/
	SampleSword[SwordScale::SMALL]->SetPosition(samplePos_small);
	SampleSword[SwordScale::NORMAL]->SetPosition(samplePos_normal);
	SampleSword[SwordScale::BIG]->SetPosition(samplePos_big);

	for (int i = 0; i < 3; i++) {
		SampleSword[i]->SetRotation(sampleSwordRot[i]);
		SampleSword[i]->SetScale( {2.0f,2.0f,2.0f });
		SampleSword[i]->Update({ 1.0f,1.0f,1.0f,1.0f }, CameraControl::GetInstance()->GetCamera());
	}

	//現在選択中の剣くるくる回る
	switch (index)
	{
	case SwordScale::SMALL:
		sampleSwordRot[SwordScale::SMALL].y--;
	break;
	case SwordScale::NORMAL:
		sampleSwordRot[SwordScale::NORMAL].y--;
		break; 
	case SwordScale::BIG:
		sampleSwordRot[SwordScale::BIG].y--;
		break;
	default:
		break;
	}

	PedestalObj->SetScale({ 10.0f,10.0f,10.0f });
	PedestalObj->SetPosition(PedestalPos);
	PedestalObj->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	PedestalObj->Update({ 10.0f,10.0f,10.0f,10.0f }, CameraControl::GetInstance()->GetCamera());
}

void SelectSword::SwordRot()
{
	//イージングカウント
	const float EaseC = 0.05f;
	if (rotDir==RIGHT) {
			EaseTime += EaseC;
			//現在の角度から120度回転
			RotAngle = Easing::EaseOut(EaseTime, RotAngle_Old[index-1], RotAngle_Old[index]);
			if (EaseTime >= 1.0f) {
				rotDir = NON;
			}
		}
		else if (rotDir==LEFT) {
			EaseTime -= EaseC;
			//現在の角度から120度回転
			RotAngle = Easing::EaseOut(EaseTime, RotAngle_Old[index+1], RotAngle_Old[index]);
			if (EaseTime <= -1.0f) {
				rotDir = NON;
			}
	}

}
void SelectSword::Draw()
{
	SpriteDraw();
}

void SelectSword::SpriteDraw()
{
	if (SelectJudg) {
		Sprite::PreDraw();

		for (int i = 0; i < 3; i++) {
			SwordSample[i]->Draw();
		}

		Frame->Draw();
		Sprite::PostDraw();
	}
}

void SelectSword::PedestalDraw()
{
	Object3d::PreDraw();
	PedestalObj->Draw();
	for (int i = 0; i < 3; i++) {
		SampleSword[i]->Draw();
	}
	Object3d::PostDraw();

}

void SelectSword::SwordDraw()
{
	Sword->Draw();
}

