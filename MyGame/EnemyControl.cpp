#include"EnemyControl.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include"EnemyAlpha.h"
#include"TutorialSprite.h"
#include"SistemConfig.h"
#include"SceneManager.h"

#include"ChestControl.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"KnockAttack.h"
#include"CircleAttack.h"
#include"HalfAttack.h"
#include"AltAttack.h"
#include"DebugTxt.h"
#include"FrontCircleAttack.h"

EnemyControl* EnemyControl::GetInstance()
{
	static EnemyControl instance;

	return &instance;
}

/*------------------------*/
/*--------�������---------*/
/*------------------------*/
void EnemyControl::Finalize()
{
	Num.clear();
	pos.clear();
	enemys.clear();
	Load_EnemyPosition.clear();
}

/*------------------------*/
/*--------�Ǎ�����---------*/
/*----------csv-----------*/

void EnemyControl::Init_Tutorial(DebugCamera* camera)
{
	enemys.resize(BOSS + 1);

	enemys[TUTORIAL].resize(1);
	enemys[TUTORIAL][0] = std::make_unique<MobEnemy>();
	enemys[TUTORIAL][0]->Initialize(camera);
	tutorial_pos = { 100.137f, 20.5045f, -650.987f };
	enemys[TUTORIAL][0]->SetPosition(tutorial_pos);
	enemys[TUTORIAL][0]->SetRespawnPos(tutorial_pos);
}

void EnemyControl::Init_Play(DebugCamera* camera)
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
	for (int i = 0; i < Quantity; i++)
	{
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find("Number") == 0)
			{
				std::getline(line_stream, word, ',');
				int number = static_cast<int>(std::atof(word.c_str()));
				Num[i] = number;
			} else if (word.find("POP") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				pos[i] = { x, y, z };
				break;
			}
		}
	}
	enemys[PLAYSCENE].resize(Quantity);

	Load_EnemyPosition.resize(Quantity);

	for (int i = 0; i < Quantity; i++)
	{
		//����������
		if (Num[i] == ALPHAENEMY)
		{
			enemys[PLAYSCENE][i] = std::make_unique<MobEnemy>();
		}
		if (Num[i] == BETAENEMY)
		{
			enemys[PLAYSCENE][i] = std::make_unique<EnemyAlpha>();
		}

		enemys[PLAYSCENE][i]->Initialize(camera);
		enemys[PLAYSCENE][i]->SetPosition(pos[i]);
		enemys[PLAYSCENE][i]->SetRespawnPos(pos[i]);
	}
}


void EnemyControl::Init_Boss(DebugCamera* camera)
{
	enemys[BOSS].push_back(std::make_unique<BossEnemy>());
	enemys[BOSS][0] = std::make_unique<BossEnemy>();
	enemys[BOSS][0]->Initialize(camera);
	boss_pos = { -1.0f, 10.0f, 20.987f };
	enemys[BOSS][0]->SetPosition(boss_pos);
	for (int i = 0; i < EnemySize; i++) {
		SummonEnemys[i] = std::make_unique<MobEnemy>();
		SummonEnemys[i]->Initialize(camera);
		SummonEnemys[i]->SetPosition({ 0,-20,20 });
	}

	Texture* l_shield[4];
	Texture::LoadTexture(101, L"Resources/2d/BossAttackEffect/shield.png");
	for (int i = 0; i < 4; i++) {
		l_shield[i] = Texture::Create(101, { 1,1,1 }, { 0,0,0 }, { 1,1,1,1 });
		ShielTex[i].reset(l_shield[i]);
		ShielTex[i]->CreateTexture();
		ShielTex[i]->SetAnchorPoint({ 0.5f,0.5f });
		ShielTex[i]->SetRotation({ 180,0,0 });
	}
	Texangle[0] = 0;
	Texangle[1] = 90;
	Texangle[2] = 180;
	Texangle[3] = 270;

	HalfAttack::GetInstance()->Initialize();
	KnockAttack::GetInstance()->Initialize();
	CircleAttack::GetInstance()->Initialize();
	AltAttack::GetInstance()->Initialize();
	FrontCircleAttack::GetInstance()->Initialize();
}

void EnemyControl::Load(DebugCamera* camera)
{
}

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
void EnemyControl::Update_Tutorial(DebugCamera* camera)
{
	if (enemys[TUTORIAL][0] == nullptr)
	{
		return;
	}
	if (TutorialSprite::GetInstance()->GetClearMove())
	{
		//enemys[EnemyType::TUTORIAL][0]->SetMoveFlag(true);
		enemys[TUTORIAL][0]->Update(camera);
	}
	if (enemys[TUTORIAL][0]->GetObjAlpha() <= 0.0f)
	{
		Destroy_unique(enemys[TUTORIAL][0]);
	}
}

void EnemyControl::Update_Play(DebugCamera* camera)
{
	XMFLOAT3 pPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	for (int i = 0; i < Quantity; i++)
	{
		if (enemys[PLAYSCENE][i] == nullptr)
		{
			continue;
		}
		if (Collision::GetLength(pPos, enemys[PLAYSCENE][i]->GetPosition()) < 200)
		{
			enemys[PLAYSCENE][i]->SetMoveFlag(true);
			enemys[PLAYSCENE][i]->Update(camera);
		}
		if (enemys[PLAYSCENE][i]->GetObjAlpha() <= 0.0f)
		{
			if(enemys[PLAYSCENE][i]->GetEnemyNumber()== enemys[PLAYSCENE][i]->EnemyNumber::GOLEM)
			{
				Task::GetInstance()->SetGolemDestroyCount();
			}
			else if(enemys[PLAYSCENE][i]->GetEnemyNumber() == enemys[PLAYSCENE][i]->EnemyNumber::FLOG)
			{
				Task::GetInstance()->SetFlogDestroyCount();
			}
			Destroy_unique(enemys[PLAYSCENE][i]);
		}
	}
}

void EnemyControl::Update_Boss(DebugCamera* camera)
{
	if (enemys[BOSS][0] == nullptr)
	{
		return;
	}
	enemys[BOSS][0]->Update(camera);
	if (enemys[BOSS][0]->GetObjAlpha() <= 0)
	{
		Destroy_unique(enemys[BOSS][0]);
	}
	if (HalfAttack::GetInstance()->SummonEnemy() == true) {
		summonEnemyCreate = true;
	}
	if (summonEnemyCreate) {
		SummonEPos.y += 0.1f;
		for (int i = 0; i < EnemySize; i++) {
			if (SummonEnemys[i] == nullptr)continue;
			if (SummonEPos.y < 10.0f) {
				Shieldalpha = 0.0f;
				SummonEnemys[i]->SetMoveStop(true);
				SummonEnemys[i]->SetPosition({ HalfAttack::GetInstance()->GetTexPos(i).x,SummonEPos.y,HalfAttack::GetInstance()->GetTexPos(i).z });
			} else {
				if (!SummonEnemysDeath) {
					Shieldalpha += 0.05f;
				} else {
					Shieldalpha -= 0.05f;
				}
				SummonEnemys[i]->SetMoveStop(false);
			}
			SummonEnemys[i]->Update(camera);

		}
	}

	if (SummonEnemys[0] == nullptr && SummonEnemys[1] == nullptr) {
		Shieldalpha -= 0.02f;
		SummonEnemysDeath = true;
	}
	if (Shieldalpha < -1.0f) {

		for (int i = 0; i < 4; i++) {
			Destroy_unique(ShielTex[i]);
		}
	}

	SummonEnemysApper = SummonEPos.y >= 10.0f;

	for (int i = 0; i < 4; i++) {
		if (enemys[BOSS][0] == nullptr)break;
		if (ShielTex[i] == nullptr)continue;
		Texangle[i]++;
		ShieldTexPos[i].x = enemys[BOSS][0]->GetPosition().x + sinf(Texangle[i] * (3.14f / 180.0f)) * 10.0f;
		ShieldTexPos[i].y = enemys[BOSS][0]->GetPosition().y + 10.0f;
		ShieldTexPos[i].z = enemys[BOSS][0]->GetPosition().z + cosf(Texangle[i] * (3.14f / 180.0f)) * 10.0f;

		ShielTex[i]->SetColor({ 1.0f,1.0f,1.0f,Shieldalpha });
		ShielTex[i]->SetPosition({ ShieldTexPos[i] });
		ShielTex[i]->SetScale({ 5.0f,5.0f,1.0f });
		ShielTex[i]->Update(camera);
	}
	for (int i = 0; i < 2; i++) {
		if (SummonEnemys[i] == nullptr)continue;
		if (SummonEnemys[i]->GetObjAlpha() <= 0)
		{
			Destroy_unique(SummonEnemys[i]);
		}
	}
	Shieldalpha = min(Shieldalpha, 1);
	Shieldalpha = max(Shieldalpha, 0);
	SummonEPos.y = min(SummonEPos.y, 10);
}

/*------------------------*/
/*--------�`�揈��---------*/
/*------------------------*/
#include"imgui.h"

void EnemyControl::Draw_Tutorial()
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		if (enemys[TUTORIAL][0] != nullptr)
		{
			if (TutorialSprite::GetInstance()->GetClearMove())
			{
				enemys[TUTORIAL][0]->Draw();
			}
		}
	}
}

void EnemyControl::Draw_Play()
{
	XMFLOAT3 pPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	for (int i = 0; i < Quantity; i++)
	{
		if (enemys[PLAYSCENE][i] != nullptr)
		{
			if (Collision::GetLength(pPos, enemys[PLAYSCENE][i]->GetPosition()) < 200)
			{
				enemys[PLAYSCENE][i]->Draw();
			}
		}
	}
}

void EnemyControl::Draw_Boss()
{
	if (enemys[BOSS][0] == nullptr)
	{
		return;
	}
	for (int i = 0; i < EnemySize; i++) {
		if (SummonEnemys[i] == nullptr)continue;
		SummonEnemys[i]->Draw();
	}
	enemys[BOSS][0]->Draw();
	Texture::PreDraw();
	for (int i = 0; i < 4; i++) {
		ShielTex[i]->Draw();
	}
	Texture::PostDraw();
	CircleAttack::GetInstance()->Draw();
	HalfAttack::GetInstance()->Draw();
	KnockAttack::GetInstance()->Draw();
	AltAttack::GetInstance()->Draw();
	FrontCircleAttack::GetInstance()->Draw();
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
