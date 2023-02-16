#include"EnemyControl.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include"EnemyAlpha.h"
#include"EnemyBeta.h"
#include"TutorialSprite.h"
#include"SceneManager.h"
#include"DropWeapon.h"
#include"ChestControl.h"
#include"PlayerControl.h"
#include"HalfAttack.h"
#include "CameraControl.h"
#include"GuardianEnemy.h"

EnemyControl* EnemyControl::GetIns()
{
	static EnemyControl instance;

	return &instance;
}

EnemyControl::~EnemyControl()
{
}

/*------------------------*/
/*--------�������---------*/
/*------------------------*/
void EnemyControl::Finalize()
{
	Num.clear();
	pos.clear();
	enemys[BOSS].clear();
	enemys[PLAYSCENE].clear();
	enemys[TUTORIAL].clear();


	Load_EnemyPosition.clear();
}

/*------------------------*/
/*--------�Ǎ�����---------*/
/*----------csv-----------*/

void EnemyControl::Init_Tutorial()
{
	enemys.resize(
		static_cast<std::vector<std::vector<std::unique_ptr<Enemy, std::default_delete<Enemy>>, std::allocator<
			                                    std::unique_ptr<Enemy, std::default_delete<Enemy>>>>, std::allocator<
			                        std::vector<std::unique_ptr<Enemy, std::default_delete<Enemy>>, std::allocator<
				                                    std::unique_ptr<Enemy, std::default_delete<Enemy>>>>>>::size_type>(
			BOSS) + 1);

	enemys[TUTORIAL].resize(1);
	enemys[TUTORIAL][0] = std::make_unique<MobEnemy>();
	enemys[TUTORIAL][0]->Initialize();
	tutorial_pos = {50.137f, 20.5045f, -320.987f};
	enemys[TUTORIAL][0]->SetPosition(tutorial_pos);
	enemys[TUTORIAL][0]->SetRespawnPos(tutorial_pos);
}

void EnemyControl::Init_Play()
{
	enemys.resize(BOSS + 1);
	file.open("Param_CSV/enemy.csv");

	popcom << file.rdbuf();

	file.close();
	/*����Ƃ��Ă�
	�G�̐��ǂݍ���->
	�ǂݍ��񂾓G�̐����G�l�~�[�̃p�����[�^�z��̗v�f�����₷->
	�G�̐����������z��̒��g�����[�h�������̂�->
	�G�̔ԍ���1�������烿,2����������ŃC���X�^���X�����A������
	*/
	while (std::getline(popcom, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find("Enemy_Quantity") == 0)
		{
			std::getline(line_stream, word, ',');
			int quantity = static_cast<int>(std::atof(word.c_str()));
			Quantity = quantity;
			break;
		}
	}
	Num.resize(Quantity);
	pos.resize(Quantity);
	scl.resize(Quantity);
	rescount.resize(Quantity);
	for (int i = 0; i < Quantity; i++)
	{
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//�S�[����") == 0 || word.find("//�~�j�S�[����") == 0 || word.find("//�g�J�Q") == 0)
			{
				continue;
			}
			if (word.find("Wait") == 0)
			{
				std::getline(line_stream, word, ',');
				int count = static_cast<int>(std::atof(word.c_str()));
				rescount[i] = count;
			}
			else if (word.find("Number") == 0)
			{
				std::getline(line_stream, word, ',');
				int number = static_cast<int>(std::atof(word.c_str()));
				Num[i] = number;
			}

			else if (word.find("POP") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				pos[i] = {x, y, z};
				//break;
			}
			else if (word.find("SCL") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				scl[i] = {x + 0.02f, y + 0.02f, z + 0.02f};
				break;
			}
		}
	}
	//�G�̐���CSV���ǂݍ���
	enemys[PLAYSCENE].resize(Quantity);

	Load_EnemyPosition.resize(Quantity);

	for (int i = 0; i < Quantity; i++)
	{
		//����������
		if (Num[i] == GOLEMENEMY)
		{
			//�S�[����
			enemys[PLAYSCENE][i] = std::make_unique<MobEnemy>();
		}
		if (Num[i] == ALPHAENEMY)
		{
			//�g�J�Q
			enemys[PLAYSCENE][i] = std::make_unique<EnemyAlpha>();
		}
		if (Num[i] == BETAENEMY)
		{
			//�g�J�Q
			enemys[PLAYSCENE][i] = std::make_unique<EnemyBeta>();
		}
		enemys[PLAYSCENE][i]->Initialize();
		enemys[PLAYSCENE][i]->SetPosition(pos[i]);
		enemys[PLAYSCENE][i]->SetCreatePos(pos[i]);
		enemys[PLAYSCENE][i]->SetScale(scl[i]);
		enemys[PLAYSCENE][i]->SetRespawnPos(pos[i]);
		enemys[PLAYSCENE][i]->SetRespawnCountMax(rescount[i]);
	}
	//�K�[�f�B�A��
	Guardian = std::make_unique<GuardianEnemy>();
	Guardian->Initialize();
}


void EnemyControl::Init_Boss()
{
	//�{�X������
	enemys[BOSS].resize(1);
	enemys[BOSS][0] = std::make_unique<BossEnemy>();
	enemys[BOSS][0]->Initialize();
	boss_pos = {-1.0f, 14.75f, 80.987f};

	enemys[BOSS][0]->SetPosition(boss_pos);
	enemys[BOSS][0]->SetHP(enemys[BOSS][0]->GetMaxHP());
	//�U�R�G�̏�������������
	//SummonEnemyInit();
}

void EnemyControl::Load()
{
}

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
void EnemyControl::Update_Tutorial()
{
	if (enemys[TUTORIAL][0] == nullptr)
	{
		return;
	}
	if (TutorialSprite::GetIns()->GetClearMove())
	{
		enemys[TUTORIAL][0]->Update();
	}
	if (enemys[TUTORIAL][0]->GetObjAlpha() <= 0.0f)
	{
		Destroy_unique(enemys[TUTORIAL][0]);
	}
}

void EnemyControl::Update_Play()
{
	XMFLOAT3 pPos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	for (int i = 0; i < Quantity; i++)
	{
		if (enemys[PLAYSCENE][i] == nullptr)
		{
			continue;
		}
		if (enemys[PLAYSCENE][i]->getdeath() == false)
		{
			if (Collision::GetLength(pPos, enemys[PLAYSCENE][i]->GetPosition()) < 150)
			{
				enemys[PLAYSCENE][i]->SetMoveFlag(true);
				enemys[PLAYSCENE][i]->Update();
			}
		}
		else
		{
			enemys[PLAYSCENE][i]->SetMoveFlag(true);
			enemys[PLAYSCENE][i]->Update();
		}

		if (enemys[PLAYSCENE][i]->GetObjAlpha() <= 0.0f && enemys[PLAYSCENE][i]->GetRespawnCount() == 0)
		{
			if (enemys[PLAYSCENE][i]->GetEnemyNumber() == 0)
			{
				Task::GetIns()->SetGolemDestroyCount();
				if (Task::GetIns()->GetGolemDesthCount(1))
				{
					DropWeapon::GtIns()->Drop(DropWeapon::AXE, enemys[PLAYSCENE][i]->GetPosition());
				}

				if (Task::GetIns()->GetGolemDesthCount(2))
				{
					ChestControl::GetIns()->SetChestAppearance(ChestControl::RED, {
						                                           enemys[PLAYSCENE][i]->GetPosition().x,
						                                           enemys[PLAYSCENE][i]->GetPosition().y + 10.0f,
						                                           enemys[PLAYSCENE][i]->GetPosition().z
					                                           });
				}
			}
			
			//
			else if (enemys[PLAYSCENE][i]->GetEnemyNumber() == 2)
			{
				Task::GetIns()->SetMiniGolemDestroyCount();
				if (Task::GetIns()->GetMiniGolemDesthCount(1))
				{
					DropWeapon::GtIns()->Drop(DropWeapon::SWORD, enemys[PLAYSCENE][i]->GetPosition());
				}
					if (Task::GetIns()->GetMiniGolemDesthCount(2))
					{
						ChestControl::GetIns()->SetChestAppearance(ChestControl::GREEN, {
							                                           enemys[PLAYSCENE][i]->GetPosition().x,
							                                           enemys[PLAYSCENE][i]->GetPosition().y +
							                                           10.0f,
							                                           enemys[PLAYSCENE][i]->GetPosition().z
						                                           });
					}
				}
			}
			//Destroy_unique(enemys[PLAYSCENE][i]);
		
		if (Guardian != nullptr && Guardian->GetisAlive() == FALSE && Guardian->GetHP() <= 0)
		{
			ChestControl::GetIns()->SetChestAppearance(ChestControl::YELLOW, {
				                                           Guardian->GetPosition().x,
				                                           Guardian->GetPosition().y +
				                                           10.0f,
				                                           Guardian->GetPosition().z
			                                           });
		}
	}
	if (Guardian != nullptr)
	{
		Guardian->Update();
		if (Guardian->GetObjAlpha() <= 0.0f)
		{
			Destroy_unique(Guardian);
		}
	}
}

void EnemyControl::GuardianCreate()
{
	Guardian.reset(new GuardianEnemy());
	Guardian->Initialize();
}

void EnemyControl::GuardianReset()
{
	if (Guardian != nullptr)
	{
		if (PlayerControl::GetIns()->GetPlayer()->GetHP() <= 0)
		{
			Destroy_unique(Guardian);
		}
	}
}

void EnemyControl::Update_Boss()
{
	if (enemys[BOSS][0] == nullptr)
	{
		return;
	}
	//bAttack->Upda();

	enemys[BOSS][0]->Update();

	//�{�X�̊J������
	if (enemys[BOSS][0]->GetObjAlpha() <= 0)
	{
		Destroy_unique(enemys[BOSS][0]);
	}
}

/*------------------------*/
/*--------�`�揈��---------*/
/*------------------------*/
void EnemyControl::HPFrameDraw()
{
	//�G��HP�o�[�\��

	//�`���[�g���A���̓G
	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	{
		if (enemys[TUTORIAL][0] != nullptr)
		{
			enemys[TUTORIAL][0]->EnemyHPDraw();
		}
	}

	//�T���V�[���̃U�R�G
	else if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	{
		for (int i = 0; i < Quantity; i++)
		{
			if (enemys[PLAYSCENE][i] == nullptr)
			{
				continue;
			}
			enemys[PLAYSCENE][i]->EnemyHPDraw();
		}
		if (Guardian != nullptr)
		{
			Guardian->EnemyHPDraw();
		}
	}

	//�{�X�̏����G
	else if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		if (enemys[BOSS].size() > 0 && enemys[BOSS][0] != nullptr)
		{
			enemys[BOSS][0]->EnemyHPDraw();
		}
	}
}

#include"imgui.h"

void EnemyControl::Draw_Tutorial()
{
	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	{
		if (enemys[TUTORIAL][0] != nullptr)
		{
			//�`���[�g���A���̈ړ��^�X�N�N���A������`��
			if (TutorialSprite::GetIns()->GetClearMove())
			{
				enemys[TUTORIAL][0]->Draw();
			}
		}
	}
}

void EnemyControl::Draw_Play()
{
	//�v���C���[�̍��W
	XMFLOAT3 pPos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	//�T���V�[���̓G�`��
	for (int i = 0; i < Quantity; i++)
	{
		if (enemys[PLAYSCENE][i] != nullptr)
		{
			//���ȏ�͂Ȃꂽ��`��؂�
			if (Collision::GetLength(pPos, enemys[PLAYSCENE][i]->GetPosition()) < 100)
			{
				enemys[PLAYSCENE][i]->Draw();
			}
		}
	}
	if (Guardian != nullptr)
	{
		//�K�[�f�B�A���`��
		Guardian->Draw();
	}
}

void EnemyControl::Draw_Boss()
{
	if (enemys.size() == 0 || enemys[BOSS].size() == 0 || enemys[BOSS][0] == nullptr)
	{
		return;
	}

	//�{�X�`��
	enemys[BOSS][0]->Draw();
}


std::vector<std::unique_ptr<Enemy>>& EnemyControl::GetEnemy(EnemyType type)
{
	if (type == TUTORIAL)
	{
		return enemys[TUTORIAL];
	}
	if (type == PLAYSCENE)
	{
		return enemys[PLAYSCENE];
	}
	if (type == BOSS)
	{
		return enemys[BOSS];
	}

	return enemys[NON];
}

void EnemyControl::GameOverResetParam()
{
	//enemys[BOSS][0] = std::make_unique<BossEnemy>();
	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		enemys[BOSS][0]->Initialize();

		boss_pos = {-1.0f, 14.75f, 20.987f};

		HalfAttack::GetIns()->SummonEnemyResetParam();

		enemys[BOSS][0]->SetPosition(boss_pos);
		enemys[BOSS][0]->SetHP(enemys[BOSS][0]->GetMaxHP());
	}
}
