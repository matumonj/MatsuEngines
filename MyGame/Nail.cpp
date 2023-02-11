#include "Nail.h"
#include"CameraControl.h"
#include"Input.h"
#include"ModelManager.h"
#include <EnemyControl.h>

#include "PlayerControl.h"

Nail::~Nail()
{
	//delete Nailmodel;
}

Nail* Nail::GetIns()
{
	static Nail instance;
	return &instance;
}

void Nail::HalfAttack(const HalfAttackArea& area)
{
	if (HAttack.phase == NON && !HAttack.EndAction)
	{
		HAttack.phase = PHASE_ONE;
	}

	switch (HAttack.phase)
	{
	case PHASE_ONE:

		NailAmountSet(20);
		for (int i = 0; i < Nails.size(); i++)
		{
			HAttack.ZN.resize(Nails.size());
			HAttack.XN.resize(Nails.size());

			HAttack.ZN[i] = rand() % (40 + 1 + 60) - 60;

			if (area == RIGHT)
			{
				HAttack.XN[i] = rand() % 40;
			}
			else if (area == LEFT)
			{
				HAttack.XN[i] = rand() % 40 - 40;
			}
			Nails[i]->SetScale({1.0f, 3.0f, 1.0f});
			Nails[i]->SetPosition({static_cast<float>(HAttack.XN[i]), MinY, static_cast<float>(HAttack.ZN[i])});
		}
		HAttack.phase = PHASE_TWO;
		break;
	case PHASE_TWO:
		if (MinY <= -10.0f)
		{
			MinY += 1.0f;
		}
		else
		{
			HAttack.phase = PHASE_THREE;
		}
		for (int i = 0; i < Nails.size(); i++)
		{
			Nails[i]->SetPosition({Nails[i]->GetPosition().x, MinY, Nails[i]->GetPosition().z});
		}
		
		HAttack.WaitCount = 0;
		break;
	case PHASE_THREE:
		HAttack.WaitCount++;
		if (HAttack.WaitCount >= 60)
		{
			MinY -= 1.0f;
		}
		if (MinY <= -30.0f)
		{
			HAttack.EndAction = true;
			HAttack.phase = NON;
		}
		for (int i = 0; i < Nails.size(); i++)
		{
			Nails[i]->SetPosition({Nails[i]->GetPosition().x, MinY, Nails[i]->GetPosition().z});
		}
		break;

	case NON:
		Nails.clear();
		HAttack.WaitCount = 0;
	//HAttack.EndAction = false;
		break;
	default:
		break;
	}
	for (int i = 0; i < Nails.size(); i++)
	{
		Nails[i]->SetRotation({180.0f, 0.0f, 0.0f});
		Nails[i]->Update( CameraControl::GetIns()->GetCamera());
	}
}

void Nail::CircleAttack(int area1, int area2)
{
	Enemy* boss = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0].get();

	//������k��AOE���S�ꏊ�@�Q�Ƃ���Ȃ肵����������
	XMFLOAT3 DirectionPos[4] = {
		{0.0f, -18.0f, 60.0f}, {0.0f, -18.0f, -60.0f}, {60.0f, -18.0f, 0.0f}, {-60.0f, -18.0f, 0.0f}
	}; //3
	if (CAttack.phase == NON && !CAttack.EndAction)
	{
		CAttack.phase = PHASE_ONE;
	}
	XMFLOAT3 Bpos = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
	//float Ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition().x;
	XMFLOAT3 Ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	bool col = Collision::GetLength(Ppos, Bpos) < 30.f;
	switch (CAttack.phase)
	{
	case PHASE_ONE:
		//�����ŏo���B�̐��ݒ�
		NailAmountSet(25);
	//���W���炵�̗v�f����B�̔z��T�C�Y��
		CAttack.ZN.resize(Nails.size());
		CAttack.XN.resize(Nails.size());
	//1�߂�AOE�͈͕�
		for (int i = 0; i < Nails.size(); i++)
		{
			CAttack.ZN[i] = rand() % 50 - 25;
			CAttack.XN[i] = rand() % 50 - 25;
			Nails[i]->SetPosition({
				boss->GetPosition().x + static_cast<float>(CAttack.XN[i]), MinY,
				boss->GetPosition().z + static_cast<float>(CAttack.ZN[i])
			});
		}

	//���̃t�F�[�Y��
		CAttack.phase = PHASE_TWO;
		break;
	case PHASE_TWO:
		if (MinY <= 20.0f)
		{
			if (CAttack.phase == PHASE_TWO) {
			if (col)
				{
						PlayerControl::GetIns()->GetPlayer()->RecvDamage(5);
				}
			}
			MinY += 2.0f; //�B�o���
		}
		else
		{
			CAttack.phase = PHASE_THREE; //�o�I������玟�̃t�F�[�Y��
		}

		for (int i = 0; i < Nails.size(); i++)
		{
			Nails[i]->SetPosition({Nails[i]->GetPosition().x, MinY, Nails[i]->GetPosition().z});
			Nails[i]->SetScale({1.5f, 3.0f + static_cast<float>(i) / 10.0f, 1.5f});
		}
		break;
	case PHASE_THREE:
		CAttack.WaitCount++; //�B�o�����Ă����莞�ԗ�������������߂�
		if (CAttack.WaitCount >= 120)
		{
			MinY -= 1.0f;
		}
		if (MinY <= -50.0f)
		{
			CAttack.phase = NON;
			CAttack.EndAction = true; //�P�A�̋����I��
		}
		for (int i = 0; i < Nails.size(); i++)
		{
			Nails[i]->SetPosition({Nails[i]->GetPosition().x, MinY, Nails[i]->GetPosition().z});
		}
		break;

	case NON:
		CAttack.EndAction = false;
		Nails.clear();
		CAttack.WaitCount = 0;
		break;
	default:
		break;
	}
	for (int i = 0; i < Nails.size(); i++)
	{
		Nails[i]->SetRotation({180.0f, 0.0f, 0.0f});
		Nails[i]->Update( CameraControl::GetIns()->GetCamera());
	}
}

void Nail::NailAmountSet(int amount)
{
	//�B�̗ʕ����
	Nails.resize(amount);
	for (int i = 0; i < Nails.size(); i++)
	{
		Nails[i] = std::make_unique<Object3d>();
		//���f�����蓖��
		Nails[i]->Initialize(CameraControl::GetIns()->GetCamera());
		Nails[i]->SetModel(Nailmodel);
	}
	CAttack.WaitCount = 0;
}

void Nail::ModelSet()
{
	Nailmodel = ModelManager::GetIns()->GetModel(ModelManager::NAIL);
	//�U���t�F�[�Y�͏����lNON�Œ�
	HAttack.phase = NON;
	CAttack.phase = NON;
	MinY = -30;
}

void Nail::Update()
{
}

#include"imgui.h"

void Nail::Draw()
{
	for (int i = 0; i < Nails.size(); i++)
	{
		Object3d::PreDraw();
		Nails[i]->Draw();
		Object3d::PostDraw();
	}
	/*ImGui::Begin("er");
	ImGui::Text("size %d", Nails.size());
	if (Nails.size()>0) {
		ImGui::Text("posy %f", Nails[0]->GetPosition().y);
	}
		ImGui::End();*/
}

void Nail::Finalize()
{
	Nails.clear();
}
