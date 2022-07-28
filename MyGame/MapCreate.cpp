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
	//�����ɐݒu��new���Ă�������
	if (placeobj.size() == 0) {//create->playscene->create�Ƃ������Ƃ��ɓ��new�����̖h��
		placeobj.push_back(new PlaceFence());//��
		placeobj.push_back(new PlaceWood());//��
		placeobj.push_back(new PlaceEnemy());//�G
	}
	for (int i = 0; i < placeobj.size(); i++){
		placeobj[i]->Initialize(camera);
}//���f���p�ӁA������
}
void MapCreate::ObjectUpdate(DebugCamera* camera)
{
	for (int i = 0; i < placeobj.size(); i++){
		placeobj[i]->Update(camera);//���f���X�V
}
}

void MapCreate::ObjectArgment(DebugCamera* camera)
{
	if (savef) {
		for (int i = 0; i < placeobj.size(); i++) {
			placeobj[i]->FileWriting();//csv�ւ̏�������
		}
	}
	for (int i = 0; i < placeobj.size(); i++) {
		placeobj[i]->ArgMent(camera);//�z�u�I�u�W�F�N�g�̐ݒu�A�X�V
	}
}

void MapCreate::ImGuiDraw()
{	//ImGui�`�����
	for (int i = 0; i < placeobj.size(); i++){
		placeobj[i]->ImGui_Draw();
	}
	ImGui::Begin("SaveButton");
	if (ImGui::Button("Save", ImVec2(90, 50))) {
		savef = true;//�I�u�W�F�N�g�̃p�����[�^�ۑ��t���O
	}
	ImGui::End();
}
void MapCreate::ObjectDraw()
{
	for (int i = 0; i < placeobj.size(); i++){
		placeobj[i]->Draw();//���f���A�ݒu���̕`��
}
}

