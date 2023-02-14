#include "PlaceFootSwitch.h"
#include"imgui.h"

void PlaceFootSwitch::Initialize(DebugCamera* camera)
{
	//	Model = Model::CreateFromOBJ("Fence");
	Obj = Object3d::Create(camera);
	Obj->SetModel(Model::CreateFromOBJ("FootSwitch"));
scl = { 3, 3, 3};}

void PlaceFootSwitch::FileWriting()
{
	
	file.open("fence.csv");

	popcom << file.rdbuf();

	file.close();
	///std::ofstream pofs("EnemyParam_CSV/position.csv");
	std::ofstream ofs("Param_CSV/Gimmic.csv"); // ファイルパスを指定する
	
	for (int i = 0; i < SwitchSize; i++)
	{
		ofs << "POSITION" << "," << switchs[i]->GetPosition().x
			<< "," << switchs[i]->GetPosition().y
			<< "," << switchs[i]->GetPosition().z << std::endl;
	}
}

bool PlaceFootSwitch::ErrorJudg()
{
	return false;
}

void PlaceFootSwitch::ArgMent(DebugCamera* camera)
{
	if (ArgmentFlag)
	{
		std::unique_ptr<Object3d> newFence;
		if (ArgmentFlag)
		{
			newFence = std::make_unique<Object3d>();
		}

		newFence->Initialize(camera);
		newFence->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SWITCH));
		newFence->SetPosition(pos);
		scl = { 3, 3, 3 };
		newFence->SetScale(scl);
		newFence->SetColor({ 1,1,1,1 });
		switchs.push_back(std::move(newFence));
		ArgmentFlag = false;
	}
	for (std::unique_ptr<Object3d>& Fence : switchs)
	{
		if (Fence != nullptr)
		{
			Fence->Update(camera);
		}
	}
	if (DeleteFlag && switchs.size() > 1)
	{
		switchs.pop_back();
		DeleteFlag = false;
	}
}

void PlaceFootSwitch::Update(DebugCamera* camera)
{
	Obj->SetPosition(pos);
	Obj->SetRotation(rot);
	Obj->SetScale(scl);
	Obj->SetColor({ 1,1,1,1 });
	Obj->Update(camera);
}

void PlaceFootSwitch::Draw()
{
	Object3d::PreDraw();
	Obj->Draw();
	
	for(int i=0;i<switchs.size();i++)
	{
		switchs[i]->Draw();
	}Object3d::PostDraw();

}

void PlaceFootSwitch::ImGui_Draw()
{
	ImGui::Begin("FootSwitch");
	ImGui::SetWindowPos(ImVec2(1600, 500));
	ImGui::SetWindowSize(ImVec2(300, 300));

	{
		ImGui::SliderFloat("posX", &pos.x, -500, 500);
		ImGui::SliderFloat("posY", &pos.y, -300, 300);
		ImGui::SliderFloat("posZ", &pos.z, -800, 800);
	}
	if (ImGui::Button("OpenCsv", ImVec2(90, 50)))
	{
		OpenCsvFile(_T("CharaParam\\Player_Param.csv"));
	}
	if (ImGui::Button("Switch", ImVec2(90, 50)))
	{
		ArgmentFlag = true;
	}
	
	ImGui::End();
}

void PlaceFootSwitch::Finalize()
{
	switchs.clear();
	Destroy(Obj);
}

void PlaceFootSwitch::ColField()
{
	
}
