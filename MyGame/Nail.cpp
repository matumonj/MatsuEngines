#include "Nail.h"
#include"CameraControl.h"
#include"Input.h"
#include"ModelManager.h"
Nail::~Nail()
{
	//delete Nailmodel;
}
Nail* Nail::GetInstance()
{
	static Nail instance;
	return &instance;
}
void Nail::HalfAttack(HalfAttackArea area)
{
	if (HAttack.fase == NON && !HAttack.EndAction) {
		HAttack.fase = FASE_ONE;
	}
	switch (HAttack.fase)
	{
	case FASE_ONE:
		
		NailAmountSet(20);
		for (int i = 0; i < Nails.size(); i++) {
			HAttack.ZN.resize(Nails.size());
			HAttack.XN.resize(Nails.size());

			HAttack.ZN[i] = rand() % (40 + 1 + 60) - 60;

			if (area == RIGHT) {
				HAttack.XN[i] = rand() % 40;
			} else if (area == LEFT) {
				HAttack.XN[i] = rand() % 40 - 40;
			}
			Nails[i]->SetScale({ 1.0f,3.0f,1.0f });
			Nails[i]->SetPosition({ (float)HAttack.XN[i],MinY,(float)HAttack.ZN[i] });
		}
		HAttack.fase = FASE_TWO;
		break;
	case FASE_TWO:
		if (MinY <= -10.0f) {
			MinY += 1.0f;
		} else {
			HAttack.fase = FASE_THREE;
		}
		for (int i = 0; i < Nails.size(); i++) {
			Nails[i]->SetPosition({ Nails[i]->GetPosition().x ,MinY,Nails[i]->GetPosition().z });
		}

		HAttack.WaitCount = 0;
		break;
	case FASE_THREE:
		HAttack.WaitCount++;
		if (HAttack.WaitCount >= 60) {
			MinY -= 1.0f;
		}
		if (MinY <= -30.0f) {
			HAttack.EndAction = true;
			HAttack.fase = NON;
		}
		for (int i = 0; i < Nails.size(); i++) {
			Nails[i]->SetPosition({ Nails[i]->GetPosition().x ,MinY,Nails[i]->GetPosition().z });
		}
		break;

	case NON:
		HAttack.WaitCount = 0;
		//HAttack.EndAction = false;
		break;
	default:
		break;
	}
	for (int i = 0; i < Nails.size(); i++) {
		Nails[i]->SetRotation({ 180.0f,0.0f,0.0f });
		Nails[i]->Update({ 1.0f,1.0f,1.0f,1.0f }, CameraControl::GetInstance()->GetCamera());
	}

}
void Nail::CircleAttack(int area1, int area2)
{
	//東西南北のAOE中心場所　参照するなりした方がいい
	XMFLOAT3 DirectionPos[4] = { { 0.0f,-18.0f,30.0f }, { 0.0f,-18.0f,-30.0f }, { 30.0f,-18.0f,0.0f }, { -30.0f,-18.0f,0.0f } };//3
	if (CAttack.fase == NON && !CAttack.EndAction) {
		CAttack.fase = FASE_ONE;
	}

	switch (CAttack.fase)
	{
	case FASE_ONE:
		//ここで出す釘の数設定
		NailAmountSet(30);
		//座標ずらしの要素数を釘の配列サイズ分
		CAttack.ZN.resize(Nails.size());
		CAttack.XN.resize(Nails.size());
		//1つめのAOE範囲分
		for (int i = 0; i < Nails.size() / 2; i++) {
			CAttack.ZN[i] = rand() % (20 + 1 + 25) - 25;
			CAttack.XN[i] = rand() % (20 + 1 + 25) - 25;
			Nails[i]->SetPosition({ DirectionPos[area1].x + (float)CAttack.XN[i] ,MinY,DirectionPos[area1].z + (float)CAttack.ZN[i] });
		}
		//２つめのAOE範囲分
		for (int i = Nails.size() / 2; i < Nails.size(); i++) {
			CAttack.ZN[i] = rand() % (20 + 1 + 25) - 25;
			CAttack.XN[i] = rand() % (20 + 1 + 25) - 25;
			Nails[i]->SetPosition({ DirectionPos[area2].x + (float)CAttack.XN[i] ,MinY,DirectionPos[area2].z + (float)CAttack.ZN[i] });
		}
		//次のフェーズへ
		CAttack.fase = FASE_TWO;
		break;
	case FASE_TWO:
		if (MinY <= -10.0f) {
			MinY += 1.0f;//釘出るよ
		} else {
			CAttack.fase = FASE_THREE;//出終わったら次のフェーズへ
		}

		for (int i = 0; i < Nails.size(); i++) {
			Nails[i]->SetPosition({ Nails[i]->GetPosition().x ,MinY,Nails[i]->GetPosition().z });
			Nails[i]->SetScale({ 1.5f,3.0f,1.5f });
		}
		break;
	case FASE_THREE:
		CAttack.WaitCount++;//釘出現してから一定時間立ったら引っ込める
		if (CAttack.WaitCount >= 60) {
			MinY -= 1.0f;
		}
		if (MinY <= -30.0f) {
			CAttack.fase = NON;
			CAttack.EndAction = true;//１連の挙動終了
		}
		for (int i = 0; i < Nails.size(); i++) {
			Nails[i]->SetPosition({ Nails[i]->GetPosition().x ,MinY,Nails[i]->GetPosition().z });
		}
		break;

	case NON:
		CAttack.WaitCount = 0;
		break;
	default:
		break;
	}
	for (int i = 0; i < Nails.size(); i++) {
		Nails[i]->SetRotation({ 180.0f,0.0f,0.0f });
		Nails[i]->Update({ 1.0f,1.0f,1.0f,1.0f }, CameraControl::GetInstance()->GetCamera());
	}
}
void Nail::NailAmountSet(int amount)
{
	//釘の量分作る
	Nails.resize(amount);
	for (int i = 0; i < Nails.size(); i++) {
		Nails[i] = std::make_unique<Object3d>();
		//モデル割り当て
		Nails[i]->Initialize(CameraControl::GetInstance()->GetCamera());
		Nails[i]->SetModel(Nailmodel);
	}
}
void Nail::ModelSet()
{
	Nailmodel = ModelManager::GetIns()->GetModel(ModelManager::NAIL);
	//攻撃フェーズは初期値NON固定
	HAttack.fase = NON;
	CAttack.fase = NON;
}
void Nail::Update()
{
}
void Nail::Draw()
{
	for (int i = 0; i < Nails.size(); i++) {
		Object3d::PreDraw();
		Nails[i]->Draw();
		Object3d::PostDraw();
	}
}