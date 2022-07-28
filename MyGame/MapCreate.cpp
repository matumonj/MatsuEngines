#include "MapCreate.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include"PlaceFence.h"
#include"PlaceWood.h"
#include"PlaceEnemy.h"

MapCreate::MapCreate()
{
	
}

MapCreate* MapCreate::GetInstance()
{
	static MapCreate instance;
	return &instance;
}

void MapCreate::ObjectInitialize(DebugCamera* camera)
{
	//ここに設置物newしていくだけ
	if (placeobj.size() == 0) {//create->playscene->createといったときに二回newされるの防ぐ
		placeobj.push_back(new PlaceFence());//柵
		placeobj.push_back(new PlaceWood());//木
		placeobj.push_back(new PlaceEnemy());//敵
	}
	for (int i = 0; i < placeobj.size(); i++){
		placeobj[i]->Initialize(camera);
}//モデル用意、初期化
}
void MapCreate::ObjectUpdate(DebugCamera* camera)
{
	for (int i = 0; i < placeobj.size(); i++){
		placeobj[i]->Update(camera);//モデル更新
}
}

void MapCreate::ObjectArgment(DebugCamera* camera)
{
	if (savef) {
		for (int i = 0; i < placeobj.size(); i++) {
			placeobj[i]->FileWriting();//csvへの書き込み
		}
	}
	for (int i = 0; i < placeobj.size(); i++) {
		placeobj[i]->ArgMent(camera);//配置オブジェクトの設置、更新
	}
}

void MapCreate::ImGuiDraw()
{	//ImGui描画周り
	for (int i = 0; i < placeobj.size(); i++){
		placeobj[i]->ImGui_Draw();
	}
	ImGui::Begin("SaveButton");
	if (ImGui::Button("Save", ImVec2(90, 50))) {
		savef = true;//オブジェクトのパラメータ保存フラグ
	}
	ImGui::End();
}
void MapCreate::ObjectDraw()
{
	for (int i = 0; i < placeobj.size(); i++){
		placeobj[i]->Draw();//モデル、設置物の描画
}
}

