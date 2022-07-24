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
	placeobj[0] = new PlaceFence();
	placeobj[1] = new PlaceWood();
	placeobj[2] = new PlaceEnemy();

	for (int i = 0; i < ObjType_Num; i++) {
		placeobj[i]->Initialize(camera);
	}
}
void MapCreate::ObjectUpdate(DebugCamera* camera)
{
	for (int i = 0; i < ObjType_Num; i++) {
		placeobj[i]->Update(camera);
	}
}

void MapCreate::ObjectArgment(DebugCamera* camera)
{
	if (savef) {
		for (int i = 0; i < ObjType_Num; i++) {
			placeobj[i]->FileWriting();
		}
	}
	for (int i = 0; i < ObjType_Num; i++) {
		placeobj[i]->ArgMent(camera);
	}
}

void MapCreate::ImGuiDraw()
{
	for (int i = 0; i < ObjType_Num; i++) {
		placeobj[i]->ImGui_Draw();
	}
	ImGui::Begin("SaveButton");
	if (ImGui::Button("Save", ImVec2(90, 50))) {
		savef = true;
	}
	ImGui::End();
}
void MapCreate::ObjectDraw()
{
	for (int i = 0; i < ObjType_Num; i++) {
		placeobj[i]->Draw();
	}
}

