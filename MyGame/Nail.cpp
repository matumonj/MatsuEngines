#include "Nail.h"
#include"CameraControl.h"
#include"Input.h"
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
			Nails[i]->SetScale({ 1,3,1 });
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

		HAttack.WaitCount = 0;
		break;
	case FASE_THREE:
		HAttack.WaitCount++;
		if (HAttack.WaitCount >= 60) {
			MinY -= 1;
		}
		if (MinY <= -30) {
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
		Nails[i]->SetRotation({ 180,0,0 });
		Nails[i]->Update({ 1,1,1,1 }, CameraControl::GetInstance()->GetCamera());
	}

}
void Nail::CircleAttack(int area1, int area2)
{
	//������k��AOE���S�ꏊ�@�Q�Ƃ���Ȃ肵����������
	XMFLOAT3 DirectionPos[4] = { { 0,-18,30 }, { 0,-18,-30 }, { 30,-18,0 }, { -30,-18,0 } };//3
	if (CAttack.fase == NON && !CAttack.EndAction) {
		CAttack.fase = FASE_ONE;
	}

	switch (CAttack.fase)
	{
	case FASE_ONE:
		//�����ŏo���B�̐��ݒ�
		NailAmountSet(30);
		//���W���炵�̗v�f����B�̔z��T�C�Y��
		CAttack.ZN.resize(Nails.size());
		CAttack.XN.resize(Nails.size());
		//1�߂�AOE�͈͕�
		for (int i = 0; i < Nails.size() / 2; i++) {
			CAttack.ZN[i] = rand() % (20 + 1 + 25) - 25;
			CAttack.XN[i] = rand() % (20 + 1 + 25) - 25;
			Nails[i]->SetPosition({ DirectionPos[area1].x + (float)CAttack.XN[i] ,MinY,DirectionPos[area1].z + (float)CAttack.ZN[i] });
		}
		//�Q�߂�AOE�͈͕�
		for (int i = Nails.size() / 2; i < Nails.size(); i++) {
			CAttack.ZN[i] = rand() % (20 + 1 + 25) - 25;
			CAttack.XN[i] = rand() % (20 + 1 + 25) - 25;
			Nails[i]->SetPosition({ DirectionPos[area2].x + (float)CAttack.XN[i] ,MinY,DirectionPos[area2].z + (float)CAttack.ZN[i] });
		}
		//���̃t�F�[�Y��
		CAttack.fase = FASE_TWO;
		break;
	case FASE_TWO:
		if (MinY <= -10) {
			MinY += 1;//�B�o���
		} else {
			CAttack.fase = FASE_THREE;//�o�I������玟�̃t�F�[�Y��
		}

		for (int i = 0; i < Nails.size(); i++) {
			Nails[i]->SetPosition({ Nails[i]->GetPosition().x ,MinY,Nails[i]->GetPosition().z });
			Nails[i]->SetScale({ 1.5,3,1.5 });
		}
		break;
	case FASE_THREE:
		CAttack.WaitCount++;//�B�o�����Ă����莞�ԗ�������������߂�
		if (CAttack.WaitCount >= 60) {
			MinY -= 1;
		}
		if (MinY <= -30) {
			CAttack.fase = NON;
			CAttack.EndAction = true;//�P�A�̋����I��
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
		Nails[i]->SetRotation({ 180,0,0 });
		Nails[i]->Update({ 1,1,1,1 }, CameraControl::GetInstance()->GetCamera());
	}
}
void Nail::NailAmountSet(int amount)
{
	//�B�̗ʕ����
	Nails.resize(amount);
	for (int i = 0; i < Nails.size(); i++) {
		Nails[i] = std::make_unique<Object3d>();
		//���f�����蓖��
		Nails[i]->Initialize(CameraControl::GetInstance()->GetCamera());
		Nails[i]->SetModel(Nailmodel);
	}
}
void Nail::ModelSet()
{
	Nailmodel = Model::CreateFromOBJ("Nail");
	//�U���t�F�[�Y�͏����lNON�Œ�
	HAttack.fase = NON;
	CAttack.fase = NON;
}
void Nail::Update()
{
	//HalfAttack(RIGHT);
	//CircleAttack(CircleAttack::GetInstance()->GetDamageArea1(), CircleAttack::GetInstance()->GetDamageArea2());

}
void Nail::Draw()
{
	for (int i = 0; i < Nails.size(); i++) {
		Nails[i]->PreDraw();
		Nails[i]->Draw();
		Nails[i]->PostDraw();
	}
}