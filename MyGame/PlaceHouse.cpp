#include "PlaceHouse.h"
#include "PlaceHouse.h"
#include"imgui.h"

void PlaceHouse::Initialize(DebugCamera* camera)
{
	//Model = Model::CreateFromOBJ("huse");
	Obj = Object3d::Create(camera);
	Obj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::HOUSE));
}

void PlaceHouse::FileWriting()
{
	file.open("house.csv");
	popcom << file.rdbuf();

	file.close();
	///std::ofstream pofs("EnemyParam_CSV/position.csv");
	std::ofstream ofs("Param_CSV/house.csv"); // ファイルパスを指定する
	ofs << "house_Quantity" << "," << houses.size() << std::endl;

	for (int i = 0; i < houses.size(); i++)
	{
		ofs << "POP" << "," << houses[i]->GetPosition().x
			<< "," << houses[i]->GetPosition().y
			<< "," << houses[i]->GetPosition().z << std::endl;
		ofs << "ROTATION" << "," << houses[i]->GetRotation().x
			<< "," << houses[i]->GetRotation().y
			<< "," << houses[i]->GetRotation().z << std::endl;
		ofs << "SCALE" << "," << houses[i]->GetScale().x
			<< "," << houses[i]->GetScale().y
			<< "," << houses[i]->GetScale().z << std::endl;
	}
}

bool PlaceHouse::ErrorJudg()
{
	return false;
}

void PlaceHouse::ArgMent(DebugCamera* camera)
{
	if (ArgmentFlag)
	{
		std::unique_ptr<House> newHouse;
		if (ArgmentFlag)
		{
			newHouse = std::make_unique<House_A>();
		}

		newHouse->Initialize();
		newHouse->SetPosition(pos);
		newHouse->SetRotation(rot);
		houses.push_back(std::move(newHouse));
		ArgmentFlag = false;
	}
	for (std::unique_ptr<House>& huse : houses)
	{
		if (huse != nullptr)
		{
			huse->SetColor({1, 1, 1, 1});
			huse->Update();
			huse->CollisionField();
		}
	}
	if (DeleteFlag && houses.size() > 1)
	{
		houses.pop_back();
		DeleteFlag = false;
	}
}

void PlaceHouse::Update(DebugCamera* camera)
{
	Obj->SetPosition(pos);
	Obj->SetRotation(rot);
	Obj->SetScale({1.5, 1.5, 2.2});
	Obj->Update({1, 0, 0, 0.5}, camera);
}

void PlaceHouse::Draw()
{
	for (std::unique_ptr<House>& huse : houses)
	{
		if (huse != nullptr)
		{
			//enemy->PreDraw();
			huse->Draw();
			//enemy->PostDraw();
		}
	}
	Obj->PreDraw();
	Obj->Draw();
	Obj->PostDraw();
}

void PlaceHouse::ImGui_Draw()
{
	ImGui::Begin("House");
	ImGui::SetWindowPos(ImVec2(600, 500));
	ImGui::SetWindowSize(ImVec2(300, 300));

	if (ImGui::Button("House", ImVec2(90, 50)))
	{
		ArgmentFlag = true;
	}
	ImGui::SameLine();

	if (ImGui::Button("DeleteObj", ImVec2(90, 50)))
	{
		DeleteFlag = true;
	}

	{
		ImGui::SliderFloat("posX", &pos.x, -500, 500);
		ImGui::SliderFloat("posY", &pos.y, -300, 300);
		ImGui::SliderFloat("posZ", &pos.z, -800, 800);
	}
	{
		ImGui::Text("Rotation");
		ImGui::SliderFloat("rotX", &rot.x, 0, 360);
		ImGui::SliderFloat("rotY", &rot.y, 0, 360);
		ImGui::SliderFloat("rotZ", &rot.z, 0, 360);
	}

	ImGui::End();
}

void PlaceHouse::Finalize()
{
	Destroy(Obj);
	houses.clear();
}
