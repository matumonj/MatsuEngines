#include "MapCreate.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include"PlaceFence.h"
#include"PlaceWood.h"
#include"PlaceEnemy.h"
#include"PlaceChest.h"
#include"PlaceCameraPoints.h"
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
	//���ӁF�ʓ|�����Ǘv�f�̒ǉ��͗񋓌^�ɍ��킹��[FENCE=0,WOOD=1,ENEMY=2] errorWindow�����Ŏg����
	if (placeobj.size() == 0) {//create->playscene->create�Ƃ������Ƃ��ɓ��new�����̖h��
		placeobj.push_back(new PlaceFence());//��
		placeobj.push_back(new PlaceWood());//��
		placeobj.push_back(new PlaceEnemy());//�G
		placeobj.push_back(new PlaceChest());
		placeobj.push_back(new PlaceCameraPoints());
	}
	for (int i = 0; i < placeobj.size(); i++) {
		placeobj[i]->Initialize(camera);//���f���p�ӁA������
	}
}
void MapCreate::ObjectUpdate(DebugCamera* camera)
{
	for (int i = 0; i < placeobj.size(); i++) {
		placeobj[i]->Update(camera);//���f���X�V
	}
}

void MapCreate::ObjectArgment(DebugCamera* camera)
{
	if (savef) {
		if (!Error()) {//����������ł��G���[�����ɓ��Ă͂܂�����t�@�C���ւ̏������݂��Ȃ�
			for (int i = 0; i < placeobj.size(); i++) {
				placeobj[i]->FileWriting();//csv�ւ̏�������
			}
		}
		savef = false;
	}


	for (int i = 0; i < placeobj.size(); i++) {
		placeobj[i]->ArgMent(camera);//�z�u�I�u�W�F�N�g�̐ݒu�A�X�V
	}
}

void MapCreate::ImGuiDraw()
{	//ImGui�`�����
	for (int i = 0; i < placeobj.size(); i++) {
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
	for (int i = 0; i < placeobj.size(); i++) {
		placeobj[i]->Draw();//���f���A�ݒu���̕`��
	}
}


bool MapCreate::Error()
{
	//�G���[�E�B���h�E����
	if (placeobj[FENCE]->ErrorJudg() || placeobj[WOOD]->ErrorJudg() || placeobj[2]->ErrorJudg()) {
		WinApp::CreateErrorWindow(placeobj[ENEMY]->ErrorJudg(), TEXT("�G��1�̈ȏ�z�u���Ă�������"));
		WinApp::CreateErrorWindow(placeobj[FENCE]->ErrorJudg(), TEXT("���1�ȏ�z�u���Ă�������"));
		WinApp::CreateErrorWindow(placeobj[WOOD]->ErrorJudg(), TEXT("�؂�1�{�ȏ�z�u���Ă�������"));
		WinApp::CreateErrorWindow(placeobj[CHEST]->ErrorJudg(), TEXT("�󔠂͕K��5�z�u���Ă�������"));
		return true;
	}
	return false;
}
