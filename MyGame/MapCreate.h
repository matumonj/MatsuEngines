#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include<memory>
#include<list>
#include <DirectXMath.h>
#include"DebugCamera.h"
#include"Enemy.h"
#include"Wood.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include"AreaFence.h"
#include <iostream>
#include"PlaceFence.h"
#include"PlaceWood.h"
#include"PlaceEnemy.h"
#include"CreateObjManager.h"
class MapCreate
{
public:
	MapCreate();

	static void LoadEnemyParam();
private:
	//�ݒu�I�u�W�F�N�g[�؁E��E�G]
	enum {
		WOOD=0,
		FENCE=1,
		ENEMY=2,
		CHEST=3
	};
	//�ݒu�I�u�W�F�N�g
	std::vector<CreateObjManager*> placeobj;
public:
	static MapCreate* GetInstance();
	//ImGui
	void ImGuiDraw();
	//�ݒu����
	void ObjectArgment(DebugCamera* camera);
	//������
	void ObjectInitialize(DebugCamera*camera);
	//�X�V����
	void ObjectUpdate(DebugCamera* camera);
	//�`��
	void ObjectDraw();
	//�x���\��
	bool Error();
private:
	bool savef;
	
};

