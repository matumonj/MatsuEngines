#include "MapCreate.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include "PhysicsAction.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include"StayJudgment.h"
#include"StayAction.h"

#include"PlaceFence.h"
#include"PlaceWood.h"
#include"PlaceEnemy.h"
std::vector<CreateObjManager*> MapCreate::placeobj = {};
 bool MapCreate::savef = false;
MapCreate::MapCreate()
{
	
}

void MapCreate::ObjectInitialize(DebugCamera*camera)
{
	//ここに設置物newしていくだけ
	placeobj.push_back(new PlaceFence());//柵
	placeobj.push_back(new PlaceWood());//木
	placeobj.push_back(new PlaceEnemy());//敵
	
	for (auto obj :placeobj)
		obj->Initialize(camera);//モデル用意、初期化
}
void MapCreate::ObjectUpdate(DebugCamera* camera)
{
	for (auto obj : placeobj)
		obj->Update(camera);//モデル更新
}

void MapCreate::ObjectArgment(DebugCamera* camera)
{
	if (savef) {
		for (auto obj : placeobj)
			obj->FileWriting();//csvへの書き込み
	}
	for (auto obj : placeobj) 
		obj->ArgMent(camera);//配置オブジェクトの設置、更新
}

void MapCreate::ImGuiDraw()
{	//ImGui描画周り
	for (auto obj : placeobj) {
		obj->ImGui_Draw();
	}
	ImGui::Begin("SaveButton");
	if (ImGui::Button("Save", ImVec2(90, 50))) {
		savef = true;//オブジェクトのパラメータ保存フラグ
	}
	ImGui::End();
}
void MapCreate::ObjectDraw()
{
	for (auto obj : placeobj )
		obj->Draw();//モデル、設置物の描画
}

