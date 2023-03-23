#include "PlaceEnemy.h"
#include"MobEnemy.h"
#include"EnemyAlpha.h"
#include "EnemyBeta.h"
#include"imgui.h"

void PlaceEnemy::Initialize(DebugCamera* camera)
{
	//Model = Model::CreateFromOBJ("box");
	Obj = Object3d::Create(camera);
	Obj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::PLACEBOX));
}

bool PlaceEnemy::ErrorJudg()
{
	const int Min_EnemySize = 2; //設置できる敵の最小数
	const int Max_EnemySize = 30; //設置できる敵の最大数

	if (enemys.size() < Min_EnemySize || enemys.size() > Max_EnemySize)
	{
		return true;
	}
	return false;
}

void PlaceEnemy::FileWriting()
{
	file.open("enemy.csv");

	popcom << file.rdbuf();

	file.close();
	std::ofstream ofs("Param_CSV/enemy.csv"); // ファイルパスを指定する
	ofs << "Enemy_Quantity" << "," << enemys.size() << std::endl;

	for (int i = 0; i < enemys.size(); i++)
	{
		ofs << "//--------------------------------//" << std::endl;
		if (Number[i] == 1)
		{
			ofs << "//ゴーレム" << std::endl;
		}
		else if (Number[i] == 2)
		{
			ofs << "//トカゲ" << std::endl;
		}
		else if (Number[i] == 3)
		{
			ofs << "//ミニゴーレム" << std::endl;
		}

		ofs << "Wait" << "," << 120 << std::endl;
		ofs << "Number" << "," << Number[i] << std::endl;
		ofs << "POP" << "," << enemys[i]->GetPosition().x
			<< "," << enemys[i]->GetPosition().y
			<< "," << enemys[i]->GetPosition().z << std::endl;
		ofs << "SCL" << "," << enemys[i]->GetScale().x
			<< "," << enemys[i]->GetScale().y
			<< "," << enemys[i]->GetScale().z << std::endl;
		ofs << "ROT" << "," << enemys[i]->GetRotation().x
			<< "," << enemys[i]->GetRotation().y
			<< "," << enemys[i]->GetRotation().z << std::endl;
		ofs << "//--------------------------------//" << std::endl;
	}
}

void PlaceEnemy::ArgMent(DebugCamera* camera)
{
	if (ArgmentFlag || BossArgmentFlag || BetaArgmentFlag)
	{
		std::unique_ptr<Enemy> newEnemy;
		if (ArgmentFlag)
		{
			enumbers.push_back(AXEGOLEM);
			newEnemy = std::make_unique<MobEnemy>();
		}
		if (BossArgmentFlag)
		{
			enumbers.push_back(LIZARD);
			newEnemy = std::make_unique<EnemyAlpha>();
		}
		if (BetaArgmentFlag)
		{
			enumbers.push_back(THROWGOLEM);
			newEnemy = std::make_unique<EnemyBeta>();
		}
		newEnemy->Initialize();
		newEnemy->SetPosition(pos);
		enemys.push_back(std::move(newEnemy));

		std::unique_ptr<createimgui> newlist;
		newlist = std::make_unique<createimgui>(std::to_string(static_cast<int>(enemys.size())), 0.02f, pos);
		newlist->SetImguiNumber(static_cast<int>(enemys.size() - 1));

		imguilist.push_back(std::move(newlist));
		SelThis.push_back(false);

		ArgmentFlag = false;
		BossArgmentFlag = false;
		BetaArgmentFlag = false;
	}
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		if (enemy != nullptr)
		{
			//enemy->SetColors({ 0,1,0,1 });
			enemy->SetMoveStop(true);
			enemy->Update();
		}
	}
	if (DeleteFlag && enemys.size() > 1)
	{
		enemys.pop_back();
		Number.pop_back();
		DeleteFlag = false;
	}
}

void PlaceEnemy::Update(DebugCamera* camera)
{
	Obj->SetPosition(pos);
	Obj->SetScale({8, 6, 8});
	Obj->SetColor({1, 1, 1, 0.5f});
	Obj->Update(camera);

	for (int i = 0; i < enemys.size(); i++)
	{
		if (imguilist[i] == nullptr)
		{
			continue;
		}
		if (enemys[i] == nullptr)
		{
			continue;
		}

		if (i != 0)
		{
			if (imguilist[i]->GetImguiNumber() - imguilist[i - 1]->GetImguiNumber() > 1)
			{
				imguilist[i]->SetImguiNumber(imguilist[i - 1]->GetImguiNumber() + 1);
			}
		}
		if (imguilist.size() == 1)
		{
			imguilist[i]->SetImguiNumber(0);
		}
		if (imguilist[i]->GetSelectThis())
		{
			enemys[i]->SetColors({1.f, 1.f, 1.f, 1.f});
		}
		else
		{
			enemys[i]->SetColors({1.f, 0.f, 0.f, 1.f});
		}
		enemys[i]->SetPosition({imguilist[i]->GetPos().x, enemys[i]->GetPosition().y, imguilist[i]->GetPos().z});
		enemys[i]->SetScale({imguilist[i]->GetScl(), imguilist[i]->GetScl(), imguilist[i]->GetScl()});
	}
}

void PlaceEnemy::Draw()
{
	Obj->PreDraw();
	Obj->Draw();
	Obj->PostDraw();
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		if (enemy != nullptr)
		{
			enemy->Draw();
		}
	}
}

void PlaceEnemy::ImGui_Draw()
{
	ImGui::StyleColorsLight();
	ImGui::Begin("SelectEnemy");
	ImGui::SetWindowPos(ImVec2(0, 500));
	ImGui::SetWindowSize(ImVec2(300, 300));


	if (ImGui::Button("AlphaEnemy", ImVec2(90, 50)))
	{
		ArgmentFlag = true;

		Number.push_back(1);
	}
	ImGui::SameLine();
	if (ImGui::Button("BetaEnemy", ImVec2(90, 50)))
	{
		BossArgmentFlag = true;
		Number.push_back(2);
	}
	ImGui::SameLine();
	if (ImGui::Button("CammaEnemy", ImVec2(90, 50)))
	{
		BetaArgmentFlag = true;
		Number.push_back(3);
	}
	if (ImGui::Button("OpenCsv", ImVec2(90, 50)))
	{
		OpenCsvFile(_T("enemy.csv"));
	}


	ImGui::SliderFloat("posX", &pos.x, -500, 500);
	ImGui::SliderFloat("posY", &pos.y, -300, 300);
	ImGui::SliderFloat("posZ", &pos.z, -800, 800);


	static float col1[3] = {1.0f, 0.0f, 0.2f};
	//ImGui::SameLine();
	ImGui::ColorPicker3("color 1", col1);
	ImGui::ShowFontSelector("Rd");
	for (int i = 0; i < imguilist.size(); i++)
	{
		if (imguilist[i] == nullptr)
		{
			continue; // {
		}
		if (imguilist[i]->GetDelF())
		{
			enemys.erase(std::cbegin(enemys) + i);
			SelThis.erase(std::cbegin(SelThis) + i);
			enumbers.erase(std::cbegin(enumbers) + i);
			imguilist.erase(std::cbegin(imguilist) + i); // erase(std::cbegin(enemys) + i);
			//continue;
		}
	}


	ImGui::End();

	ImGui::Begin("EnemyList");
	ImGui::SetWindowPos(ImVec2(300, 500));
	ImGui::SetWindowSize(ImVec2(300, 300));

	for (int i = 0; i < imguilist.size(); i++)
	{
		if (imguilist[i] == nullptr)
		{
			continue;
		}

		if (enemys[i] == nullptr)
		{
			continue;
		}
		imguilist[i]->CreateImguiList();
		if (enumbers[i] != AXEGOLEM && imguilist[i]->GetEnumber() == createimgui::AXEGOLEM)
		{
			XMFLOAT3 oldpos = enemys[i]->GetPosition();
			enemys[i].reset(new MobEnemy());
			enemys[i]->Initialize();
			enemys[i]->SetPosition(oldpos);
			enumbers[i] = (AXEGOLEM);
		}
		if (enumbers[i] != LIZARD && imguilist[i]->GetEnumber() == createimgui::LIZARD)
		{
			XMFLOAT3 oldpos = enemys[i]->GetPosition();
			enemys[i].reset(new EnemyAlpha());
			enemys[i]->Initialize();
			enumbers[i] = (LIZARD);
			enemys[i]->SetPosition(oldpos);
		}
		if (enumbers[i] != THROWGOLEM && imguilist[i]->GetEnumber() == createimgui::THROWGOLEM)
		{
			XMFLOAT3 oldpos = enemys[i]->GetPosition();

			enemys[i].reset(new EnemyBeta());
			enemys[i]->Initialize();
			enumbers[i] = (THROWGOLEM);
			enemys[i]->SetPosition(oldpos);
		}
	}
	ImGui::End();
}

PlaceEnemy::createimgui::createimgui(std::string num, float scl, XMFLOAT3 pos)
{
	Scl = scl;
	Pos = pos;
	listnum.push_back("Enemy" + num);
}

void PlaceEnemy::createimgui::CreateImguiList()
{
	std::string TitName = std::to_string(imnumber) + "----------------------";
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), TitName.c_str());
	//ImGui::StyleColorsClassic();

	if (ImGui::CollapsingHeader(TitName.c_str()))
	{
		ImGui::Text("Scale");
		std::string sclname = "Scl" + TitName;
		ImGui::SliderFloat(sclname.c_str(), &Scl, 0.f, 1.f);

		ImGui::Text("Position");

		float pos[3] = {Pos.x, Pos.y, Pos.z};
		std::string posname_x = "Pos.x" + TitName;
		std::string posname_z = "Pos.z" + TitName;
		ImGui::SliderFloat(posname_x.c_str(), &Pos.x, -800.f, 800.f);
		ImGui::SliderFloat(posname_z.c_str(), &Pos.z, -700.f, 700.f);

		ImGui::Text("Rotation");
		std::string rotname = "Rot" + TitName;
		ImGui::SliderFloat(rotname.c_str(), &Rot.y, 0.f, 360.f);

		ImGui::Text("SelectEnemy");
		std::string enumynum_g = "AxeGolem" + TitName;
		std::string enumynum_l = "Lizard" + TitName;
		std::string enumynum_t = "ThrowGolem" + TitName;

		if (ImGui::Button(enumynum_g.c_str(), ImVec2(70, 30)))
		{
			enumber = AXEGOLEM;
		}
		ImGui::SameLine();
		if (ImGui::Button(enumynum_l.c_str(), ImVec2(70, 30)))
		{
			enumber = LIZARD;
		}
		ImGui::SameLine();
		if (ImGui::Button(enumynum_t.c_str(), ImVec2(70, 30)))
		{
			enumber = THROWGOLEM;
		}

		ImGui::Text("HP");
		ImGui::SliderInt("HP_Value", &Hp, 100, 300);

		ImGui::Text("DeleteInstance");
		std::string delname = "Delete" + TitName;
		if (ImGui::Button(delname.c_str(), ImVec2(100, 30)))
		{
			Del = true;
		}

		std::string selname = "SelectThis" + TitName;
		ImGui::Checkbox(selname.c_str(), &Select);
	}
}


void PlaceEnemy::Finalize()
{
	enemys.clear();
	Destroy(Obj);
}
