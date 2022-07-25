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
	//�����ɐݒu��new���Ă�������
	placeobj.push_back(new PlaceFence());//��
	placeobj.push_back(new PlaceWood());//��
	placeobj.push_back(new PlaceEnemy());//�G
	
	for (auto obj :placeobj)
		obj->Initialize(camera);//���f���p�ӁA������
}
void MapCreate::ObjectUpdate(DebugCamera* camera)
{
	for (auto obj : placeobj)
		obj->Update(camera);//���f���X�V
}

void MapCreate::ObjectArgment(DebugCamera* camera)
{
	if (savef) {
		for (auto obj : placeobj)
			obj->FileWriting();//csv�ւ̏�������
	}
	for (auto obj : placeobj) 
		obj->ArgMent(camera);//�z�u�I�u�W�F�N�g�̐ݒu�A�X�V
}

void MapCreate::ImGuiDraw()
{	//ImGui�`�����
	for (auto obj : placeobj) {
		obj->ImGui_Draw();
	}
	ImGui::Begin("SaveButton");
	if (ImGui::Button("Save", ImVec2(90, 50))) {
		savef = true;//�I�u�W�F�N�g�̃p�����[�^�ۑ��t���O
	}
	ImGui::End();
}
void MapCreate::ObjectDraw()
{
	for (auto obj : placeobj )
		obj->Draw();//���f���A�ݒu���̕`��
}

