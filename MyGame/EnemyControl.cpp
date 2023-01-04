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
#include"KnockAttack.h"
#include"CircleAttack.h"
#include"HalfAttack.h"
#include"AltAttack.h"
#include "CameraControl.h"
#include"FrontCircleAttack.h"
#include"GuardianEnemy.h"

EnemyControl* EnemyControl::GetInstance()
{
	static EnemyControl instance;

	return &instance;
}

/*------------------------*/
/*--------解放処理---------*/
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
/*--------読込処理---------*/
/*----------csv-----------*/

void EnemyControl::Init_Tutorial()
{
	enemys.resize(BOSS + 1);

	enemys[TUTORIAL].resize(1);
	enemys[TUTORIAL][0] = std::make_unique<MobEnemy>();
	enemys[TUTORIAL][0]->Initialize();
	tutorial_pos = {100.137f, 20.5045f, -650.987f};
	enemys[TUTORIAL][0]->SetPosition(tutorial_pos);
	enemys[TUTORIAL][0]->SetRespawnPos(tutorial_pos);
}

void EnemyControl::Init_Play()
{
	enemys.resize(BOSS + 1);
	file.open("Param_CSV/enemy.csv");

	popcom << file.rdbuf();

	file.close();
	/*流れとしては
	敵の数読み込み->
	読み込んだ敵の数分エネミーのパラメータ配列の要素数増やす->
	敵の数分作ったら配列の中身をロードしたものに->
	敵の番号が1だったらα,2だったらβでインスタンス生成、初期化
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
				break;
			}
		}
	}
	//敵の数をCSV分読み込み
	enemys[PLAYSCENE].resize(Quantity);

	Load_EnemyPosition.resize(Quantity);

	for (int i = 0; i < Quantity; i++)
	{
		//初期化処理
		if (Num[i] == GOLEMENEMY)
		{
			//ゴーレム
			enemys[PLAYSCENE][i] = std::make_unique<MobEnemy>();
		}
		if (Num[i] == ALPHAENEMY)
		{
			//トカゲ
			enemys[PLAYSCENE][i] = std::make_unique<EnemyAlpha>();
		}
		if (Num[i] == BETAENEMY)
		{
			//トカゲ
			enemys[PLAYSCENE][i] = std::make_unique<EnemyBeta>();
		}
		enemys[PLAYSCENE][i]->Initialize();
		enemys[PLAYSCENE][i]->SetPosition(pos[i]);
	}
	//ガーディアン
	Guardian = std::make_unique<GuardianEnemy>();
	Guardian->Initialize();
}

void EnemyControl::SummonEnemyInit()
{
	//ボスが召喚するザコ敵
	for (int i = 0; i < EnemySize; i++)
	{
		SummonEnemys[i] = std::make_unique<MobEnemy>();
		SummonEnemys[i]->Initialize();
		SummonEnemys[i]->SetPosition({0, -20, 20});
	}
	//敵生成フラグ
	summonEnemyCreate = false;
	//敵死亡フラグ
	SummonEnemysDeath = false;
	//敵登場済みフラグ
	SummonEnemysApper = false;

	SummonEPos = {1, 1, 1};
	Shieldalpha = 0.0f;
	//盾テクスチャ
	Texture* l_shield[4];
	Texture::LoadTexture(101, L"Resources/2d/BossAttackEffect/shield.png");
	for (int i = 0; i < 4; i++)
	{
		l_shield[i] = Texture::Create(101, {1, 1, 1}, {0, 0, 0}, {1, 1, 1, 1});
		ShieldTex[i].reset(l_shield[i]);
		ShieldTex[i]->CreateTexture();
		ShieldTex[i]->SetAnchorPoint({0.5f, 0.5f});
		ShieldTex[i]->SetRotation({180, 0, 0});
	}
	//回転角
	Texangle[0] = 0.f;
	Texangle[1] = 90.f;
	Texangle[2] = 180.f;
	Texangle[3] = 270.f;
}

void EnemyControl::Init_Boss()
{
	//ボス初期化
	enemys[BOSS].resize(1);
	enemys[BOSS][0] = std::make_unique<BossEnemy>();
	enemys[BOSS][0]->Initialize();
	boss_pos = {-1.0f, 9.0f, 80.987f};

	enemys[BOSS][0]->SetPosition(boss_pos);
	enemys[BOSS][0]->SetHP(enemys[BOSS][0]->GetMaxHP());
	//ザコ敵の初期化もここで
	SummonEnemyInit();


	bAttack = std::make_unique<BomAttack>();
	bAttack->Init();
}

void EnemyControl::Load()
{
}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void EnemyControl::Update_Tutorial()
{
	if (enemys[TUTORIAL][0] == nullptr)
	{
		return;
	}
	if (TutorialSprite::GetInstance()->GetClearMove())
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
	XMFLOAT3 pPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	for (int i = 0; i < Quantity; i++)
	{
		if (enemys[PLAYSCENE][i] == nullptr)
		{
			continue;
		}
		if (enemys[PLAYSCENE][i]->getdeath() == false)
		{
			if (Collision::GetLength(pPos, enemys[PLAYSCENE][i]->GetPosition()) < 100)
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

		if (enemys[PLAYSCENE][i]->GetObjAlpha() <= 0.0f)
		{
			if (enemys[PLAYSCENE][i]->GetEnemyNumber() == 0)
			{
				Task::GetInstance()->SetGolemDestroyCount();
				if (Task::GetInstance()->GetGolemDesthCount(1))
				{
					DropWeapon::GtIns()->Drop(DropWeapon::AXE, enemys[PLAYSCENE][i]->GetPosition());
				}

				if (Task::GetInstance()->GetGolemDesthCount(2))
				{
					ChestControl::GetInstance()->SetChestAppearance(ChestControl::RED, {
						                                                enemys[PLAYSCENE][i]->GetPosition().x,
						                                                enemys[PLAYSCENE][i]->GetPosition().y + 10.0f,
						                                                enemys[PLAYSCENE][i]->GetPosition().z
					                                                });
				}
			}
			else if (enemys[PLAYSCENE][i]->GetEnemyNumber() == 1)
			{
				Task::GetInstance()->SetFlogDestroyCount();

				
				if (Task::GetInstance()->ClearTaskONE() == true)
				{
					if (Task::GetInstance()->GetFlogDesthCount(2))
					{
						ChestControl::GetInstance()->SetChestAppearance(ChestControl::BLUE, {
							                                                enemys[PLAYSCENE][i]->GetPosition().x,
							                                                enemys[PLAYSCENE][i]->GetPosition().y +
							                                                10.0f,
							                                                enemys[PLAYSCENE][i]->GetPosition().z
						                                                });
					}
				}
			}
			//
			else if (enemys[PLAYSCENE][i]->GetEnemyNumber() == 2)
			{
				Task::GetInstance()->SetMiniGolemDestroyCount();
				if (Task::GetInstance()->GetMiniGolemDesthCount(1))
				{
					DropWeapon::GtIns()->Drop(DropWeapon::SWORD, enemys[PLAYSCENE][i]->GetPosition());
				}
				if (Task::GetInstance()->ClearTaskTwo() == true)
				{
					if (Task::GetInstance()->GetMiniGolemDesthCount(2))
					{
						ChestControl::GetInstance()->SetChestAppearance(ChestControl::GREEN, {
																			enemys[PLAYSCENE][i]->GetPosition().x,
																			enemys[PLAYSCENE][i]->GetPosition().y +
																			10.0f,
																			enemys[PLAYSCENE][i]->GetPosition().z
							});
					}
				}
			}
			Destroy_unique(enemys[PLAYSCENE][i]);
		}
		if (Guardian != nullptr && Guardian->GetisAlive() == FALSE && Guardian->GetHP() <= 0)
		{
			ChestControl::GetInstance()->SetChestAppearance(ChestControl::YELLOW, {
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

void EnemyControl::SummonEnemyUpdate()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	/*攻撃内容の処理なので後で攻撃専用のクラスに移す*/
	if (HalfAttack::GetInstance()->SummonEnemy() == true)
	{
		summonEnemyCreate = true;
	}

	if (summonEnemyCreate)
	{
		SummonEPos.y += 0.1f; //徐々に上に
		for (int i = 0; i < EnemySize; i++)
		{
			if (SummonEnemys[i] == nullptr)
			{
				continue;
			}
			if (SummonEPos.y < 18.0f)
			{
				//敵がプレイヤー座標まで現れたら
				Shieldalpha = 0.0f;
				//下から上に出てくる際は動き止めておく
				SummonEnemys[i]->SetMoveStop(true);
				SummonEnemys[i]->SetPosition({
					HalfAttack::GetInstance()->GetTexPos(i).x, SummonEPos.y, HalfAttack::GetInstance()->GetTexPos(i).z
				});
			}
			else
			{
				//盾テクスチャフェード
				if (!SummonEnemysDeath)
				{
					Shieldalpha += 0.05f;
				}
				//動き止めていたのを解除
				SummonEnemys[i]->SetMoveStop(false);
			}
			//更新
			SummonEnemys[i]->SetColor({1.0f, 0.2f, 0.2f, 1.0f});

			SummonEnemys[i]->Update();
		}
	}

	//ザコ敵両方とも倒したら
	if (SummonEnemys[0] == nullptr && SummonEnemys[1] == nullptr)
	{
		Shieldalpha -= 0.02f; //盾テクスチャ消す
		SummonEnemysDeath = true;
	}
	//盾テクスチャのアルファ値が一定以下なったらテクスチャインスタンスは消す
	if (Shieldalpha < -1.0f)
	{
		for (int i = 0; i < 4; i++)
		{
			Destroy_unique(ShieldTex[i]);
		}
	}
	//敵登場済みのフラグ=敵がプレイヤーのY座標まで上がってきたら
	SummonEnemysApper = SummonEPos.y >= 10.0f;

	for (int i = 0; i < 4; i++)
	{
		if (enemys[BOSS][0] == nullptr)
		{
			break;
		}
		if (ShieldTex[i] == nullptr)
		{
			continue;
		}
		//テクスチャ回す
		Texangle[i]++;
		//中心はボス
		ShieldTexPos[i].x = enemys[BOSS][0]->GetPosition().x + sinf(Texangle[i] * (3.14f / 180.0f)) * 10.0f;
		ShieldTexPos[i].y = enemys[BOSS][0]->GetPosition().y + 10.0f;
		ShieldTexPos[i].z = enemys[BOSS][0]->GetPosition().z + cosf(Texangle[i] * (3.14f / 180.0f)) * 10.0f;

		ShieldTex[i]->SetColor({1.0f, 1.0f, 1.0f, Shieldalpha});
		ShieldTex[i]->SetPosition({ShieldTexPos[i]});
		ShieldTex[i]->SetScale({5.0f, 5.0f, 1.0f});
		ShieldTex[i]->Update(camera);
	}

	//ザコ敵の開放処理
	for (int i = 0; i < 2; i++)
	{
		if (SummonEnemys[i] == nullptr)
		{
			continue;
		}
		if (SummonEnemys[i]->GetObjAlpha() <= 0)
		{
			Destroy_unique(SummonEnemys[i]);
		}
	}

	//盾テクスチャのアルファ値の上限と下限
	Shieldalpha = min(Shieldalpha, 1);
	Shieldalpha = max(Shieldalpha, 0);
	SummonEPos.y = min(SummonEPos.y, 18);
}

void EnemyControl::Update_Boss()
{
	if (enemys[BOSS][0] == nullptr)
	{
		return;
	}

	bAttack->Upda();

	enemys[BOSS][0]->Update();

	SummonEnemyUpdate();
	//ボスの開放処理
	if (enemys[BOSS][0]->GetObjAlpha() <= 0)
	{
		Destroy_unique(enemys[BOSS][0]);
	}
}

/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
void EnemyControl::HPFrameDraw()
{
	//敵のHPバー表示

	//チュートリアルの敵
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		if (enemys[TUTORIAL][0] != nullptr)
		{
			enemys[TUTORIAL][0]->EnemyHPDraw();
		}
	}

	//探索シーンのザコ敵
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
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

	//ボスの召喚敵
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		for (int i = 0; i < SummonEnemys.size(); i++)
		{
			if (SummonEnemys[i] == nullptr)
			{
				continue;
			}
			SummonEnemys[i]->EnemyHPDraw();
		}
		if (enemys[BOSS].size()>0&& enemys[BOSS][0]!=nullptr) {
			enemys[BOSS][0]->EnemyHPDraw();
		}
	}
}

#include"imgui.h"

void EnemyControl::Draw_Tutorial()
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		if (enemys[TUTORIAL][0] != nullptr)
		{
			//チュートリアルの移動タスククリアしたら描画
			if (TutorialSprite::GetInstance()->GetClearMove())
			{
				enemys[TUTORIAL][0]->Draw();
			}
		}
	}
}

void EnemyControl::Draw_Play()
{
	//プレイヤーの座標
	XMFLOAT3 pPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	//探索シーンの敵描画
	for (int i = 0; i < Quantity; i++)
	{
		if (enemys[PLAYSCENE][i] != nullptr)
		{
			//一定以上はなれたら描画切る
			if (Collision::GetLength(pPos, enemys[PLAYSCENE][i]->GetPosition()) < 100)
			{
				enemys[PLAYSCENE][i]->Draw();
			}
		}
	}
	if (Guardian != nullptr)
	{
		//ガーディアン描画
		Guardian->Draw();
	}
}

void EnemyControl::Draw_Boss()
{
	if (enemys.size() == 0 || enemys[BOSS].size() == 0 || enemys[BOSS][0] == nullptr)
	{
		return;
	}
	//召喚敵の描画
	for (int i = 0; i < EnemySize; i++)
	{
		if (SummonEnemys[i] == nullptr)
		{
			continue;
		}
		SummonEnemys[i]->Draw();
	}
	//ボス描画
	enemys[BOSS][0]->Draw();

	//ボスの貼るシールドテクスチャ
	Texture::PreDraw();

	for (int i = 0; i < 4; i++)
	{
		ShieldTex[i]->Draw();
	}

	Texture::PostDraw();

	//ボスの各攻撃描画
	CircleAttack::GetInstance()->Draw();
	HalfAttack::GetInstance()->Draw();
	KnockAttack::GetInstance()->Draw();
	AltAttack::GetInstance()->Draw();
	FrontCircleAttack::GetInstance()->Draw();
	if (bAttack != nullptr)
	{
		bAttack->Draw();
	}
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
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		enemys[BOSS][0]->Initialize();
		boss_pos = { -1.0f, 10.0f, 20.987f };

		enemys[BOSS][0]->SetPosition(boss_pos);
		enemys[BOSS][0]->SetHP(enemys[BOSS][0]->GetMaxHP());
	}
}
