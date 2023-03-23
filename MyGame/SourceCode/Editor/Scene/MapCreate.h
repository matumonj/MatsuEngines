#pragma once
#include"DebugCamera.h"
#include"Enemy.h"
#include"Wood.h"
#include"CreateObjManager.h"

class MapCreate
{
public:
	MapCreate();

	static void LoadEnemyParam();
private:
	//�ݒu�I�u�W�F�N�g[�؁E��E�G]
	enum
	{
		WOOD=0,
		FENCE=1,
		ENEMY=2,
		CHEST=3,
		STONE=4,
		GRASS=5
	};

	//�ݒu�I�u�W�F�N�g
	std::vector<CreateObjManager*> placeobj;
public:
	static MapCreate* GetIns();
	//ImGui
	void ImGuiDraw();
	//�ݒu����
	void ObjectArgment(DebugCamera* camera);
	//������
	void ObjectInitialize(DebugCamera* camera);
	//�X�V����
	void ObjectUpdate(DebugCamera* camera);
	//�`��
	void ObjectDraw();
	//�x���\��
	bool Error();
	//
	void Finalize();
private:
	bool savef;
};
