#include "PlaceWood.h"
#include"imgui.h"
#include "Wood.h"
#include "WoodB.h"

void PlaceWood::Initialize(DebugCamera* camera)
{
	//Model = Model::CreateFromOBJ("wood");
	Obj = Object3d::Create(camera);
	Obj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WOOD));
}

void PlaceWood::FileWriting()
{
	file.open("wood.csv");
	popcom << file.rdbuf();

	file.close();
	//std::ofstream pofs("EnemyParam_CSV/position.csv");
	std::ofstream ofs("Param_CSV/wood.csv"); // ファイルパスを指定する
	ofs << "Wood_Quantity" << "," << woods.size() << std::endl;

	for (int i = 0; i < woods.size(); i++)
	{
		ofs << "Number" << "," << Number[i] << std::endl;
		ofs << "POP" << "," << woods[i]->GetPosition().x
			<< "," << woods[i]->GetPosition().y
			<< "," << woods[i]->GetPosition().z << std::endl;
	}
}

bool PlaceWood::ErrorJudg()
{
	const int Min_WoodSize = 4; //設置できる木の最小数
	const int Max_WoodSize = 50; //設置できる木の最大数

	if (woods.size() < Min_WoodSize || woods.size() > Max_WoodSize)
	{
		return true;
	}
	return false;
}

void PlaceWood::ArgMent(DebugCamera* camera)
{
	if (ArgmentFlag)
	{
		std::unique_ptr<WoodBase> newWood;
		newWood = std::make_unique<Wood>();

		newWood->Initialize();
		newWood->SetPosition(pos);
		woods.push_back(std::move(newWood));
		ArgmentFlag = false;
	}
	if (AWoodArgmentFlag)
	{
		std::unique_ptr<WoodBase> newWood;
		newWood = std::make_unique<WoodB>();

		newWood->Initialize();
		newWood->SetPosition(pos);
		woods.push_back(std::move(newWood));
		AWoodArgmentFlag = false;
	}
	for (std::unique_ptr<WoodBase>& wood : woods)
	{
		if (wood != nullptr)
		{
			wood->SetColors({1, 1, 1, 1});
			wood->Update();
			wood->CollisionField();
		}
	}
	if (DeleteFlag && woods.size() > 1)
	{
		woods.pop_back();
		Number.pop_back();
		DeleteFlag = false;
	}
}

void PlaceWood::Update(DebugCamera* camera)
{
	Obj->SetPosition(pos);
	Obj->SetScale({12.0f, 9.0f, 12.0f});

	Obj->Update(camera);
}

void PlaceWood::Draw()
{
	for (std::unique_ptr<WoodBase>& wood : woods)
	{
		if (wood != nullptr)
		{
			//enemy->PreDraw();
			wood->Draw();
			//enemy->PostDraw();
		}
	}
	Obj->PreDraw();
	Obj->Draw();
	Obj->PostDraw();
}

void PlaceWood::ImGui_Draw()
{
	ImGui::Begin("SelectWood");
	ImGui::SetWindowPos(ImVec2(1500, 500));
	ImGui::SetWindowSize(ImVec2(300, 300));

	if (ImGui::Button("Wood", ImVec2(90, 50)))
	{
		ArgmentFlag = true;
		Number.push_back(1);
	}
	ImGui::SameLine();
	if (ImGui::Button("B", ImVec2(90, 50)))
	{
		AWoodArgmentFlag = true;
		Number.push_back(2);
	}
	if (ImGui::Button("C", ImVec2(90, 50)))
	{
	}
	ImGui::SameLine();


	if (ImGui::Button("DeleteObj", ImVec2(90, 50)))
	{
		DeleteFlag = true;
	}

	{
		ImGui::Text("Position");
		ImGui::SliderFloat("posX", &pos.x, -500, 500);
		ImGui::SliderFloat("posY", &pos.y, -300, 300);
		ImGui::SliderFloat("posZ", &pos.z, -800, 800);
	}
	{
		ImGui::Text("Scale");
		ImGui::SliderFloat("sclX", &scl.x, 0, 8);
		ImGui::SliderFloat("sclY", &scl.y, 0, 8);
		ImGui::SliderFloat("sclZ", &scl.z, 0, 8);
	}
	ImGui::End();
}

void PlaceWood::Finalize()
{
	Destroy(Obj);
	for (int i = 0; i < woods.size(); i++)
	{
		Destroy_unique(woods[i]);
	}
	woods.clear();
}
