#include "PlaceGrassField.h"


void PlaceGrassField::Initialize(DebugCamera* camera)
{
	//Model = Model::CreateFromOBJ("wood");
	Obj = Object3d::Create(camera);
	Obj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::GRASSFIELD));

	scl = { 2,3,2 };
}
void PlaceGrassField::FileWriting()
{
	file.open("grass.csv");
	popcom << file.rdbuf();

	file.close();
	//std::ofstream pofs("EnemyParam_CSV/position.csv");
	std::ofstream ofs("Param_CSV/grass.csv");  // ファイルパスを指定する
	ofs << "Grass_Quantity" << "," << grassfields.size() << std::endl;

	for (int i = 0; i < grassfields.size(); i++) {
		ofs << "Number" << "," << Number[i] << std::endl;
		ofs << "POP" << "," << grassfields[i]->GetPosition().x
			<< "," << grassfields[i]->GetPosition().y
			<< "," << grassfields[i]->GetPosition().z << std::endl;
		ofs << "ROTATION" << "," << grassfields[i]->GetRotation().x
			<< "," << grassfields[i]->GetRotation().y
			<< "," << grassfields[i]->GetRotation().z << std::endl;
		ofs << "SCALE" << "," << grassfields[i]->GetScale().x
			<< "," << grassfields[i]->GetScale().y
			<< "," << grassfields[i]->GetScale().z << std::endl;

	}
}

bool PlaceGrassField::ErrorJudg()
{
	return false;
}
void PlaceGrassField::ArgMent(DebugCamera* camera)
{
	if (ArgmentFlag) {
		std::unique_ptr<GrassField>newStone;
		if (ArgmentFlag) {
			newStone = std::make_unique<GrassField>();
		}
		newStone->Initialize(camera);
		newStone->SetPosition(pos);
		newStone->SetRotation(rot);
		newStone->SetScale(scl);
		grassfields.push_back(std::move(newStone));
		if (ArgmentFlag) {
			Number.push_back(1);
		}
		if (SubRockF) {
			Number.push_back(2);
		}
		ArgmentFlag = false;
		SubRockF = false;

	}
	for (std::unique_ptr<GrassField>& wood : grassfields) {
		if (wood != nullptr) {
			wood->Update(camera);
			//wood->CollisionField(camera);
		}
	}
	if (DeleteFlag && grassfields.size() > 1) {
		grassfields.pop_back();
		DeleteFlag = false;
	}
}
void PlaceGrassField::Update(DebugCamera* camera)
{
	Obj->SetPosition(pos);
	Obj->SetRotation(rot);
	Obj->SetScale(scl);
	//	Obj->SetScale({ 2,3,2 });

	Obj->Update({ 1,0,1,0.5 }, camera);
}

void PlaceGrassField::Draw()
{
	for (std::unique_ptr<GrassField>& stone : grassfields) {
		if (stone != nullptr) {
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
void PlaceGrassField::ImGui_Draw()
{
	ImGui::Begin("SelectGrass");
	ImGui::SetWindowPos(ImVec2(1200, 500));
	ImGui::SetWindowSize(ImVec2(300, 300));

	if (ImGui::Button("StoneA", ImVec2(90, 50))) {
		ArgmentFlag = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("StoneB", ImVec2(90, 50))) {
		SubRockF = true;
	}

	ImGui::SameLine();


	if (ImGui::Button("DeleteObj", ImVec2(90, 50))) {

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
	
	ImGui::End();
}

void PlaceGrassField::Finalize()
{
	Destroy(Obj);
	for (int i = 0; i < grassfields.size(); i++) {
		Destroy_unique(grassfields[i]);
	}
	grassfields.clear();
}