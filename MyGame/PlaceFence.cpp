#include "PlaceFence.h"
#include"imgui.h"
void PlaceFence::Initialize(DebugCamera* camera)
{
	Model = Model::CreateFromOBJ("Fence");
	Obj = Object3d::Create(camera);
	Obj->SetModel(Model);
}

void PlaceFence::FileWriting()
{
	file.open("fence.csv");

	popcom << file.rdbuf();

	file.close();
	///std::ofstream pofs("EnemyParam_CSV/position.csv");
	std::ofstream ofs("EnemyParam_CSV/fence.csv");  // ファイルパスを指定する
	ofs << "Fence_Quantity" << "," << fences.size() << std::endl;

	for (int i = 0; i < fences.size(); i++) {
		ofs << "POP" << "," << fences[i]->GetPosition().x
			<< "," << fences[i]->GetPosition().y
			<< "," << fences[i]->GetPosition().z << std::endl;

	}
	Position.resize(fences.size());
}

void PlaceFence::ArgMent(DebugCamera* camera)
{
	if (ArgmentFlag) {
		std::unique_ptr<AreaFence>newFence;
		if (ArgmentFlag) {
			newFence = std::make_unique<AreaFence>();
		}

		newFence->Initialize(camera);
		newFence->SetPosition(pos);
		fences.push_back(std::move(newFence));
		ArgmentFlag = false;
	}
	for (std::unique_ptr<AreaFence>& Fence : fences) {
		if (Fence != nullptr) {
			Fence->Update(camera);
		}
	}
	if (DeleteFlag && fences.size() > 1) {
		fences.pop_back();
		DeleteFlag = false;
	}
}
void PlaceFence::Update(DebugCamera* camera)
{
	Obj->SetPosition(pos);
	Obj->SetScale({10,10,10 });
	Obj->Update({ 1,0,0,0.5 }, camera);
}

void PlaceFence::Draw()
{
	Obj->PreDraw();
	Obj->Draw();
	Obj->PostDraw();
	for (std::unique_ptr<AreaFence>& fence : fences) {
		if (fence != nullptr) {
			//enemy->PreDraw();
			fence->Draw();
			//enemy->PostDraw();
		}
	}
}

void PlaceFence::ImGui_Draw()
{
	ImGui::Begin("Fence");
	if (ImGui::Button("Fence", ImVec2(90, 50))) {
		ArgmentFlag = true;
	}
	ImGui::SameLine();
	
	if (ImGui::Button("DeleteObj", ImVec2(90, 50))) {

		DeleteFlag = true;
	}

	{
		ImGui::SliderFloat("posX", &pos.x, -300, 300);
		ImGui::SliderFloat("posY", &pos.y, -300, 300);
		ImGui::SliderFloat("posZ", &pos.z, -300, 300);

	}
	ImGui::End();
}