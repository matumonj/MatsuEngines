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
CreateObjManager* MapCreate::placeobj[] = {};
 bool MapCreate::savef = false;
MapCreate::MapCreate()
{
	
}

void MapCreate::ObjectInitialize(DebugCamera*camera)
{
	placeobj[0] = new PlaceFence();//柵
	placeobj[1] = new PlaceWood();//木
	placeobj[2] = new PlaceEnemy();//敵

	for (int i = 0; i < ObjType_Num; i++) {
		placeobj[i]->Initialize(camera);//モデル用意、初期化
	}
}
void MapCreate::ObjectUpdate(DebugCamera* camera)
{
	for (int i = 0; i < ObjType_Num; i++) {
		placeobj[i]->Update(camera);//モデル更新
	}
}

void MapCreate::ObjectArgment(DebugCamera* camera)
{
	if (savef) {
		for (int i = 0; i < ObjType_Num; i++) {
			placeobj[i]->FileWriting();//csvへの書き込み
		}
	}
	for (int i = 0; i < ObjType_Num; i++) {
		placeobj[i]->ArgMent(camera);//配置オブジェクトの設置、更新
	}
}

void MapCreate::ImGuiDraw()
{	//ImGui描画周り
	for (int i = 0; i < ObjType_Num; i++) {
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
	for (int i = 0; i < ObjType_Num; i++) {
		placeobj[i]->Draw();//モデル、設置物の描画
	}
}

