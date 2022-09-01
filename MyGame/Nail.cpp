#include "Nail.h"
#include"CameraControl.h"
#include"Input.h"
void Nail::HalfAttack(HalfAttackArea area)
{
	if (Input::GetInstance()->TriggerButton(Input::GetInstance()->Button_A)) {
		HAttack.fase = FASE_ONE;
	}
	switch (HAttack.fase)
	{
	case FASE_ONE:
		SetF = true;

		NailAmountSet(20);
		for (int i = 0; i < Nails.size(); i++) {
			HAttack.ZN.resize(Nails.size());
			HAttack.XN.resize(Nails.size());

			HAttack.ZN[i] = rand() % (40 + 1 + 60) - 60;

			if (area == RIGHT) {
				HAttack.XN[i] = rand() % 40;
			}
			else if (area == LEFT) {
				HAttack.XN[i] = rand() % 40 - 40;
			}

			Nails[i]->SetPosition({ (float)HAttack.XN[i],MinY,(float)HAttack.ZN[i] });
		}
		HAttack.fase = FASE_TWO;
		break;
	case FASE_TWO:
		if (MinY <= -10) {
			MinY += 1;
		} else {
			HAttack.fase = FASE_THREE;
		}
		for (int i = 0; i < Nails.size(); i++) {
			Nails[i]->SetPosition({ Nails[i]->GetPosition().x ,MinY,Nails[i]->GetPosition().z });
		}
		break;
	case FASE_THREE:
		HAttack.WaitCount++;
		if (HAttack.WaitCount >= 60) {
			MinY -= 1;
		}
		if (MinY <= -30) {
			HAttack.fase = NON;
		}
		for (int i = 0; i < Nails.size(); i++) {
			Nails[i]->SetPosition({ Nails[i]->GetPosition().x ,MinY,Nails[i]->GetPosition().z });
		}
		break;

	case NON:
		HAttack.WaitCount = 0;
		break;
	default:
		break;
	}
	

}
void Nail::CircleAttack(int area1, int area2)
{

}
void Nail::NailAmountSet(int amount)
{
	if (SetF) {
		Nails.resize(amount);
		for (int i = 0; i < Nails.size(); i++) {
			Nails[i] = std::make_unique<Object3d>();
			//フィールドにモデル割り当て
			Nails[i]->Initialize(CameraControl::GetInstance()->GetCamera());
			Nails[i]->SetModel(Nailmodel);

			//ps0 = new OBBCollision();
		}
		SetF = false;
	}
}
void Nail::ModelSet()
{
	Nailmodel= Model::CreateFromOBJ("Nail");
	HAttack.fase = NON;
}
void Nail::Update()
{
	HalfAttack(RIGHT);
	for (int i = 0; i < Nails.size(); i++) {
		Nails[i]->SetScale({ 1,3,1 });
		Nails[i]->SetRotation({ 180,0,0 });
		Nails[i]->Update({ 1,1,1,1 }, CameraControl::GetInstance()->GetCamera());
	}
}
void Nail::Draw()
{
	for (int i = 0; i < Nails.size(); i++) {
		Nails[i]->PreDraw();
		Nails[i]->Draw();
		Nails[i]->PostDraw();
	}
}