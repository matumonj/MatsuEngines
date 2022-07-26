#include "PlaceEnemy.h"
#include "PlaceEnemy.h"
#include "PlaceEnemy.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include "PhysicsAction.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include"FollowJudgement.h"
#include"FollowAction.h"
void PlaceEnemy::Initialize(DebugCamera* camera)
{
	behavior.AddNode("", "Root", 0, BehaviorTree::SELECT_RULE::PRIORITY, NULL, NULL);
	
	Model = Model::CreateFromOBJ("box");
	Obj = Object3d::Create(camera);
	Obj->SetModel(Model);
}

void PlaceEnemy::FileWriting()
{
	file.open("open.csv");

	popcom << file.rdbuf();

	file.close();
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
	Position.resize(enemys.size());
	Number.resize(enemys.size());

	//fwrite(&EnemyPosition, sizeof(XMFLOAT3),EnemyPosition.size(), fp);
}

void PlaceEnemy::ArgMent(DebugCamera* camera)
{
	if (ArgmentFlag || BossArgmentFlag) {

		std::unique_ptr<Enemy>newEnemy;
		if (ArgmentFlag) {
			newEnemy = std::make_unique<MobEnemy>(&behavior, 100.0f, 100.0f, 30.0f, 10.0f);
		}
		if (BossArgmentFlag) {
			newEnemy = std::make_unique<BossEnemy>(&behavior, 100.0f, 100.0f, 30.0f, 10.0f);
		}
		newEnemy->Initialize(camera);
		newEnemy->SetEnemyPosition(pos);
		enemys.push_back(std::move(newEnemy));
		if (ArgmentFlag) {
			Number.push_back(1);
		}
		ArgmentFlag = false;
		BossArgmentFlag = false;
	}
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		if (enemy != nullptr) {
			enemy->Update({ 1,1,1,1 }, camera);
			//enemy->CollisionField(camera);
		}
	}
	if (DeleteFlag && enemys.size() > 1) {
		enemys.pop_back();
		Number.pop_back();
		DeleteFlag = false;
	}
}
void PlaceEnemy::Update(DebugCamera* camera)
{
	Obj->SetPosition(pos);
	Obj->SetScale({ 2,3,2 });
	Obj->Update({ 1,0,0,0.5 }, camera);
}

void PlaceEnemy::Draw()
{
	Obj->PreDraw();
	Obj->Draw();
	Obj->PostDraw();
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		if (enemy != nullptr) {
			//enemy->PreDraw();
			enemy->Draw();
			//enemy->PostDraw();
		}
	}
}

void PlaceEnemy::ImGui_Draw()
{
	ImGui::Begin("SelectEnemy");
	if (ImGui::Button("AlphaEnemy", ImVec2(90, 50))) {
		ArgmentFlag = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("BetaEnemy", ImVec2(90, 50))) {
		ArgmentFlag = true;
		Number.push_back(2);
	}
	ImGui::SameLine();
	if (ImGui::Button("CammaEnemy", ImVec2(90, 50))) {
	}
	ImGui::SameLine();


	if (ImGui::Button("DeleteObj", ImVec2(90, 50))) {
		DeleteFlag = true;
	}
	const char* l[] = { "aaa","bbb","ccc","ddd" };
	static int lc = 1;
	ImGui::ListBox("list\n(ki)", &lc, l, IM_ARRAYSIZE(l), 4);
	{
		ImGui::SliderFloat("posX", &pos.x, -300, 300);
		ImGui::SliderFloat("posY", &pos.y, -300, 300);
		ImGui::SliderFloat("posZ", &pos.z, -300, 300);

	}

}