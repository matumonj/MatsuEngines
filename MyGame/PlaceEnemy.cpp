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
		if(Number[i]==1)
		{
			ofs << "//ゴーレム"<< std::endl;
		}
		else if(Number[i]==2)
		{
			ofs << "//トカゲ" << std::endl;
		}
		else if(Number[i]==3)
		{
			ofs << "//ミニゴーレム"<< std::endl;
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
	}
}

void PlaceEnemy::ArgMent(DebugCamera* camera)
{
	if (ArgmentFlag || BossArgmentFlag || BetaArgmentFlag)
	{
		std::unique_ptr<Enemy> newEnemy;
		if (ArgmentFlag)
		{
			newEnemy = std::make_unique<MobEnemy>();
		}
		if (BossArgmentFlag)
		{
			newEnemy = std::make_unique<EnemyAlpha>();
		}
		if (BetaArgmentFlag)
		{
			newEnemy = std::make_unique<EnemyBeta>();
		}
		newEnemy->Initialize();
		newEnemy->SetPosition(pos);
		enemys.push_back(std::move(newEnemy));

		std::unique_ptr<createimgui>newlist;
		newlist=std::make_unique<createimgui>(std::to_string(int(enemys.size())),0.02f,pos);
		imguilist.push_back(std::move(newlist));

		ArgmentFlag = false;
		BossArgmentFlag = false;
		BetaArgmentFlag = false;
	}
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		if (enemy != nullptr)
		{
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
	Obj->Update({1, 0, 0, 0.5}, camera);

	for (int i = 0; i < enemys.size(); i++)
	{
		if (enemys[i] == nullptr)continue;

		enemys[i]->SetPosition({ imguilist[i]->GetPos().x,enemys[i]->GetPosition().y, imguilist[i]->GetPos().z });
		enemys[i]->SetScale({ imguilist[i]->GetScl(),imguilist[i]->GetScl() ,imguilist[i]->GetScl() });
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
	if (ImGui::Button("DeleteObj", ImVec2(90, 50)))
	{
		enemys.pop_back();
		DeleteFlag = true;
	}
		ImGui::SliderFloat("posX", &pos.x, -500, 500);
		ImGui::SliderFloat("posY", &pos.y, -300, 300);
		ImGui::SliderFloat("posZ", &pos.z, -800, 800);

			for (int i = 0; i < imguilist.size(); i++)
			{
				if (imguilist[i] == nullptr)continue;// {
				if (imguilist[i]->GetDelF())
				{
					enemys.erase(std::cbegin(enemys)+i);
					imguilist.erase(std::cbegin(imguilist)+i);// erase(std::cbegin(enemys) + i);
					//continue;
				}
			}

		for (int i = 0; i < imguilist.size(); i++)
		{
			if (imguilist[i] == nullptr)continue;
			imguilist[i]->CreateImguiList();
			}
		ImGui::End();
}

PlaceEnemy::createimgui::createimgui(std::string num,float scl, XMFLOAT3 pos)
{
	Scl = scl;
	Pos = pos;
	listnum.push_back("Enemy"+num);
}

void PlaceEnemy::createimgui::CreateImguiList()
{
	std::string TitName = listnum.back() + "----------------------";
	ImGui::Text(TitName.c_str());
	std::string sclname = "Scl"+listnum.back();
	std::string posname_x = "Pos.x" + listnum.back();
	std::string posname_z = "Pos.z" + listnum.back();

	std::string rotname = "Rot" + listnum.back();

	std::string delname = "Delete" + listnum.back();
	float pos[3] = { Pos.x,Pos.y,Pos.z };
	ImGui::SliderFloat(sclname.c_str(), &Scl, 0.f, 1.f);

	ImGui::SliderFloat(posname_x.c_str(),&Pos.x ,-800.f,800.f );
	ImGui::SliderFloat(posname_z.c_str(), &Pos.z, -700.f, 700.f);

	ImGui::SliderFloat(rotname.c_str(), &Rot.y, 0.f, 360.f);

	if(ImGui::Button(delname.c_str(),ImVec2(100,30)))
	{
		Del = true;
	}
}
 


void PlaceEnemy::Finalize()
{
	enemys.clear();
	Destroy(Obj);
}
