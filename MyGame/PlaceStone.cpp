#include "PlaceStone.h"

void PlaceStone::Initialize(DebugCamera* camera)
{
	//Model = Model::CreateFromOBJ("wood");
	Obj = Object3d::Create(camera);
	Obj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGROCK_A));

	scl = {2, 3, 2};
}

void PlaceStone::FileWriting()
{
	file.open("stone.csv");
	popcom << file.rdbuf();

	file.close();
	//std::ofstream pofs("EnemyParam_CSV/position.csv");
	std::ofstream ofs("Param_CSV/stone.csv"); // ファイルパスを指定する
	ofs << "Stone_Quantity" << "," << stones.size() << std::endl;

	for (int i = 0; i < stones.size(); i++)
	{
		ofs << "Number" << "," << Number[i] << std::endl;
		ofs << "POP" << "," << stones[i]->GetPosition().x
			<< "," << stones[i]->GetPosition().y
			<< "," << stones[i]->GetPosition().z << std::endl;
		ofs << "ROTATION" << "," << stones[i]->GetRotation().x
			<< "," << stones[i]->GetRotation().y
			<< "," << stones[i]->GetRotation().z << std::endl;
		ofs << "SCALE" << "," << stones[i]->GetScale().x
			<< "," << stones[i]->GetScale().y
			<< "," << stones[i]->GetScale().z << std::endl;
	}
}

bool PlaceStone::ErrorJudg()
{
	return false;
}

void PlaceStone::ArgMent(DebugCamera* camera)
{
	if (ArgmentFlag)
	{
		std::unique_ptr<Stone> newStone;
		if (ArgmentFlag)
		{
			newStone = std::make_unique<Stone_A>();
		}
		if (SubRockF)
		{
			newStone = std::make_unique<Stone_B>();
		}
		newStone->Initialize();
		newStone->SetPosition(pos);
		newStone->SetRotation(rot);
		newStone->SetScale(scl);
		stones.push_back(std::move(newStone));
		if (ArgmentFlag)
		{
			Number.push_back(1);
		}
		if (SubRockF)
		{
			Number.push_back(2);
		}
		ArgmentFlag = false;
		SubRockF = false;
	}
	for (std::unique_ptr<Stone>& wood : stones)
	{
		if (wood != nullptr)
		{
			wood->SetColor({1, 1, 1, 1});
			wood->Update();
			wood->CollisionField();
		}
	}
	if (DeleteFlag && stones.size() > 1)
	{
		stones.pop_back();
		DeleteFlag = false;
	}
}

void PlaceStone::Update(DebugCamera* camera)
{
	Obj->SetPosition(pos);
	Obj->SetRotation(rot);
	Obj->SetScale(scl);
	//	Obj->SetScale({ 2,3,2 });

	Obj->Update({1, 0, 1, 0.5}, camera);
}

void PlaceStone::Draw()
{
	for (std::unique_ptr<Stone>& stone : stones)
	{
		if (stone != nullptr)
		{
			//enemy->PreDraw();
			stone->Draw();
			//enemy->PostDraw();
		}
	}
	Obj->PreDraw();
	Obj->Draw();
	Obj->PostDraw();
}

#include"imgui.h"

void PlaceStone::ImGui_Draw()
{
	ImGui::Begin("SelectStone");
	ImGui::SetWindowPos(ImVec2(300, 500));
	ImGui::SetWindowSize(ImVec2(300, 300));

	if (ImGui::Button("StoneA", ImVec2(90, 50)))
	{
		ArgmentFlag = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("StoneB", ImVec2(90, 50)))
	{
		SubRockF = true;
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
		ImGui::SliderFloat("posX", &pos.x, -500, 500);
		ImGui::SliderFloat("posY", &pos.y, -300, 300);
		ImGui::SliderFloat("posZ", &pos.z, -800, 800);
	}
	{
		ImGui::SliderFloat("rotX", &rot.x, 0, 360);
		ImGui::SliderFloat("rotY", &rot.y, 0, 360);
		ImGui::SliderFloat("rotZ", &rot.z, 0, 360);
	}
	{
		ImGui::SliderFloat("sclX", &scl.x, 0, 20);
		ImGui::SliderFloat("sclY", &scl.y, 0, 20);
		ImGui::SliderFloat("sclZ", &scl.z, 0, 20);
	}
	ImGui::End();
}

void PlaceStone::Finalize()
{
	Destroy(Obj);
	for (int i = 0; i < stones.size(); i++)
	{
		Destroy_unique(stones[i]);
	}
	stones.clear();
}
