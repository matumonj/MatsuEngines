#include "PlaceFence.h"
#include"imgui.h"

void PlaceFence::Initialize(DebugCamera* camera)
{
	//	Model = Model::CreateFromOBJ("Fence");
	Obj = Object3d::Create(camera);
	Obj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::FENCE));
}

void PlaceFence::FileWriting()
{
	file.open("fence.csv");

	popcom << file.rdbuf();

	file.close();
	///std::ofstream pofs("EnemyParam_CSV/position.csv");
	std::ofstream ofs("Param_CSV/fence.csv"); // ファイルパスを指定する
	ofs << "Fence_Quantity" << "," << fences.size() << std::endl;

	for (int i = 0; i < fences.size(); i++)
	{
		ofs << "POP" << "," << fences[i]->GetPosition().x
			<< "," << fences[i]->GetPosition().y
			<< "," << fences[i]->GetPosition().z << std::endl;
		ofs << "ROTATION" << "," << fences[i]->GetRotation().x
			<< "," << fences[i]->GetRotation().y
			<< "," << fences[i]->GetRotation().z << std::endl;
		ofs << "SCALE" << "," << fences[i]->GetScale().x
			<< "," << fences[i]->GetScale().y
			<< "," << fences[i]->GetScale().z << std::endl;
	}
}

bool PlaceFence::ErrorJudg()
{
	return false;
}

void PlaceFence::ArgMent(DebugCamera* camera)
{
	if (ArgmentFlag)
	{
		std::unique_ptr<AreaFence> newFence;
		if (ArgmentFlag)
		{
			newFence = std::make_unique<AreaFence>();
		}

		newFence->Initialize();
		newFence->SetPosition(pos);
		newFence->SetRotation(rot);
		scl = {3, 3, 1};
		newFence->SetScale(scl);

		fences.push_back(std::move(newFence));
		ArgmentFlag = false;
	}
	for (std::unique_ptr<AreaFence>& Fence : fences)
	{
		if (Fence != nullptr)
		{
			Fence->SetColors({1, 1, 1, 1});
			Fence->Update();
			Fence->CollisionField();
		}
	}
	if (DeleteFlag && fences.size() > 1)
	{
		fences.pop_back();
		DeleteFlag = false;
	}
}

void PlaceFence::Update(DebugCamera* camera)
{
	Obj->SetPosition(pos);
	Obj->SetRotation(rot);
	Obj->SetScale(scl);
	Obj->Update({1, 0, 0, 0.5}, camera);
}

void PlaceFence::Draw()
{
	Obj->PreDraw();
	Obj->Draw();
	Obj->PostDraw();
	for (std::unique_ptr<AreaFence>& fence : fences)
	{
		if (fence != nullptr)
		{
			fence->Draw();
		}
	}
}

void PlaceFence::ImGui_Draw()
{
	ImGui::Begin("Player");
	ImGui::SetWindowPos(ImVec2(900, 500));
	ImGui::SetWindowSize(ImVec2(300, 300));

	if (ImGui::Button("OpenCsv", ImVec2(90, 50)))
	{
		OpenCsvFile(_T("CharaParam\\Player_Param.csv"));
	}
	ImGui::SameLine();
	{
		ImGui::Text("HP");
		ImGui::SliderInt("HPValue", &hp, 0, 500);
	}
	{
		ImGui::Text("RunSpeed");
		ImGui::SliderFloat("SpeedValue", &runspeed, 0.f, 6.0f);
	}
	{
		ImGui::Text("Scale");
		ImGui::SliderFloat("sclX", &scl.x, 0, 30);
		ImGui::SliderFloat("sclY", &scl.y, 0, 10);
		ImGui::SliderFloat("sclZ", &scl.z, 0, 10);
	}
	ImGui::End();
}

void PlaceFence::Finalize()
{
	fences.clear();
	Destroy(Obj);
}
