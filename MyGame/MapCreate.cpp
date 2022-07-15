#include "MapCreate.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include "PhysicsAction.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include"StayJudgment.h"
#include"StayAction.h"
 std::ifstream MapCreate::file;
 std::stringstream MapCreate::popcom;
std::vector<MapCreate::XMFLOAT3> MapCreate::EnemyPosition;
bool MapCreate::MobArgment;
bool MapCreate::BossArgment;
bool MapCreate::EnemyDelete;
std::vector<std::unique_ptr<Enemy>> MapCreate::enemys;
//デバッグ用
std::vector <int>MapCreate::randmove;
std::vector <int>MapCreate::randmovement;
MapCreate::XMFLOAT3 MapCreate::pos;
float MapCreate::a;
FILE* MapCreate::fp;
bool MapCreate::savef;
bool MapCreate::MoveFlags;
std::vector<int> MapCreate::Number;
Model* MapCreate::BoxModel;
Object3d* MapCreate::BoxObj;
BehaviorTree MapCreate::behavior;
MapCreate::MapCreate()
{
	
}
void MapCreate::CreateMapMode_MoveCamera(DebugCamera*camera,XMFLOAT3 map)
{
	camera->SetTarget({ map.x,map.y + 20,map.z });
	camera->SetTarget(map);
}

void MapCreate::SetBoxModel(DebugCamera*camera)
{
	behavior.AddNode("", "Root", 0, BehaviorTree::SELECT_RULE::PRIORITY, NULL, NULL);
	//behavior.AddNode("Root", "Attack", 1, BehaviorTree::SELECT_RULE::RANDOM, AttackJudgment::GetInstance(), NULL);
	behavior.AddNode("Root", "Walk", 2, BehaviorTree::SELECT_RULE::NON, WalkJudgment::GetInstance(), WalkAction::GetInstance());
	
	BoxModel = Model::CreateFromOBJ("box");
	BoxObj= Object3d::Create(camera);
	BoxObj->SetModel(BoxModel);
	
}
void MapCreate::UpdateBoxModel(DebugCamera* camera)
{
	BoxObj->SetPosition(pos);
	BoxObj->SetScale({ 5,5,5 });
	BoxObj->Update({ 1,0,0,0.5 }, camera);
}

void MapCreate::DrawBoxModel()
{
	BoxObj->PreDraw();
	BoxObj->Draw();
	BoxObj->PostDraw();
}
void MapCreate::LoadEnemyParam()
{
	file.open("open.csv");

	popcom << file.rdbuf();

	file.close();
}
void MapCreate::EnemyArgment(DebugCamera* camera)
{
	if (savef) {

		LoadEnemyParam();
		std::ofstream pofs("EnemyParam_CSV/position.csv");
		std::ofstream ofs("EnemyParam_CSV/open.csv");  // ファイルパスを指定する
		ofs << "Enemy_Quantity" << "," << enemys.size() << std::endl;

		for (int i = 0; i < enemys.size(); i++) {
			ofs << "Wait" << "," << 120 << std::endl;
			ofs << "Number" << "," << Number[i] << std::endl;
			ofs << "POP" << "," << enemys[i]->GetEnemyPosition().x
				<< "," << enemys[i]->GetEnemyPosition().y
				<< "," << enemys[i]->GetEnemyPosition().z << std::endl;
			
		}
		EnemyPosition.resize(enemys.size());
		Number.resize(enemys.size());
		
		//fwrite(&EnemyPosition, sizeof(XMFLOAT3),EnemyPosition.size(), fp);
	}
	if (MobArgment||BossArgment) {
		
		std::unique_ptr<Enemy>newEnemy;
		if (MobArgment) {
			newEnemy = std::make_unique<MobEnemy>(&behavior, 100.0f, 100.0f, 30.0f, 10.0f);
		}
		if (BossArgment) {
			newEnemy = std::make_unique<BossEnemy>(&behavior, 100.0f, 100.0f, 30.0f, 10.0f);
		}
		newEnemy->Initialize(camera);
		newEnemy->SetEnemyPosition(pos);
		enemys.push_back(std::move(newEnemy));
		MobArgment = false;
		BossArgment = false;
	}
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		if (enemy != nullptr) {
			if (!MoveFlags) {
				//enemy->SetMoveFlag(false);
			} else {
				enemy->SetMoveFlag(true);
			}
			enemy->Update({ 1,1,1,1 }, camera);
			//enemy->CollisionField(camera);
		}
	}
	if ( EnemyDelete&&enemys.size()>1) {
		enemys.pop_back();
		Number.pop_back();
		EnemyDelete = false;
	}
	
}

void MapCreate::EnemyDraw()
{
		for (std::unique_ptr<Enemy>& enemy : enemys) {
			if (enemy != nullptr) {
			//enemy->PreDraw();
			enemy->Draw();
			//enemy->PostDraw();
		}
	}
}

void MapCreate::ImguiDraw_Enemy()
{
	ImGui::Begin("SelectEnemy");
	if (ImGui::Button("AlphaEnemy", ImVec2(90, 50))) {
		MobArgment = true;
		Number.push_back(1);
	}
	ImGui::SameLine();
	if (ImGui::Button("BetaEnemy", ImVec2(90, 50))) {
		BossArgment = true;
		Number.push_back(2);
	}
	ImGui::SameLine();
	if (ImGui::Button("CammaEnemy", ImVec2(90, 50))) {
	}
	ImGui::SameLine();
	

	if (ImGui::Button("DeleteObj", ImVec2(90, 50))) {
		
			EnemyDelete = true;
		}
	const char* l[] = { "aaa","bbb","ccc","ddd" };
	static int lc = 1;
	ImGui::ListBox("list\n(ki)", &lc, l, IM_ARRAYSIZE(l), 4);
	{
		ImGui::SliderFloat("posX", &pos.x, -300, 300);
		ImGui::SliderFloat("posY", &pos.y, -300, 300);
		ImGui::SliderFloat("posZ", &pos.z, -300, 300);
		
	}

		if (ImGui::Button("Save", ImVec2(90, 50))) {
			savef = true;
		}
		ImGui::Checkbox("Move", &MoveFlags);

	ImGui::End();

	ImGui::Begin("Scene");

}